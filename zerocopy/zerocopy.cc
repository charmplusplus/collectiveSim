#include "zerocopy_sim.decl.h"

#define NUM_SIM 100000

class start : public CBase_start {
private:
    int doneCount{};
    double start_time;
public:
    start(CkArgMsg *m) {
        int MSG_SIZE = atoi(m->argv[1]);
        CProxy_sim simArray = CProxy_sim::ckNew(thisProxy, MSG_SIZE, 2);
        start_time = CkWallTimer();
    }

    void done() {
        doneCount++;
        if(doneCount == 2) {
            double end_time = CkWallTimer();
            ckout << "[SUCCESS] COMPLETED PING PONG USING ZERO-COPY" << endl;
            ckout << "Time Taken: " << end_time - start_time << " seconds" << endl;
            CkExit();
        }
    }
};

class sim : public CBase_sim {
private:
    CProxy_start startProxy;
    int cnt{};
    CkCallback zero_copy_callback;
    CkCallback dum_dum;
    int* recvData{};
    int MSG_SIZE{};
public:
    sim(CProxy_start _startProxy, int msgSize) : startProxy(_startProxy), MSG_SIZE(msgSize) {
        // SETUP
        zero_copy_callback = CkCallback(CkIndex_sim::local_done(NULL), thisProxy[thisIndex]);
        dum_dum = CkCallback(CkCallback::ignore);
        recvData = (int*)malloc(MSG_SIZE * sizeof(int));

        // START
        if(thisIndex == 0) {
            int data[MSG_SIZE];
            for(int i = 0; i < MSG_SIZE; i++) {
                data[i] = i * 42;
            }
            CkNcpyBuffer src(data, MSG_SIZE * sizeof(int), dum_dum, CK_BUFFER_UNREG);
            thisProxy[1].ping_pong(src);
            cnt++;
        }
    }

    void local_done(CkDataMsg *m) {
        // sanity check
        if(recvData[MSG_SIZE - 42] != (MSG_SIZE - 42) * 42) {
            CkAbort("[ERROR] Data integrity check failed");
        }

        cnt++;
        if(cnt == NUM_SIM) {
            startProxy.done();
        }
    }

    void ping_pong(CkNcpyBuffer src) {
        CkNcpyBuffer dst(recvData, MSG_SIZE * sizeof(int), zero_copy_callback, CK_BUFFER_UNREG);
        dst.get(src);
        thisProxy[(thisIndex + 1) % 2].ping_pong(src);
    }
};

#include "zerocopy_sim.def.h"
