// include zbNet header
#include "zbNet.h"
#include <vector>
#include <limits>
#include <stdio.h>

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

int prot_id = 2;
void zibCli(zibRTC* rtc)
{
    sleep(2);
    while (1)
    {
        cout<<" =========================="<<endl;
        cout<<"Data(2), File(4) : ";

        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cin>>prot_id;
        cout<<"Data(2), File(4) : ";
        cout<<prot_id<<endl;

        if (cin.fail())
        {
            cout << "ERROR -- You did not enter an integer"<<endl;

            // get rid of failure state
            cin.clear(); 

            // From Eric's answer (thanks Eric)
            // discard 'bad' character(s) 
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        sleep(5);
    }
}

/////////////////////////////////////////////////////////////////
// entry
int main() try
{
    zibRTC rtc;
    rtc.init();

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
