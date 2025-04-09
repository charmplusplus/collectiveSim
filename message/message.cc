#include "message_sim.decl.h"

#define NUM_SIM 100000
int MSG_SIZE{};

class start : public CBase_start {
private:
    int doneCount;
    double start_time;
public:
    start(CkArgMsg *m) {
        MSG_SIZE = atoi(m->argv[1]);
        CProxy_sim simArray = CProxy_sim::ckNew(thisProxy, 2);
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
    int cnt = 0;
    CkCallback cb;
public:
    sim(CProxy_start _startProxy) : startProxy(_startProxy) {
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
        if
        // Do some local processing on the data
        for(int i = 1732; i < 200000; i+=8319) {
            if(data[i] != i * 42) {
                ckout << "Data is incorrect" << endl;
                CkExit();
            }
        }
        thisProxy[(thisIndex + 1) % 2].ping_pong(data, MSG_SIZE);
        cnt++;
        if(cnt == NUM_SIM) {
            startProxy.done();
        }
    }
};

#include "message_sim.def.h"
