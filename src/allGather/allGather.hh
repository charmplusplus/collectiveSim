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
  std::vector<std::vector<int>> graph{};
  std::map<int, bool> recvFloodMsg{};
  int randCounter{};
  allGatherMsg *msg = new allGatherMsg;
  long int* data;

public:
  AllGather_SDAG_CODE

  AllGather(int k, int n);

  void startGather();

  int gen_rand();

  void Flood(int sender, long *data, int size, double recvTime);

  void init(long int* result, long int* data, CkCallback cb);

  void initdone();
};
