#include "allGather.hh"

double alpha;
double beta;

int AllGather::gen_rand() {
  std::mt19937_64 gen(randCounter++);
  std::uniform_int_distribution<int> dis(0, n - 1);
  return dis(gen);
}

AllGather::AllGather(int k, int n, int type) : k(k), n(n) {
  this->type = (allGatherType)type;
  switch (type) {
  case allGatherType::ALL_GATHER_HYPERCUBE: {
    if((n & (n - 1)) != 0) {
      CkPrintf("[ERROR] Hypercube only works when the size of chare array is a power of 2\n");
      CkExit();
    }
    numHypercubeIter = std::ceil(std::log2(n));
  } break;
  case allGatherType::ALL_GATHER_FLOODING: {
    graph.resize(n);
    for (int i = 0; i < n; i++) {
      graph[i].resize(n);
    }
    // Create a connected graph
    // Ring
    for (int i = 0; i < n; i++) {
      graph[i][(i + 1) % n] = 1;
      graph[i][(i - 1) % n] = 1;
    }
    // Random [n/2] connections
    for (int i = 0; i < (int)(n / 2); i++) {
      int x = gen_rand();
      int y = gen_rand();
      if (x != y) {
        graph[x][y] = 1;
        graph[y][x] = 1;
      }
    }
  } break;
  case allGatherType::ALL_GATHER_DEFAULT: {
  } break;
  }
}

// will be called only for index 0
void AllGather::initdone(int num) {
  if (num == n) {
    thisProxy.startGather();
  }
}

void AllGather::init(long int* result, long int* data, CkCallback cb) {
  this->cb = cb;
  this->store = result;
  this->data = data;
  int cnt = 1;
  CkCallback cbinitdone(CkReductionTarget(AllGather, initdone), thisProxy(0));
  contribute(sizeof(int), &cnt, CkReduction::sum_int, cbinitdone);
}

void AllGather::startGather() {
  switch (type) {
  case allGatherType::ALL_GATHER_DEFAULT: {
    for (int i = 0; i < k; i++) {
      store[k * thisIndex + i] = data[i];
    }
    numDefaultMsg++;
#ifdef TIMESTAMP
    thisProxy[(thisIndex + 1) % n].recvDefault(
        thisIndex, data, k, (timeStamp + alpha + beta * k * 8));
    timeStamp += alpha;
#else
    thisProxy[(thisIndex + 1) % n].recvDefault(thisIndex, data, k, 0.0);
#endif
    if (numDefaultMsg == n) {
      cb.send(msg);
    }
  } break;
  case allGatherType::ALL_GATHER_HYPERCUBE: {
    hyperCubeIndx.push_back(thisIndex);
    for (int i = 0; i < k; i++) {
      hyperCubeStore.push_back(data[i]);
    }
    thisProxy(thisIndex).Hypercube();
  } break;
  case allGatherType::ALL_GATHER_FLOODING: {
    for (int i = 0; i < k; i++) {
      store[k * thisIndex + i] = data[i];
    }
    numAccFloodMsg++;
    recvFloodMsg[thisIndex] = true;
    for (int i = 0; i < n; i++) {
      if (graph[thisIndex][i] == 1) {
#ifdef TIMESTAMP
        thisProxy(i).Flood(thisIndex, data, k,
                           (timeStamp + alpha + beta * k * 8));
        timeStamp += alpha;
#else
        thisProxy(i).Flood(thisIndex, data, k, 0.0);
#endif
      }
    }
    if (numAccFloodMsg == n) {
      cb.send(msg);
    }
  } break;
  }
}

void AllGather::recvDefault(int sender, long int data[], int _,
                            double recvTime) {
  numDefaultMsg++;
  for (int i = 0; i < k; i++) {
    store[k * sender + i] = data[i];
  }
#ifdef TIMESTAMP
  timeStamp = std::max(recvTime, timeStamp);
#endif
  if (((thisIndex + 1) % n) != sender) {
#ifdef TIMESTAMP
    thisProxy[(thisIndex + 1) % n].recvDefault(
        sender, data, k, (timeStamp + alpha + beta * k * 8));
    timeStamp += alpha;
#else
    thisProxy[(thisIndex + 1) % n].recvDefault(sender, data, k, 0.0);
#endif
  }
  if (numDefaultMsg == n) {
    cb.send(msg);
  }
}

void AllGather::Flood(int sender, long int data[], int _, double recvTime) {
  if (recvFloodMsg[sender]) {
    return;
  }
  numAccFloodMsg++;
  recvFloodMsg[sender] = true;
  for (int i = 0; i < k; i++) {
    store[k * sender + i] = data[i];
  }
#ifdef TIMESTAMP
  timeStamp = std::max(recvTime, timeStamp);
#endif
  for (int i = 0; i < n; i++) {
    if (graph[thisIndex][i] == 1 and i != sender) {
#ifdef TIMESTAMP
      thisProxy(i).Flood(sender, data, k, (timeStamp + alpha + beta * k * 8));
      timeStamp += alpha;
#else
      thisProxy(i).Flood(sender, data, k, 0.0);
#endif
    }
  }
  if (numAccFloodMsg == n) {
    cb.send(msg);
  }
}

#include "allGather.def.h"
