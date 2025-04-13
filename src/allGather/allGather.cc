#include "allGather.hh"

double alpha;
double beta;

int AllGather::gen_rand() {
  std::mt19937_64 gen(randCounter++);
  std::uniform_int_distribution<int> dis(0, n - 1);
  return dis(gen);
}

AllGather::AllGather(int k, int n) : k(k), n(n) {

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
  this->store = result;
  this->data = data;
  thisProxy[0].initdone();
}


void AllGather::startGather() {
  int currPE = CkMyPe();
  // for (int i = 0; i < k; i++) {
  //   store[k * currPE + i] = data[i];
  // }

  switch (type) {
  case allGatherType::ALL_GATHER_RING:
  case allGatherType::ALL_GATHER_HYPERCUBE: 
   break;
  case allGatherType::ALL_GATHER_FLOODING: {
    recvFloodMsg[currPE] = true;
    for (int i = 0; i < n; i++) {
      if (graph[currPE][i] == 1) {
        thisProxy[i].Flood(currPE, data, k, 0.0);
      }
    }
  } break;
  }
}


void AllGather::Flood(int sender, long* data, int size, double recvTime) {
  if (recvFloodMsg[sender]) {
    return;
  }
  recvFloodMsg[sender] = true;
  // for(int i=0;i<k;i++){
  //   store[k * sender + i] = data[i];
  // }

  // for (int i = 0; i < n; i++) {
  //   if (graph[CkMyPe()][i] == 1 and i != sender) {
  //     thisProxy[i].Flood(sender, data, size, 0.0);
  //   }
  // }
}

#include "allGather.def.h"
