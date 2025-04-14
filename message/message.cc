#include "message_sim.decl.h"

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
            ckout << "[SUCCESS] COMPLETED PING PONG USING MESSAGE PASSING" << endl;
            ckout << "Time Taken: " << end_time - start_time << " seconds" << endl;
            CkExit();
        }
    }
};

class sim : public CBase_sim {
private:
    CProxy_start startProxy;
    int cnt{};
    CkCallback cb;
    int MSG_SIZE{};
public:
    sim(CProxy_start _startProxy, int msgSize) : startProxy(_startProxy), MSG_SIZE(msgSize) {
        if(thisIndex == 0) {
            int data[MSG_SIZE];
            for(int i = 0; i < MSG_SIZE; i++) {
                data[i] = i * 42;
            }
            thisProxy[1].ping_pong(data, MSG_SIZE);
            cnt++;
        }
    }

    void ping_pong(int data[], int _) {
        // sanity check
        if(data[MSG_SIZE - 42] != (MSG_SIZE - 42) * 42) {
            CkAbort("[ERROR] Data integrity check failed");
        }

        thisProxy[(thisIndex + 1) % 2].ping_pong(data, MSG_SIZE);
        cnt++;
        if(cnt == NUM_SIM) {
            startProxy.done();
        }
    }
};

#include "message_sim.def.h"
