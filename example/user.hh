#include "user.decl.h"

#include "allGather.hh"

class start : public CBase_start {
private:
  int n;
  int k;
  int x;
  int y;
  CProxy_simBox sim;
  int numSizeGathered;
  long int* sizeArray;

public:
  start(CkArgMsg *msg);

  void fini(int numDone);

  void gatherSize(int arrayIndex, int dataSize);
};

class simBox : public CBase_simBox {
private:
  CProxy_start startProxy;
  long dataSize;
  int n;
  int x;
  int y;
  long int *data;
  long int *result;
  long int *dispArray;
  CProxy_AllGather allGatherProxy;

public:
  simBox(CProxy_start startProxy, CProxy_AllGather allGatherProxy, int n, int x, int y);

  void begin(long* dispArray, int size);

  void done(allGatherMsg *msg);
};
