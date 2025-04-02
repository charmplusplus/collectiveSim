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
  case allGatherType::ALL_GATHER_RING: {
  } break;
  }
}

// will be called only for index 0
void AllGather::initdone() {
  static int num_init_done = 0;
  num_init_done++;
  if (num_init_done == n) {
    thisProxy.startGather();
  }
}

void AllGather::init(long int* result, long int* data, CkCallback cb) {
  this->lib_done_callback = cb;
  zero_copy_callback = CkCallback(CkIndex_AllGather::local_buff_done(NULL), thisProxy[CkMyPe()]);
  dum_dum = CkCallback(CkCallback::ignore);
  this->store = result;
  this->data = data;
  thisProxy[0].initdone();
}

void AllGather::local_buff_done(CkDataMsg *m) {
  numRecvMsg++;
  if (numRecvMsg == n - 1) {
    lib_done_callback.send(msg);
  }
}

void AllGather::startGather() {
  int currPE = CkMyPe();
  for (int i = 0; i < k; i++) {
    store[k * currPE + i] = data[i];
  }
  CkNcpyBuffer src(data, k*sizeof(long int), dum_dum, CK_BUFFER_UNREG);

  switch (type) {
  case allGatherType::ALL_GATHER_RING: {
#ifdef TIMESTAMP
    thisProxy[(currPE + 1) % n].recvRing(
        currPE, src, (timeStamp + alpha + beta * k * 8));
    timeStamp += alpha;
#else
    thisProxy[(currPE + 1) % n].recvRing(currPE, src, 0.0);
#endif
  } break;
  case allGatherType::ALL_GATHER_HYPERCUBE: {
    hyperCubeIndx.push_back(currPE);
    hyperCubeStore.push_back(src);
    thisProxy[currPE].Hypercube();
  } break;
  case allGatherType::ALL_GATHER_FLOODING: {
    recvFloodMsg[currPE] = true;
    for (int i = 0; i < n; i++) {
      if (graph[currPE][i] == 1) {
#ifdef TIMESTAMP
        thisProxy[i].Flood(currPE, src,
                           (timeStamp + alpha + beta * k * 8));
        timeStamp += alpha;
#else
        thisProxy[i].Flood(currPE, src, 0.0);
#endif
      }
    }
  } break;
  }
}

void AllGather::recvRing(int sender, CkNcpyBuffer src, double recvTime) {
  CkNcpyBuffer dst(store + sender * k, k * sizeof(long int), zero_copy_callback, CK_BUFFER_UNREG);
  dst.get(src);
#ifdef TIMESTAMP
  timeStamp = std::max(recvTime, timeStamp);
#endif
  if (((CkMyPe() + 1) % n) != sender) {
#ifdef TIMESTAMP
    thisProxy[(CkMyPe() + 1) % n].recvRing(
        sender, src, (timeStamp + alpha + beta * k * 8));
    timeStamp += alpha;
#else
    thisProxy[(CkMyPe() + 1) % n].recvRing(sender, src, 0.0);
#endif
  }
}

void AllGather::Flood(int sender, CkNcpyBuffer src, double recvTime) {
  if (recvFloodMsg[sender]) {
    return;
  }
  recvFloodMsg[sender] = true;
  CkNcpyBuffer dst(store + sender * k, k * sizeof(long int), zero_copy_callback, CK_BUFFER_UNREG);
  dst.get(src);
#ifdef TIMESTAMP
  timeStamp = std::max(recvTime, timeStamp);
#endif
  for (int i = 0; i < n; i++) {
    if (graph[CkMyPe()][i] == 1 and i != sender) {
#ifdef TIMESTAMP
      thisProxy[i].Flood(sender, src, (timeStamp + alpha + beta * k * 8));
      timeStamp += alpha;
#else
      thisProxy[i].Flood(sender, src, 0.0);
#endif
    }
  }
}

#include "allGather.def.h"
