// include zbNet header
#include "zbNet.h"
#include <vector>
#include <limits>
#include <stdio.h>
#include <thread>

//////////////////////////////////////////////////////////////////
// window class 
class zibRTC
{
public:
    zbNet     _net;

/**
@brief  zibNet 의 초기화 작업 담당
*/
    void init() { CreateChild(); };

    // print pkeys only for nks svr
    void PrintPkeys()
    {
        print("******** pkeys\n");
        _net._nks.Print(); _net.SaveNks();
    };

protected:
    // create child window
    virtual void CreateChild()
    {
        setlocale(LC_ALL, "");

        // init net
        _net.Init(this, cbRcvNetStt);
    };

    ////////////////////////////////////
    // callback functions for net
    static int cbRcvNetStt(void* lnx, uchar ptc_id, char cmd_id, void* arg)
    {
        return ((zibRTC*)lnx)->cbRcvNet(ptc_id, cmd_id, arg);
    };
    int cbRcvNet(uchar ptc_id, char cmd_id, void* arg)
    {
        switch(ptc_id)
        {
        //case 0: return cbRcvNetPtcBrdc(cmd_id, arg);
        //case 1: return cbRcvNetPtcCnnt(cmd_id, arg);
        //case 2: return cbRcvNetPtcData(cmd_id, arg); 
        //case 3: return cbRcvNetPtcLrgd(cmd_id, arg);
        //case 4: return cbRcvNetPtcFile(cmd_id, arg);
        }
        return -1;
    };
};

/////////////////////////////////////////////////////////////////
void mainConvWC4to2(wchar* wc, ushort* c, const ushort& n) { for (ushort i = 0; i < n; ++i) { c[i] = (ushort)wc[i]; } };

void zibCli(zibRTC* rtc)
{
    while (1)
    {
        cout<<" =========================="<<endl;
        cout<<" Input Command : ";
        string cmd = "";
        cin>>cmd;

        if (cmd == "p" || cmd == "print" || cmd == "P")
        {
            rtc->_net._nks.Print();
        }
        else
        {
            cout<<" support command : p, print, P"<<endl;
        }
    }
}

/////////////////////////////////////////////////////////////////
// entry
int main() try
{
    zibRTC rtc;
    rtc.init();

    // for debug
    std::thread cli(zibCli, &rtc);

    while (1) {
        sleep(7);
        //rtc.PrintPkeys();
    }

    return 0;
}

catch(kmException e)
{
    print("* kmnet.cpp catched an exception\n");
    kmPrintException(e);
    system("pause");
    return 0;
}
