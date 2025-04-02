#pragma once

#include "allGather.decl.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <map>
#include <utility>
#include <vector>
#include <random>

class allGatherMsg : public CMessage_allGatherMsg {};

// NB: ALL_GATHER_HYPERCUBE only works when the size of chare array is a power of 2.
enum allGatherType {
  ALL_GATHER_RING,
  ALL_GATHER_HYPERCUBE,
  ALL_GATHER_FLOODING
};

class AllGather : public CBase_AllGather {
private:
  int k{};
  int n{};
  long int *store;
  int numRecvMsg{};
  double timeStamp{};
  CkCallback lib_done_callback;
  allGatherType type;
  int numHypercubeIter{};
  int iter;
  int HypercubeToSend;
  std::vector<std::vector<int>> graph{};
  std::map<int, bool> recvFloodMsg{};
  int randCounter{};
  std::vector<int> hyperCubeIndx{};
  std::vector<CkNcpyBuffer> hyperCubeStore{};
  allGatherMsg *msg = new allGatherMsg;
  long int* data;
  CkCallback zero_copy_callback;
  CkCallback dum_dum;

public:
  AllGather_SDAG_CODE

  AllGather(int k, int n, int type);

  void startGather();

  void recvRing(int sender, CkNcpyBuffer data, double recvTime);

  void local_buff_done(CkDataMsg *m);

  int gen_rand();

  void Flood(int sender, CkNcpyBuffer data, double recvTime);

  void init(long int* result, long int* data, CkCallback cb);

  void initdone();
};
