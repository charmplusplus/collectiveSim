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
  ALL_GATHER_DEFAULT,
  ALL_GATHER_HYPERCUBE,
  ALL_GATHER_FLOODING
};

class AllGather : public CBase_AllGather {
private:
  int k{};
  int n{};
  long int *store;
  int numDefaultMsg{};
  double timeStamp{};
  CkCallback cb;
  allGatherType type;
  int numHypercubeIter{};
  int iter;
  int HypercubeToSend;
  int numAccFloodMsg{};
  std::vector<std::vector<int>> graph{};
  std::map<int, bool> recvFloodMsg{};
  int randCounter{};
  std::vector<int> hyperCubeIndx{};
  std::vector<long int> hyperCubeStore{};
  allGatherMsg *msg = new allGatherMsg;
  long int* data;

public:
  AllGather_SDAG_CODE

  AllGather(int k, int n, int type);

  void startGather();

  void recvDefault(int sender, long int data[], int _, double recvTime);

  int gen_rand();

  void Flood(int sender, long int data[], int _, double recvTime);

  void init(long int* result, long int* data, CkCallback cb);

  void initdone(int num);
};
