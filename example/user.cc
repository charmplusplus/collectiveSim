#include "user.hh"

start::start(CkArgMsg *msg) {
  if (msg->argc != 5) {
    ckout << "Usage: " << msg->argv[0]
          << " <chare_array_size> <num_data_points_per_chare_array_element> "
             "<num_bits_for_pe> <num_bits_for_data_points>"
          << endl;
    CkExit();
  }

  n = atoi(msg->argv[1]);
  k = atoi(msg->argv[2]);// TODO: remove this
  x = atoi(msg->argv[3]);
  y = atoi(msg->argv[4]);
  sizeArray = (long int *)malloc((n + 1) * sizeof(long int));
  delete msg;

  #ifdef FLOODING
   CProxy_AllGather allGatherProxy = CProxy_AllGather::ckNew(n, (int)allGatherType::ALL_GATHER_FLOODING);
  #endif
  
  #ifdef HYPERCUBE
   CProxy_AllGather allGatherProxy = CProxy_AllGather::ckNew(n, (int)allGatherType::ALL_GATHER_HYPERCUBE);
  #endif
  
  #ifdef RING
  CProxy_AllGather allGatherProxy = CProxy_AllGather::ckNew(n, (int)allGatherType::ALL_GATHER_RING);
  #endif

  sim = CProxy_simBox::ckNew(thisProxy, allGatherProxy, n, x, y, n);
}

void start::fini(int numDone) {
  if (numDone == n) {
    ckout << "[STATUS] Completed the AllGather Simulation" << endl;
    CkExit();
  }
}
void start::gatherSize(int arrayIndex, int dataSize) {
  sizeArray[arrayIndex] = dataSize;
  numSizeGathered++;
  if(numSizeGathered==n){
    long int* dispArray = (long int *)malloc((n + 1) * sizeof(long int));
    // do a prefix sum
    dispArray[0] = 0;
    for (int i = 1; i < n + 1; i++) {
      dispArray[i] = dispArray[i - 1] + sizeArray[i - 1];
    }
    // send the displacement arrays to begin
    sim.begin(dispArray, n+1);
    
  }
}

simBox::simBox(CProxy_start startProxy, CProxy_AllGather allGatherProxy, int n, int x, int y)
    : startProxy(startProxy), allGatherProxy(allGatherProxy), n(n), x(x), y(y) {
  srand(thisIndex);
  dataSize = random()%100+1; 
  data = (long int *)malloc(dataSize*sizeof(long int));
  long int max_serial = (1 << y) - 1;
  long int base = thisIndex;
  while (max_serial > 0) {
    base = base * 10;
    max_serial = max_serial / 10;
  }
  for (int i = 0; i < dataSize; i++) {
    data[i] = base + i;
  }
  startProxy.gatherSize(thisIndex, dataSize);
}

void simBox::begin(long* dispArray, int _) {
  this->dispArray = dispArray;
  result = (long int *)malloc(dispArray[n]*sizeof(long int));
  CkCallback cb(CkIndex_simBox::done(NULL), CkArrayIndex1D(thisIndex), thisProxy);
  AllGather* libptr = allGatherProxy.ckLocalBranch();
  libptr->init(result, data, dispArray, thisIndex, cb);
}

void simBox::done(allGatherMsg *msg) {
  bool success = true;
  for(int i = 0; i < n; i++) {
    long int max_serial = (1 << y) - 1;
    long int base = i;
    while (max_serial > 0) {
      base = base * 10;
      max_serial = max_serial / 10;
    }
    long int dataSize = dispArray[i+1] - dispArray[i];
    int offset = dispArray[i];
    for(int j = 0; j < dataSize; j++) {
      if(result[offset + j] != base + j) {
        success = false;
        break;
      }
    }
    if(!success) break;
  }

  if(success) ckout << "[STATUS] Correct result for Chare " << thisIndex << endl;
  else {
    ckout << "[STATUS] Incorrect result for Chare " << thisIndex << endl;
    for(int i = 0; i < dispArray[thisIndex+1] - dispArray[thisIndex]; i++) {
      ckout << result[i] << " ";
    }
    ckout << endl;
  }
  int cnt = 1;
  CkCallback cbfini(CkReductionTarget(start, fini), startProxy);
  contribute(sizeof(int), &cnt, CkReduction::sum_int, cbfini);
}

#include "user.def.h"
