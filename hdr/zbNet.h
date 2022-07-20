#ifndef __zbNet_H_INCLUDED_2022_04_07__
#define __zbNet_H_INCLUDED_2022_04_07__

/* Note ----------------------
* zbNet has been created by Choi, Kiwan
* This is version 1
* zbNet is based on kmNet (ver. 7)
*/

// base header
#include "km7Net.h"
#include <sstream>

/////////////////////////////////////////////////////////////////////////////////////////
// zbNet class

///////////////////////////////////////////////////////////
// zbNet main class

// zibsvr notify enum class
enum class zbNoti : int { none };

// zibsvr mode enum class
enum class zbMode : int { svr, clt, nks };

// zibsvr main class
class zbNet : public kmNet
{
public:
    zbMode      _mode = zbMode::nks;  // svr (server), clt (client), nks ( net key signaling server)
    ushort      _port{DEFAULT_PORT}; // port
    kmStrw      _path{};     // zibsvr root's path
    kmNetNks    _nks;        // net key signaling function only for zbMode::nks
    kmWorks     _wrks;       // worker to send data

    // init
    void Init(void* parent, kmNetCb netcb)
    {
        // init kmnet
        kmNet::Init(parent, netcb, _port);

        setAndMakeDir();

        // create network key table only for nks
        if(LoadNks() == 0) _nks.Create();
    };

    // enqueue work
    template<typename... Ts>
    void EnqueueWork(int id, Ts... args) { _wrks.Enqueue(id, args...); }

    ///////////////////////////////////////////////
    // virtual functions for rcv callback
protected:
    // virtual callback for ptc_nkey
    //  cmd_id 0 : rcv reqkey, 1 : rcv key, 2 : rcv reqaddr, 3 : rcv addr
    virtual void vcbRcvPtcNkey(char cmd_id)
    {
        switch(cmd_id)
        {
        case 0 : RcvNkeyReqKey (); break;
        case 1 : RcvNkeyKey    (); break;
        case 2 : RcvNkeyReqAddr(); break;
        case 3 : RcvNkeyAddr   (); break;
        case 4 : RcvNkeySig    (); break;
        case 5 : RcvNkeyRepSig (); break;
        case 6 : RcvNkeyReqAccs(); break;
        }
    };
    void RcvNkeyReqKey() // _rcv_addr, _rcv_mac ->_snd_key
    {
        _ptc_nkey._snd_key = _nks.Register(_ptc_nkey._rcv_mac, _ptc_nkey._rcv_addr);
        SaveNks();
    };
    void RcvNkeyKey()
    {
    };
    void RcvNkeyReqAddr() // _rcv_key, _rcv_mac -> _snd_addr
    {
        _ptc_nkey._snd_addr = _nks.GetAddr(_ptc_nkey._rcv_key, _ptc_nkey._rcv_mac);
    };
    void RcvNkeyAddr()
    {
    };
    void RcvNkeySig() // only for nks
    {
        if(_mode != zbMode::nks) return;        

        // update key
        int flg = _nks.Update(_ptc_nkey._rcv_key, _ptc_nkey._rcv_mac, _ptc_nkey._rcv_addr);

        if(flg >= 0) ReplyNksSig(_ptc_nkey._rcv_addr, flg);
    };
    void RcvNkeyRepSig()
    {
        int flg = _ptc_nkey._rcv_sig_flg;
    };
    void RcvNkeyReqAccs()
    {
        static kmAddr4 trg_addr; trg_addr = _ptc_nkey._rcv_addr;

        // preconnect in work thread
        EnqPreconnect(trg_addr);
    };

    void EnqPreconnect(kmAddr4 addr) { EnqueueWork(3, addr); };

    /////////////////////////////////////////////////////////////
    void convWC4to2(wchar* wc, ushort* c, const ushort& n) { for (ushort i = 0; i < n; ++i) { c[i] = (ushort)wc[i]; } };
    void convWC2to4(ushort* c, wchar* wc, const ushort& n) { for (ushort i = 0; i < n; ++i) { wc[i] = (wchar)c[i]; } };

    /////////////////////////////////////////////////////////////
    // nsk functions
public:
    // save nks table
    void SaveNks()
    {
        kmStrw path(L"%S/nkstable", _path.P());

        _nks.Save(path.P());
    };

    // load nks table
    int LoadNks() try
    {
        kmStrw path(L"%S/nkstable", _path.P());

        return _nks.Load(path.P());
    }
    catch(kmException) { return 0; };

    void setAndMakeDir()
    {
        _path = kmStrw(L"/home/kktjin/backup/keystrg");

        kmFile::MakeDir(_path.P());
    }
};

#endif /* __zbNet_H_INCLUDED_2022_04_07__ */
