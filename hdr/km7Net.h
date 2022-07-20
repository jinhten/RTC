#ifndef __km7Net_H_INCLUDED_2021_05_31__
#define __km7Net_H_INCLUDED_2021_05_31__

/* Note ----------------------
* kmMat has been created by Choi, Kiwan
* This is version 7
* kmMat v7 is including the following
*   - km7Define.h
*   - km7Define.h -> km7Mat.h
*   - km7Define.h -> km7Mat.h -> km7Wnd.h
*   - km7Define.h -> km7Mat.h -> kc7Mat.h -> km7Dnn.h
*   - km7Define.h -> km7Mat.h -> kc7Mat.h
*   - km7Define.h -> km7Mat.h -> km7Net.h
*/

// reserved port list
//
// FTP   : 21
// HTTP  : 80
// HTTPS : 8080

// base header
#include "km7Mat.h"

// general network header
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>
#include <wchar.h>
#include <errno.h>

///////////////////////////////////////////////////////////////
// enum and definition for net

// * Note that TCP's MTU is from ethernet v2's limit, 1500byte.
#define TCP_MTU_BYTE       1500
#define TCP_MSS_BYTE       1460  // 1500 - 20 (ip header) - 20 (tcp header)

// * Note that UDP's MTU and MSS are only for efficient transmit.
#define UDP_MTU_BYTE       1500
#define UDP_MSS_BYTE       (UDP_MTU_BYTE - 28) // 1500 - 20 (ip header) - 8 (udp header)
//#define UDP_MSS_BYTE       34000 - 28 // 1500 - 20 (ip header) - 8 (udp header)

#define UDP_MAX_PKT_BYTE   65535
#define UDP_MAX_DATA_BYTE  65507 // 65525 - 20 (ip header) - 8 (udp header)

//#define DEFAULT_PORT 60215
#define DEFAULT_PORT 60165
#define RTC_PORT 60165

#define INVALID_SOCKET -1

#define ID_NAME 64

// socket type... tcp, udp
enum class kmSockType : int { tcp = 0, udp = 1 };

// nat type... full(full cone), rstr(restricted), prst(port-restricted), symm (symmetric)
enum class kmNatType { full, rstr, prst, symm };

///////////////////////////////////////////////////////////////////
// transfer functions between network and host
// network : big endian
// host    : big endian (linux ... ) or little endian (windows)

inline uchar  ntoh(uchar  a) { return        a ; };
inline wchar  ntoh(wchar  a) { return ntohs (a); };
inline ushort ntoh(ushort a) { return ntohs (a); };
inline uint   ntoh(uint   a) { return ntohl (a); };
inline uint64 ntoh(uint64 a) { return (((uint64)ntohl(a)) << 32) + ntohl(a >> 32); };

inline char   ntoh(char   a) { return        a ; };
inline short  ntoh(short  a) { return ntohs (a); };
inline int    ntoh(int    a) { return ntohl (a); };
inline int64  ntoh(int64  a) { return (((int64)ntohl(a)) << 32) + ntohl(a >> 32); };

//inline float  ntoh(float  a) { return ntohf(*((uint*  )&a)); };
//inline double ntoh(double a) { return ntohd(*((uint64*)&a)); };

inline uchar  hton(uchar  a) { return        a ; };
inline wchar  hton(wchar  a) { return htons (a); };
inline ushort hton(ushort a) { return htons (a); };
inline uint   hton(uint   a) { return htonl (a); };
inline uint64 hton(uint64 a) { return (((uint64)htonl(a)) << 32) + htonl(a >> 32); };

inline char   hton(char   a) { return        a ; };
inline short  hton(short  a) { return htons (a); };
inline int    hton(int    a) { return htonl (a); };
inline int64  hton(int64 a) { return (((int64)htonl(a)) << 32) + htonl(a >> 32); };

//inline float  hton(float  a) { uint   b = htonf(a); return *((float* )&b); };
//inline double hton(double a) { uint64 b = htond(a); return *((double*)&b); };

template<typename T> T& ntoh(T& a) { return a; }
template<typename T> T& hton(T& a) { return a; }

//////////////////////////////////////////////////////////////
// class for address

// mac address... 8byte
class kmMacAddr
{
public:
    union { uint64 i64; uchar c[8]; };

    // constructor
    kmMacAddr()         { i64 = 0; };
    kmMacAddr( int   a) { i64 = a; };
    kmMacAddr( int64 a) { i64 = a; };
    kmMacAddr(uchar* a) { Set(        a); };
    kmMacAddr( char* a) { Set((uchar*)a); };

    kmMacAddr(const kmMacAddr& a) { i64 = a.i64; };

    // set from char[8] or uchar[8]
    void Set(uchar* addr) { for(int i = 0; i < 8; ++i) c[i] = *(addr + i); };

    // assignment operator
             kmMacAddr& operator=(const          kmMacAddr& a)          { i64 = a.i64; return *this; };    
    volatile kmMacAddr& operator=(const volatile kmMacAddr& a) volatile { i64 = a.i64; return *this; };

    // operator
    bool operator==(const kmMacAddr& b) const { return i64 == b.i64; };
    bool operator!=(const kmMacAddr& b) const { return i64 != b.i64; };

    // get string
    kmStra GetStr() const
    {
        return kmStra("%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X", c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]);
    };
    kmStrw GetStrw() const
    {
        return kmStrw(L"%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X", c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]);
    };
};
typedef kmMat1<kmMacAddr> kmMacAddrs;

// ipv4 address's state... valid, invalid, pending
enum class kmAddr4State : ushort { valid = 0, invalid = 1, pending = 2 };

// ipv4 address... compatible with sockaddr_in (4 + 2 + 2 byte)
class kmAddr4
{
public:
    union { uint ip; uchar c[4];}; // compatible with sockaddr_in.sin_addr.s_addr
    ushort port{};                 // compatible with sockaddr_in.sin_port
    kmAddr4State state{};          // optional * Note that it's 8 byte even without this

    // constructor
    kmAddr4()                        :                ip(0)        {};
    kmAddr4(kmAddr4State state)      : state(state) , ip(0)        {};
    kmAddr4(int     a, ushort p = 0) : port(hton(p)), ip(a)        {};
    kmAddr4(uint    a, ushort p = 0) : port(hton(p)), ip(a)        {};
    kmAddr4(ulong   a, ushort p = 0) : port(hton(p)), ip(a)        {};
    kmAddr4(in_addr a, ushort p = 0) : port(hton(p)), ip(a.s_addr) {};
    kmAddr4(LPCSTR  s, ushort p = 0) : port(hton(p)) { inet_pton(AF_INET, s, &ip); };
    //kmAddr4(LPCWSTR s, ushort p = 0) : port(hton(p)) { InetPtonW(AF_INET, s, &ip); };
    kmAddr4(sockaddr_in saddr)
    {
        ip = saddr.sin_addr.s_addr; port = saddr.sin_port;
    };
    kmAddr4(uchar a0, uchar a1, uchar a2, uchar a3, ushort p = DEFAULT_PORT)
    {
        c[0] = a0; c[1] = a1; c[2] = a2; c[3] = a3; port = htons(p);
    };    

    // operator
    uchar operator()(int i) { return c[i]; };

    bool operator==(const kmAddr4& b) { return (ip == b.ip && port == b.port); };
    bool operator!=(const kmAddr4& b) { return (ip != b.ip || port != b.port); };

    // get sockaddr_in
    // * note that if ip is 0, it measns that INADDR_ANY
    sockaddr_in GetSckAddr()
    {
        sockaddr_in saddr;
        saddr.sin_family      = AF_INET;  // ipv4
        saddr.sin_addr.s_addr = ip;       // address (4 byte)
        saddr.sin_port        = port;     // port (2 byte)
        return saddr;
    };

    // get string
    kmStra GetStr   () const { return kmStra( "%d.%d.%d.%d:%d",c[0], c[1], c[2],c[3], ntohs(port)); };
    kmStrw GetStrw  () const { return kmStrw(L"%d.%d.%d.%d:%d",c[0], c[1], c[2],c[3], ntohs(port)); };
    kmStra GetIpStr () const { return kmStra( "%d.%d.%d.%d"   ,c[0], c[1], c[2],c[3]); };
    kmStrw GetIpStrw() const { return kmStrw(L"%d.%d.%d.%d"   ,c[0], c[1], c[2],c[3]); };

    // get state
    bool isValid  () const { return state == kmAddr4State::valid;   };
    bool IsInvalid() const { return state == kmAddr4State::invalid; };
    bool IsPending() const { return state == kmAddr4State::pending; };

    // set state
    void SetValid  () { state = kmAddr4State::valid;   };
    void SetInvalid() { state = kmAddr4State::invalid; };
    void SetPending() { state = kmAddr4State::pending; };

    // get port
    ushort GetPort() const { return ntoh(port); };

    // set port
    void SetPort(ushort p) { port = hton(p); };

    // print
    void Print() { print("* addr : %s\n", GetStr().P()); };
};
typedef kmMat1<kmAddr4> kmAddr4s;

///////////////////////////////////////////////////////////////
// socket class with winsock2 and tcp/ip
//
// * Note that you should not define a destructor which will close the socket.
//
class kmSock
{
public:
    int    _sck   = INVALID_SOCKET;
    int    _state = 0; // 0 : free, 1 : socket, 2 : bind, 3: listen, 4: connected

    void Init() { _sck = INVALID_SOCKET; _state = 0; };

    /////////////////////////////////
    // static functions

    static bool GetIntfAddr(kmAddr4& ipAddr)
    {
        char buf[8192] = {0,};
        struct ifconf ifc = {0,};
        struct ifreq *ifr = NULL;
        int sck = 0;
        char ip[INET6_ADDRSTRLEN] = {0,};
        struct sockaddr_in *addr;

        /* Get a socket handle. */
        sck = socket(PF_INET, SOCK_DGRAM, 0);
        if(sck < 0)
        {
            perror("socket");
            return false;
        }

        /* Query available interfaces. */
        ifc.ifc_len = sizeof(buf);
        ifc.ifc_buf = buf;
        if(ioctl(sck, SIOCGIFCONF, &ifc) < 0)
        {
            perror("ioctl(SIOCGIFCONF)");
            return false;
        }

        /* Iterate through the list of interfaces. */
        ifr = ifc.ifc_req;
        addr = (struct sockaddr_in*)&(ifr[1].ifr_addr);

        /* Get the IP address*/
        if(ioctl(sck, SIOCGIFADDR, &ifr[1]) < 0)
        {
            perror("ioctl(OSIOCGIFADDR)");
        }

        if (inet_ntop(AF_INET, &(((struct sockaddr_in *)addr)->sin_addr), ip, sizeof ip) == NULL) //vracia adresu interf
        {
            perror("inet_ntop");
            return false;
        }

        kmAddr4 tempIpAddr(ip, DEFAULT_PORT);
        ipAddr = tempIpAddr;

        close(sck);
        return true;
    };

    // get WSA error
    static kmStra GetErrStr(int err_code = 0)
    {
        const char* str = nullptr;
        const int   err = err_code;//(err_code == 0) ? WSAGetLastError():err_code;

        switch(err)
        {
        case EISCONN            : str = "the socket is not connected"; break;
        case EPIPE              : str = "the connection has been broken"; break;
/*
        case WSANOTINITIALISED  : str = "not initialized"; break;
        case WSAENETDOWN        : str = "the network has failed"; break;
        case WSAEFAULT          : str = "the buf is not completely contained"; break;
        case WSAENOTCONN        : str = "the socket is not connected"; break;
        case WSAENETRESET       : str = "the connection has been broken"; break;
        case WSAESHUTDOWN       : str = "the socket has been shut down"; break;
        case WSAECONNABORTED    : str = "the connection was aborted by your host"; break;
        case WSAECONNRESET      : str = "the existing connection was forcibly closed"; break;
        case WSASYSNOTREADY     : str = "wsastartup cannot function"; break;
        case WSAVERNOTSUPPORTED : str = "the version requested is not supported"; break;
        case WSAEINPROGRESS     : str = "a blocking is currently executing"; break;
        case WSAENOTSOCK        : str = "the socket is not valid"; break;
        case WSAEADDRINUSE      : str = "the address is already in use"; break;
        case WSAEISCONN         : str = "the socket is already connected"; break;
*/
        default                 : str = "default";
        }
        return kmStra("wsa err %d (%s)", err, str);
    };

    /////////////////////////////////
    // interface member functions

    // get socket
    // network address type : IPv4(AF_INET), IPv6(AF_INET6)
    // socket type          : TCP(SOCK_STREAM), UDP(SOCK_DGRAM)
    // protocol             : TCP(IPPROTO_TCP), UDP(IPPROTO_UDP)    
    void GetSocket(kmSockType type = kmSockType::tcp)
    {
        ASSERTFA(_state == 0, "kmSock::GetSocket in 172");

        if(type == kmSockType::tcp) _sck = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        else                        _sck = socket(AF_INET, SOCK_DGRAM , IPPROTO_UDP);

        if(_sck != INVALID_SOCKET) _state = 1;

        ASSERTFA(_sck != INVALID_SOCKET, "kmSock::GetSocket in 178");
    };

    // get rcv buffer size
    int GetRcvBufByte()
    {
        int byte = 0; socklen_t len = sizeof(byte);

        getsockopt(_sck, SOL_SOCKET, SO_RCVBUF, (char*)&byte, &len);

        return byte;
    };    

    // get snd buffer size
    int GetSndBufByte()
    {
        int byte = 0; socklen_t len = sizeof(byte);

        getsockopt(_sck, SOL_SOCKET, SO_SNDBUF, (char*)&byte, &len);

        return byte;
    };

    // set rcv and snd buffer size
    int SetRcvBufByte(int byte) { return setsockopt(_sck, SOL_SOCKET, SO_RCVBUF, (char*)&byte, sizeof(byte)); };
    int SetSndBufByte(int byte) { return setsockopt(_sck, SOL_SOCKET, SO_SNDBUF, (char*)&byte, sizeof(byte)); };


    // get socket type
    kmSockType GetSckType() const
    {
        int type; socklen_t len = sizeof(int);

        getsockopt(_sck, SOL_SOCKET, SO_TYPE, (char*)&type, &len);

        return (type == SOCK_STREAM) ? kmSockType::tcp : kmSockType::udp;
    };
    
    // bind
    //  [return] 0                : if no error occurs
    //           SOCKET_ERROR(-1) : otherwise
    int Bind(kmAddr4 addr, kmSockType type = kmSockType::tcp)
    {
        // get socket
        if(_state == 0) GetSocket(type);

        // bind
        sockaddr_in sckaddr = addr.GetSckAddr();

        int ret = ::bind(_sck, (LPSOCKADDR)&sckaddr, sizeof(sckaddr));

        if(ret == 0) _state = 2;

        return ret;
    };

    // listen (for server)
    //  [return] 0                : if no error occurs
    //           SOCKET_ERROR(-1) : otherwise
    int Listen()
    {
        const int ret = ::listen(_sck, SOMAXCONN);

        if(ret == 0) _state = 3;

        return ret;
    };

    // accept client (for server)
    kmSock Accept()
    {
        kmSock client; sockaddr_in saddr; socklen_t len = sizeof(saddr);
    
        client._sck   = ::accept(_sck, (sockaddr*)&saddr, &len);

        if(client._sck != INVALID_SOCKET) client._state = 4;

        return client;
    };

    // connect to server (for client)
    //  [return] 0                : if no error occurs
    //           SOCKET_ERROR(-1) : otherwise
    int Connect(kmAddr4 addr, kmSockType type = kmSockType::tcp)
    {
        // get socket
        if(_state == 0) GetSocket(type);

        // connect
        sockaddr_in saddr = addr.GetSckAddr();

        int ret = ::connect(_sck, (LPSOCKADDR)&saddr, sizeof(saddr));

        if(ret == 0) _state = 4;

        return ret;
    };

    // receive and send    
    //   [return] 0 < : the number of bytes received or sent.
    //            0   : end of receiving process (Recv only)
    //            SOCKET_ERROR(-1) : you should call WSAGetLastError() to get error code.
    int Send(const kmStra& str) { return ::send(_sck, str.P(), (int)str.GetLen(), 0); };
    int Recv(      kmStra& str) { return ::recv(_sck, str.P(), (int)str.Size()  , 0); };

    // recive from (for udp)
    int Recvfrom(kmStra& str, kmAddr4& addr)
    {
        sockaddr_in saddr; socklen_t len = sizeof(saddr);

        int ret = ::recvfrom(_sck, str.P(), (int) str.Size(), 0, (sockaddr*)&saddr, &len);

        addr = kmAddr4(saddr);

        return ret;
    };

    // recive from (for udp)
    template<typename T> int Recvfrom(kmMat1<T>& data, kmAddr4& addr)
    {
        sockaddr_in saddr; socklen_t len = sizeof(saddr);

        int ret = ::recvfrom(_sck, data.P(), (int) data.Byte(), 0, (sockaddr*)&saddr, &len);

        data.SetN1(MAX(0, ret));

        addr = kmAddr4(saddr);

        return ret;
    }

    // send to (for udp)
    int Sendto(const kmStra& str, kmAddr4 addr)
    {
        sockaddr_in saddr = addr.GetSckAddr();

        return ::sendto(_sck, str.P(), (int)str.GetLen(), 0, (sockaddr*)&saddr, sizeof(saddr));
    };

    // send to (for udp)
    template<typename T> int Sendto(const kmMat1<T>& data, kmAddr4 addr)
    {
        sockaddr_in saddr = addr.GetSckAddr();

        return ::sendto(_sck, data.P(), (int)data.N1()*sizeof(T), 0, (sockaddr*)&saddr, sizeof(saddr));
    }

    // send to (for udp broadcast) using global broadcast address (255.255.255.255)
    template<typename T> int SendtoBroadcast(const kmMat1<T>& data, ushort port = DEFAULT_PORT)
    {
        sockaddr_in saddr;
        saddr.sin_family      = AF_INET;
        saddr.sin_addr.s_addr = 0xffffffff; 
        saddr.sin_port        = htons(port);

        return ::sendto(_sck, data.P(), (int)data.N1()*sizeof(T), 0, (sockaddr*)&saddr, sizeof(saddr));
    };

/*
    // send to (for udp broadcast) using local broadcast address (ex. 10.114.75.255)
    template<typename T> int SendtoBroadcastLocal(const kmMat1<T>& data, ushort port = DEFAULT_PORT)
    {
        kmAddr4      addr = kmSock::GetBrdcAddr(port);
        sockaddr_in saddr = addr.GetSckAddr();

        return ::sendto(_sck, data.P(), (int)data.N1()*sizeof(T), 0, (sockaddr*)&saddr, sizeof(saddr));
    };
*/

    // shutdown
    //   [ how ] SD_RECEIVE (0) : shutdown receive operations
    //           SD_SEND    (1) : shutdown send operations
    //           SD_BOTH    (2) : shutdown both send and receive operations
    int Shutdown(int how = SD_BOTH)
    {
        if(_state == 0) return 0;

        const int ret = ::shutdown(_sck, how);

        if(ret == 0) _state = 1;

        return ret;
    };

    // close socket
    int Close()
    {
        if(_state == 0) return 0;

        const int ret = ::close(_sck); Init();

        return ret;
    };

    // get state
    //   0 : free, 1 : socket, 2 : bind, 3: listen, 4: connected
    int GetState() const { return _state; };

    // get source address
    kmAddr4 GetSrcAddr() const 
    {
        sockaddr_in saddr; socklen_t len = sizeof(saddr);

        getsockname(_sck, (sockaddr*)&saddr, &len);

        return kmAddr4(saddr);
    };

    // get destination address
    kmAddr4 GetDstAddr() const
    {
        sockaddr_in saddr; socklen_t len = sizeof(saddr);

        getpeername(_sck, (sockaddr*)&saddr, &len);

        return kmAddr4(saddr);
    };
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// network class for udp
//
class kmUdp
{
protected:    
    kmSock   _sck;   // client sockets
    kmThread _thrd;  // receiving threads

public:
    // constructor
    kmUdp() { Init(); };
    
    // destructor
    virtual ~kmUdp() { CloseAll(); };

    // init
    void Init() {};

    ///////////////////////////////////////////////
    // interface functions

    // bind
    int Bind(kmAddr4 addr) { return _sck.Bind(addr, kmSockType::udp); };

    // bind to own ip
/*
    int Bind(ushort port = DEFAULT_PORT)
    {
        return _sck.Bind(kmSock::GetLocalAddr(port), kmSockType::udp);
    };
*/
    // connect    
    int Connect(kmAddr4 addr = INADDR_ANY)
    {
        if(addr.port == 0) addr.port = htons(DEFAULT_PORT);

        return _sck.Connect(addr);
    };

    // receiving process for _sck(isck)
    //   [return] 0   : end of receiving process
    //            0 < : the number of bytes received or sent.
    //            0 > : error of the socket
    //
    // * Note that this is an example. So you should re-define this virtual function.
    virtual int RcvProc()
    {
        kmStra str(32); kmAddr4 addr;
    
        const int n = _sck.Recvfrom(str, addr);
    
        return n;
    };

    // send data to every connected peer
    void Send(const kmStra& str, kmAddr4 addr)
    {
        _sck.Sendto(str, addr);
    };

    // close isck socket and thread
    void Close() { _sck.Close(); };

    // get socket
    kmSock& operator()() { return _sck; };

    ///////////////////////////////////////////////
    // inner functions
    
    // create receiving thread
    void CreateRcvThrd()
    {
        _thrd.Begin([](kmUdp* net)
        {
            int ret;

            while(1) { if((ret = net->RcvProc()) < 1) break; }

            // close socket
            net->Close();
        }, this);
    };    

    // close all socket and thread
    virtual void CloseAll()
    {    
        _sck.Close();
        if(_thrd.IsRunning()) _thrd.Wait();
    };
};

/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// kmNet new version since 2021.12.16

// kmNet header flag
union kmNetHdFlg
{
    uchar uc; struct
    {
        uchar reqack   : 1;  // request for acknoledgement
        uchar reject   : 1;  // reject request
        uchar rsv2     : 1;
        uchar rsv3     : 1;
        uchar rsv4     : 1;
        uchar rsv5     : 1;
        uchar rsv6     : 1;
        uchar rsv7     : 1;
    };
    kmNetHdFlg()          { uc = 0;   };
    kmNetHdFlg(uchar flg) { uc = flg; };

    operator uchar() { return  uc; };
};

// kmNet header (8byte)
//  { src_id, des_id, ptc_id, cmd_id, opt, flg }
class kmNetHd
{
public:
    ushort      src_id = 0xffff;   // source ID as the sender's point of view
    ushort      des_id = 0xffff;   // destination ID as the sender's point of view
    uchar       ptc_id = 0;        // protocol ID
     char       cmd_id = 0;        // command ID... minus means sending
    uchar       opt    = 0;        // option
    kmNetHdFlg  flg    = 0;        // flag

    // get flag
    inline bool IsReqAck() { return flg.reqack; };
    inline bool IsReject() { return flg.reject; };

    // set flag
    inline void SetReqAck() { flg.reqack = 1; };
    inline void SetReject() { flg.reject = 1; };
};

// kmNet data buffer
class kmNetBuf : public kmMat1i8
{
public:
    // constructor
    kmNetBuf() {};
    kmNetBuf(           int64 size) { Create(     0, size); };
    kmNetBuf(char* ptr, int64 size) { Set   (ptr, 0, size); };

    // set buf as starting position (in front of hd)
    void SetPos0() { SetN1(sizeof(kmNetHd)); };

    // move the current position ptr as step
    void MoveCurPtr(int stp_byte) { IncN1(stp_byte); };

    // get the current position ptr 
    char* GetCurPtr() const { return End1(); };

    // get header
    inline kmNetHd& GetHd() const { return *(kmNetHd*)P(); };

    //////////////////////////////////////////
    // buf control operator

    // operator to get data
    template<typename T> kmNetBuf& operator>>(T& data) { GetData(data); return *this; };

    // operator to put data
    template<typename T> kmNetBuf& operator<<(      T& data) { PutData(data); return *this; };
    template<typename T> kmNetBuf& operator<<(const T& data) { PutData(data); return *this; };

    // operator to set header
    kmNetBuf& operator>>(      kmNetHd& hd) { hd      = GetHd(); SetPos0(); return *this; };
    kmNetBuf& operator<<(      kmNetHd& hd) { GetHd() = hd;      SetPos0(); return *this; };    
    kmNetBuf& operator<<(const kmNetHd& hd) { GetHd() = hd;      SetPos0(); return *this; };

    //////////////////////////////////////////
    // get data from buffer

    // get n size char without reading n
    void GetDataOnly(char* data, ushort n) { memcpy(data, End1(), n); IncN1(n); };

    // get n size data without reading n
    template<typename T> void GetDataOnly(T* data, ushort n)
    {
        T* ptr = (T*)End1();

        for(ushort i = 0; i < n; ++i) *(data + i) = ntoh(*(ptr++));
        IncN1(sizeof(T)*n);
    }

    // get one size data
    template<typename T> void GetData(T& data)
    {
        data = ntoh(*((T*)End1())); IncN1(sizeof(T));
    }

    // get n size data
    template<typename T> void GetData(T* data, ushort& n)
    {
        GetData(n); GetDataOnly(data, n);
    }

    // get n size char
    void GetData( char* data, ushort& n) { GetData(n); GetDataOnly(data, n); };
    void GetData(wchar* data, ushort& n) { GetData(n); GetDataOnly(data, n); };
    
    // get kmStr or kmMat1
    template<typename T> void GetData(kmStr <T>& data) { ushort n; GetData(n); data.Recreate(n); GetDataOnly(data.P(), n); }
    template<typename T> void GetData(kmMat1<T>& data) { ushort n; GetData(n); data.Recreate(n); GetDataOnly(data.P(), n); }

    //////////////////////////////////////////
    // put data into buffer

    // put n size char without inserting n
    void PutDataOnly(const char* data, ushort n) { memcpy(End1(), data, n); IncN1(n); };

    // put n size data without inserting n
    template<typename T> void PutDataOnly(const T* data, ushort n)
    {
        T* ptr = (T*)End1();

        for(ushort i = 0; i < n; ++i) *(ptr++) = hton(*(data + i));
        IncN1(sizeof(T)*n);
    }

    // put one size data
    template<typename T> void PutData(const T& data)
    {
        *((T*)End1()) = hton(data); IncN1(sizeof(T));
    }

    // put n size data
    template<typename T> void PutData(const T* data, ushort n)
    {
        PutData((ushort) n); PutDataOnly(data, n);
    }

    // put n size char
    void PutData(const char* data, ushort n) { PutData(n); PutDataOnly(data, n); };

    // put kmStr or kmMat1
    template<typename T> void PutData(const kmStr <T>& data) { PutData(data.P(), (ushort)data.GetLen()); }
    template<typename T> void PutData(const kmMat1<T>& data) { PutData(data.P(), (ushort)data.N1()    ); }
};

// kmNet's time-out monitoring class
class kmNetTom
{
protected:
    double  _tout_sec = 2;
    kmTimer _timer;
    kmLock  _lck;

public:
    void Set(double tout_sec) { _tout_sec = tout_sec; };

    double GetToutSec() { return _tout_sec; };

    void On()  { kmLockGuard grd = _lck.Lock(); _timer.Start(); };
    void Off() { kmLockGuard grd = _lck.Lock(); _timer.Stop (); };

    bool IsOut() { kmLockGuard grd = _lck.Enter(); return _timer.IsStarted() && (_timer.sec() > _tout_sec); };
    bool IsOff() { kmLockGuard grd = _lck.Enter(); return _timer.IsNotStarted(); };
    bool IsOn () { kmLockGuard grd = _lck.Enter(); return _timer.IsStarted(); };
};

// kmNetBase class for protocol
class kmNetBase
{
private:
    kmSock    _sck;        // * Note that it is private to prevent 
                           // * directly using _sck without lock or unlock
public:    
    kmMacAddr _mac;        // mac address
    kmAddr4   _addr;       // source address (private)
    kmNetBuf  _rcv_buf{};  // receiving buffer including kmNetHd
    kmNetBuf  _snd_buf{};  // sending buffer including kmNetHd
    kmStrw    _name{};     // net user's name
    kmLock    _lck;        // mutex for _snd_buf
    kmNetTom  _tom;        // rcv timeout monitoring

    int Bind()
    {
        // * Note that INADDR_ANY will bind to every available ip addr.
        kmAddr4 bind_addr(htonl(INADDR_ANY), _addr.GetPort());

        return _sck.Bind(bind_addr, kmSockType::udp);
    };
    int Close() { return _sck.Close();  };

    int Recvfrom(      kmAddr4& addr) { return _sck.Recvfrom(_rcv_buf, addr); };
    int Sendto  (const kmAddr4& addr)
    {
        const int ret = _sck.Sendto(_snd_buf, addr); 
        UnlockSnd();  return ret; ///////////////////////////// unlock
    };

    template<typename T> kmNetBuf& operator<<(T& data) { return _snd_buf << data; }
    template<typename T> kmNetBuf& operator>>(T& data) { return _rcv_buf >> data; }

    kmNetBuf& operator<<(kmNetHd& hd)
    {
        LockSnd();  return _snd_buf << hd; /////////////////////////// lock
    };

    // thread lock functions
    kmLock* LockSnd  () { return _lck.Lock  (); };
    void    UnlockSnd() {        _lck.Unlock(); };
    kmLock& EnterSnd () { return _lck.Enter (); };
    void    LeaveSnd () {        _lck.Leave (); };

    // timeout monitoring functions
    inline void SetTom(float tout_sec) { _tom.Set(tout_sec); };
    inline void SetTomOn ()            { _tom.On (); };
    inline void SetTomOff()            { _tom.Off(); };
    inline bool IsTomOn ()             { return _tom.IsOn (); };
    inline bool IsTomOff()             { return _tom.IsOff(); };
    inline bool IsTomOut()             { return _tom.IsOut(); };

    // only for debug
    kmSock& GetSock() { return _sck; };
};

/////////////////////////////////////////////////////////////////////////////////////////
// kmNet protocol class 

// typedef for protocol callback
using kmNetPtcCb = int(*)(kmNetBase* net, char cmd_id, void* arg);

// base class of kmNet protocol
class kmNetPtc
{
public:
    uchar      _ptc_id;    // own ptorcol id    
    kmNetPtcCb _ptc_cb;    // protocol callback function
    kmNetBase* _net;

    // init
    kmNetPtc* Init(uchar ptc_id, kmNetBase* net, kmNetPtcCb ptc_cb = nullptr)
    {
        _ptc_id = ptc_id;
        _net    = net;
        _ptc_cb = ptc_cb;

        return this; 
    };

    // receiving procedure    
    virtual void RcvProc(char cmd_id, const kmAddr4& addr) = 0;

    void convWC4to2(wchar* wc, ushort* c, const ushort& n) { for (ushort i = 0; i < n; ++i) { c[i] = (ushort)wc[i]; } }
    void convWC2to4(ushort* c, wchar* wc, const ushort& n) { for (ushort i = 0; i < n; ++i) { wc[i] = (wchar)c[i]; } }
};

// net key type enum class... pkey, vkey, tkey
enum class kmNetKeyType : uchar { invalid = 0, pkey = 1, vkey = 2, tkey = 3 };

// net key class... 8byte
class kmNetKey
{
protected: // * Note that key is saved with big-endian for network transfer
           // * so, you should not directly access this members
    kmNetKeyType _type{};  // key type
    uchar        _svri{};  // server id for the future
    ushort       _idx0{};  // 1st dim's index
    ushort       _idx1{};  // 2nd dim's index
    ushort       _pswd{};  // password

public:
    // constructor
    kmNetKey() {};
    kmNetKey(kmNetKeyType type, ushort idx0, ushort idx1, ushort pswd = 0, uchar svri = 0)
    {
        Set(type, idx0, idx1, pswd, svri); 
    };

    ///////////////////////////////////////
    // member functions
    void Set(kmNetKeyType type, ushort idx0, ushort idx1, ushort pswd = 0, uchar svri = 0)
    {
        _type = type; 
        _svri = svri;
        _idx0 = hton(idx0);
        _idx1 = hton(idx1); 
        _pswd = hton(pswd);
    };
    void SetPkey(ushort idx0, ushort idx1, uint pswd = 0, uchar srvi = 0) { Set(kmNetKeyType::pkey,idx0,idx1,pswd,srvi); };
    void SetVkey(ushort idx0, ushort idx1, uint pswd = 0, uchar srvi = 0) { Set(kmNetKeyType::vkey,idx0,idx1,pswd,srvi); };
    void SetTkey(ushort idx0, ushort idx1, uint pswd = 0, uchar srvi = 0) { Set(kmNetKeyType::tkey,idx0,idx1,pswd,srvi); };

    kmNetKeyType GetType() const { return _type;       };
    uchar        GetSvri() const { return _svri;       };
    ushort       GetIdx0() const { return ntoh(_idx0); };
    uint         GetIdx1() const { return ntoh(_idx1); };
    ushort       GetPswd() const { return ntoh(_pswd); };

    bool IsValid() const { return _type != kmNetKeyType::invalid; };

    const char* GetTypeStr()
    {
        switch(_type)
        {
        case kmNetKeyType::invalid : return "invalid";
        case kmNetKeyType::pkey    : return "pkey";
        case kmNetKeyType::vkey    : return "vkey";
        case kmNetKeyType::tkey    : return "tkey";
        }
        return "";
    };
    kmStra GetStr()
    {
        return kmStra("[%s] %d %d %d %d", 
                      GetTypeStr(), GetIdx0(), GetIdx1(), GetPswd(), GetSvri());
    };
};

// net key renewal algorithm class
class kmNetKeyRnw
{
public:
    kmThread _thrd;
    uint     _rnw_sec     = 30;         // renewal time in sec
    uint     _rnw_min_sec = 10;
    uint     _rnw_max_sec = (60*60*24); // 24 hour

    void Start(kmNetKey* key)
    {
        _thrd.Begin([](kmNetKeyRnw* rnw, kmNetKey* key)
        {
            kmTimer timer(1);

            while(1)
            {
            }
        }, this, key);
    };
};

// net key element class for nks server... 32 byte
class kmNetKeyElm
{
public:
    kmNetKey  key {};  // key
    kmMacAddr mac {};  // mac of key owner
    kmDate    date{};  // data when addr was updated
    kmAddr4   addr{};  // ip address

    // constructor
    kmNetKeyElm() {};
    kmNetKeyElm(kmAddr4State state) { addr.state = state; };
    kmNetKeyElm(kmNetKey key, kmMacAddr mac, kmDate date, kmAddr4 addr) :
        key(key), mac(mac), date(date), addr(addr) {};

    // member functions
    bool IsValid  () { return addr.isValid  (); };
    bool IsPending() { return addr.IsPending(); };
    bool IsInvalid() { return addr.IsInvalid(); };

    kmStra GetStr()
    {
        return kmStra("%s %s %s %s", key .GetStr().P(), mac .GetStr().P(), 
                                     date.GetStrPt().P(), addr.GetStr().P()); 
    };
};
typedef kmMat1<kmNetKeyElm> kmNetKeyElms;

// net key signaling function class nks
class kmNetNks
{
    kmMat1<kmNetKeyElms> _tbl; // (idx0)(idx1)    

public:
    // create table
    void Create(int idx0_n = 32)
    {
        _tbl.Create(idx0_n);

        for(int i = 0; i < idx0_n; ++i) _tbl(i).Create(0, 32);
    };

    // register new one as pkey
    kmNetKey Register(kmMacAddr mac, kmAddr4 addr)
    {
        kmNetKey pkey;
        if (checkAlreadyPkey(mac, pkey)) return pkey;

        // find empty key... idx0, idx1
        ushort idx0 = kmfrand(0, (int)_tbl.N1() - 1), idx1 = 0;

        kmNetKeyElms& elms  = _tbl(idx0);
        const int     elm_n = (int)elms.N1();

        for(; idx1 < elm_n; ++idx1) if(elms(idx1).IsInvalid()) break;

        // set key element
        kmNetKey key(kmNetKeyType::pkey, idx0, idx1, kmfrand(0u, 65535u));
        kmDate   date(time(NULL));

        // add key element to table
        if(idx1 < elm_n) elms(idx1) =  kmNetKeyElm(key, mac, date, addr);
        else             elms.PushBack(kmNetKeyElm(key, mac, date, addr));

        return key;
    };

    // find net key element with key
    kmNetKeyElm& Find(kmNetKey key, kmMacAddr mac)
    {
        const auto idx0 = key.GetIdx0();
        const auto idx1 = key.GetIdx1();

        static kmNetKeyElm keyelm_invalid(kmAddr4State::invalid);

        // check index range
        if(idx0 >=  _tbl.N1() || idx1 >= _tbl(idx0).N1())
        {
            return keyelm_invalid;
        }
        // check pswd
        if(_tbl(idx0)(idx1).key.GetPswd() != key.GetPswd())
        {
            return keyelm_invalid;
        }
        // check mac
        if(_tbl(idx0)(idx1).mac != mac)
        {
            return keyelm_invalid;
        }
        return _tbl(idx0)(idx1);
    };

    // find address with key
    inline kmAddr4 GetAddr(kmNetKey key, kmMacAddr mac)
    {
        return Find(key, mac).addr;
    };

    // update address
    //   return : -1 (invalid key), 0 (not updated), 1 (updated since addr was changed)
    int Update(kmNetKey key, kmMacAddr mac, kmAddr4 addr)
    {
        // find key element
        kmNetKeyElm& ke = Find(key, mac);

        if(ke.IsInvalid()) return -1;

        // check if address is changed
        if(addr == ke.addr) return 0;

        // update address
        ke.addr = addr;
        ke.date.SetCur();

        return 1;
    };

    bool checkAlreadyPkey(const kmMacAddr mac, kmNetKey& pkey)
    {
        for(int64 i = 0; i < _tbl.N1(); ++i)
        {
            const kmNetKeyElms& elms = _tbl(i);

            for(int64 j = 0; j < elms.N1(); ++j)
            {
                if (elms(j).mac == mac)
                {
                    pkey = elms(j).key;
                    return true;
                }
            }
        }
        return false;
    }

    // save key table
    void Save(const wchar* path)
    {
        kmFile file(path, KF_NEW);

        file.WriteMat(&_tbl);
    };

    // load key table
    int Load(const wchar* path) try
    {
        kmFile file(path);

        file.ReadMat(&_tbl);

        return 1;
    }
    catch(kmException) { return 0; };

    // print every key
    void Print()
    {
        print("* idx0_n : %lld \n", _tbl.N1());
        for(int64 i = 0; i < _tbl.N1(); ++i)
        {
            const kmNetKeyElms& elms = _tbl(i);

            for(int64 j = 0; j < elms.N1(); ++j)
            {
                print("   (%lld, %lld) : %s\n", i, j, elms(j).GetStr().P());
            }            
        }
    };
};

// basic protocol for key connection
class kmNetPtcNkey: public kmNetPtc
{
public:
    kmAddr4      _nks_addr;         // nks server's addr

    int          _rcv_key_flg  = 0; // 0 : not received, 1: received key
    int          _rcv_addr_flg = 0; // 0 : not received, 1: received addr

    kmNetKeyType _rcv_keytype;
    kmMacAddr    _rcv_mac;
    kmNetKey     _rcv_key;
    kmAddr4      _rcv_addr;
    uint         _rcv_vld_sec; // valid time in sec
    uint         _rcv_vld_cnt; // valid count
    int          _rcv_sig_flg; // sig flag 0 : addr not changed, 1: change

    kmNetKey     _snd_key;
    kmAddr4      _snd_addr;

    // receiving procedure
    virtual void RcvProc(char cmd_id, const kmAddr4& addr)
    {
        switch(cmd_id)
        {
        case 0: RcvReqKey (addr); break;
        case 1: RcvKey    (addr); break;
        case 2: RcvReqAddr(addr); break;
        case 3: RcvAddr   (addr); break;
        case 4: RcvSig    (addr); break;
        case 5: RcvRepSig (addr); break;
        case 6: RcvReqAccs(addr); break;
        }
    };
    ///////////////////////////////////////
    // interface functions

    // request pkey to nks server (svr -> nks)
    kmNetKey ReqKey(kmNetKeyType keytype, uint vld_sec = 0, uint vld_cnt = 0, float tout_sec = 1.f)
    {
        // send request to key server
        SndReqKey(keytype, vld_sec, vld_cnt);

        // wait for timeout
        kmTimer time(1); _rcv_key_flg = 0;

        while(time.sec() < tout_sec && _rcv_key_flg == 0) std::this_thread::sleep_for(std::chrono::milliseconds(1));

        if(_rcv_key_flg == 0)
        {
            return kmNetKey();
        }        
        return _rcv_key;
    };

    // request addr to nks server by key (clt -> nks)
    kmAddr4 ReqAddr(kmNetKey key, kmMacAddr mac, float tout_sec = 1.f)
    {
        // send request to nks server
        SndReqAddr(key, mac);

        // wait for timeout
        kmTimer time(1); _rcv_addr_flg = 0;

        while(time.sec() < tout_sec && _rcv_addr_flg == 0) std::this_thread::sleep_for(std::chrono::milliseconds(1));

        if(_rcv_addr_flg == 0)
        {
            return kmAddr4(kmAddr4State::invalid);
        }
        return _rcv_addr;
    };

    // send signal to confirm the connection
    //  return : -1 (timeout) or echo time (msec, if received ack)
    float SendSig(kmNetKey key, kmMacAddr mac, float tout_msec = 300.f)
    {
        // rest flag
        _rcv_sig_flg = -1;

        // send signal
        SndSig(key, mac);

        // wait for timeout
        kmTimer time(1);

        while(time.msec() < tout_msec && _rcv_sig_flg == -1) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); };

        return (_rcv_sig_flg >= 0)? (float)time.msec():-1.f;
    };

    // reply signal (nks -> svr)
    //   flg 0 : addr was not changed, 1: changed
    inline void ReplySig(kmAddr4 addr, int flg) { SndRepSig(addr, flg); };

protected:
    ////////////////////////////////////////
    // cmd 0 : request key (svr -> nks)
    void SndReqKey(kmNetKeyType keytype, uint vld_sec = 0, uint vld_cnt = 0)
    {
        kmNetHd hd = { 0xffff, 0xffff, _ptc_id, 0, (uchar)keytype, 0};

        *_net << hd << _net->_mac << vld_sec << vld_cnt;

        _net->Sendto(_nks_addr);
    };
    void RcvReqKey(const kmAddr4& addr) // nks server
    {
        kmNetHd hd{};

        *_net >> hd >> _rcv_mac >> _rcv_vld_sec >> _rcv_vld_cnt;

        _rcv_keytype = (kmNetKeyType)hd.opt;
        _rcv_addr    = addr;

        // call cb to register mac and send key (_rcv_addr, _rcv_mac -> _snd_key)
        (*_ptc_cb)(_net, 0, 0);

        // send key to svr
        SndKey(addr);
    };

    ////////////////////////////////////////
    // cmd 1 : send key (nks -> svr)
    void SndKey(const kmAddr4& addr) // nks server
    {    
        kmNetHd hd = { 0xffff, 0xffff, _ptc_id, 1, 0, 0};

        *_net << hd << _snd_key;
    
        _net->Sendto(addr);
    };
    void RcvKey(const kmAddr4& addr)
    {    
        kmNetHd hd{};

        *_net >> hd >> _rcv_key;

        _rcv_key_flg = 1;
    };

    ////////////////////////////////////////
    // cmd 2 : request addr by key (clt -> nks)
    void SndReqAddr(kmNetKey key, kmMacAddr mac)
    {
        kmNetHd hd = { 0xffff, 0xffff, _ptc_id, 2, 0, 0};

        *_net << hd << key << mac;

        _net->Sendto(_nks_addr);
    };
    void RcvReqAddr(const kmAddr4& addr) // nks server
    {
        kmNetHd hd{};

        *_net >> hd >> _rcv_key >> _rcv_mac;

        // call cb to get addr from key and send addr (_rcv_key, _rcv_mac -> _snd_addr)
        (*_ptc_cb)(_net, 2, 0);

        // request svr to access clt
        if(_snd_addr.isValid()) SndReqAccs(_snd_addr, addr);

        // send address to clt
        SndAddr(addr);
    };

    ////////////////////////////////////////
    // cmd 3 : send addr (nks -> clt)
    void SndAddr(const kmAddr4& addr) // nks server
    {    
        kmNetHd hd = { 0xffff, 0xffff, _ptc_id, 3, 0, 0};

        *_net << hd << _snd_addr;

        _net->Sendto(addr);
    };
    void RcvAddr(const kmAddr4& addr)
    {    
        kmNetHd hd{};

        *_net >> hd >> _rcv_addr;

        _rcv_addr_flg = 1;
    };

    ////////////////////////////////////////
    // cmd 4 : send signal (svr -> nks)
    void SndSig(kmNetKey key, kmMacAddr mac)
    {
        kmNetHd hd = { 0xffff, 0xffff, _ptc_id, 4, 0, 0};

        *_net << hd << key << mac;

        _net->Sendto(_nks_addr);
    };
    void RcvSig(const kmAddr4& addr) // nks server
    {
        kmNetHd hd{};

        *_net >> hd >> _rcv_key >> _rcv_mac;

        _rcv_addr = addr;

        (*_ptc_cb)(_net, 4, 0);
    };

    ////////////////////////////////////////
    // cmd 5 : reply signal (nks --> svr)
    void SndRepSig(const kmAddr4& addr , int flg) // nks server
    {
        kmNetHd hd = { 0xffff, 0xffff, _ptc_id, 5, 0, 0};

        *_net << hd << flg;

        _net->Sendto(addr);
    };
    void RcvRepSig(const kmAddr4& addr)
    {
        kmNetHd hd{};

        *_net >> hd >> _rcv_sig_flg;

        (*_ptc_cb)(_net, 5, 0);
    };

    ////////////////////////////////////////
    // cmd 6 : request to access the target address (nks --> svr)
    void SndReqAccs(const kmAddr4& addr , const kmAddr4& trg_addr) // nks server
    {
        kmNetHd hd = { 0xffff, 0xffff, _ptc_id, 6, 0, 0};

        *_net << hd << trg_addr;

        _net->Sendto(addr);
    };
    void RcvReqAccs(const kmAddr4& addr)
    {
        kmNetHd hd{}; 

        *_net >> hd >> _rcv_addr;

        (*_ptc_cb)(_net, 6, 0);
    };
};

/////////////////////////////////////////////////////////////////////////////////////////
// kmNet class 

// typedef for net callback
using kmNetCb = int(*)(void* parent, uchar ptc_id, char cmd_id, void* arg);

// network base class (using UDP)
class kmNet : public kmNetBase
{
protected:        
    void*     _parent = nullptr;       // parent's pointer for callback
    kmNetCb   _netcb  = nullptr;       // callback function for parent

    // rcv thread members
    kmThread  _rcv_thrd;               // thread for receiving
    kmThread  _tom_thrd;               // thread for receiving

    // nks members
    kmNetKey    _pkey; // own pkey
    kmNetKey    _vkey; // volatile key
    kmNetKeyRnw _rnw;  // pkey renewal

    // protocol members
    kmMat1<kmNetPtc*> _ptcs;           // protocol array

    kmNetPtcNkey      _ptc_nkey;

public:
    // constructor
    kmNet() {};

    // destructor
    virtual ~kmNet() { Close(); };

    // init
    void Init(void* parent = nullptr, kmNetCb netcb = nullptr, ushort port = DEFAULT_PORT)
    {
        // set parent and callback
        _parent = parent;
        _netcb  = netcb;

        // get address
        if (kmSock::GetIntfAddr(_addr) == false)
            cout<<"Get Local Address Error!!"<<endl;

        // create buffer and header pointer
        // * Note that 64 KB is max size of UDP packet
        _rcv_buf.Recreate(64*1024);
        _snd_buf.Recreate(64*1024);

        // bind
        // bind
        for(int i = 8; i--;)
        if(Bind() == -1)
        {
            if(i == 0) throw KE_NET_ERROR;
            else _addr.SetPort(_addr.GetPort() + 1);
        }
        else break;

        // init and add basic protocols
        _ptcs.Recreate(0,16);

        _ptcs.PushBack(_ptc_nkey.Init(5, this, cbRcvPtcNkeyStt));

        // create threads
        CreateRcvThrd();
        CreateTomThrd();
    };

    ///////////////////////////////////////////////
    // inner functions
protected:
    // create rcv time-out monitoring thread
    void CreateTomThrd()
    {
        _tom_thrd.Begin([](kmNet* net)
        {
            print("* tom thread starts\n");
            kmTimer ext_timer; // timer for extra work

            while(1)
            {
                if(net->IsTomOn()) // check time out for rcvblk of ptc_file
                {
                    if(net->IsTomOut())
                    {
                        print("** receiving failed with timeout (%.2fsec)\n", net->_tom.GetToutSec());
                    }
                    // timer control
                    if(ext_timer.IsStarted()) ext_timer.Stop();
                }
                else
                {
                    // do extra work
                    if(ext_timer.sec() > 10.f) net->DoExtraWork();

                    // timer control
                    if(ext_timer.IsNotStarted()) ext_timer.Start();
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); // because frequent monitoring isn't required
            }
            print("* end of rto thread\n");
        }, this);
        _tom_thrd.WaitStart();
    };

    // virtual function for extra work
    virtual void DoExtraWork() { std::this_thread::sleep_for(std::chrono::milliseconds(100)); };

    // create receiving thread
    void CreateRcvThrd()
    {
        _rcv_thrd.Begin([](kmNet* net)
        {
            print("* rcv thread starts\n");
            int ret;

            while(1) { if((ret = net->RcvProc()) < 1) break; }

            // check error
            if(ret < 0) print("** %s\n", kmSock::GetErrStr().P());

            print("* end of rcv thread\n");
        }, this);
        _rcv_thrd.WaitStart();
    };    

    // receiving procedure... core
    //   [return] 0   : end of receiving process
    //            0 < : the number of bytes received or sent.
    //            0 > : error of the socket
    int RcvProc() 
    {
        // receive data
        kmAddr4 addr; int ret = Recvfrom(addr);

        if(ret <= 0) return ret;

        // get header and protocol id
        const uchar ptc_id = _rcv_buf.GetHd().ptc_id;
        const  char cmd_id = _rcv_buf.GetHd().cmd_id;
        
        if(_ptcs.N1() < 1 || ptc_id != 5) return ret; // check if ptc_id comes within range

        // call receiving procedure of the protocol
        //_ptcs(ptc_id)->RcvProc(cmd_id, addr);
        _ptcs(0)->RcvProc(cmd_id, addr);
        
        return ret;
    };

    ///////////////////////////////////////////////
    // callback functions for ptc

    // callback function for ptc_nkey
    //  cmd_id 0 : reqkey, 1: sndkey, 2: reqaddr, 3: sndaddr
    static int cbRcvPtcNkeyStt(kmNetBase* net, char cmd_id, void* arg)
    {
        return ((kmNet*)net)->cbRcvPtcNkey(cmd_id, arg);
    };
    int cbRcvPtcNkey(char cmd_id, void* arg)
    {
        // call netcb
        vcbRcvPtcNkey(cmd_id);

        return (*_netcb)(_parent, _ptc_nkey._ptc_id, cmd_id, arg);
    };

    ///////////////////////////////////////////////
    // virtual functions for rcv callback

    // virtual callback for ptc_nkey
    //  cmd_id 0 : rcv reqkey, 1 : rcv key, 2 : rcv reqaddr, 3 : rcv addr
    virtual void vcbRcvPtcNkey(char cmd_id) {};

    ///////////////////////////////////////////////
    // interface functions

public:
    // get pkey
    kmNetKey& GetPkey() { return _pkey; };

    // set address for nks server
    void SetNksAddr(kmAddr4 nks_addr)
    {
        _ptc_nkey._nks_addr = nks_addr;
    };
    // get address for nks server
    kmAddr4 GetNksAddr() { return _ptc_nkey._nks_addr; };

    ///////////////////////////////////////////////
    // interface functions for communication

    kmStrw getHostName()
    {
        char host[100] = {0,};
        gethostname(host, sizeof(host));
        wchar whost[100] = {0,};
        mbstowcs(whost, host, strlen(host));
        kmStrw ret;
        ret.SetStr(whost);
        return ret;
    }

    // request pkey
    //   return : 0 (failed), 1 (successful)
    int RequestPkey()
    {
        // get key from keysvr
        _pkey = _ptc_nkey.ReqKey(kmNetKeyType::pkey);

        if(_pkey.IsValid() == false)
        {
            return 0;
        }
        return 1;
    };

    // request vkey
    //  vld_sec : valid time in sec
    //  vld_cnt : valid count
    // 
    //   return : 0 (failed), 1 (successful)
    int RequestVkey(uint vld_sec = 3600, uint vld_cnt = 1)
    {
        // get key from keysvr
        _vkey = _ptc_nkey.ReqKey(kmNetKeyType::vkey, vld_sec, vld_cnt);

        if(_vkey.IsValid() == false) return 0;
        return 1;
    };

    // request address to nks (clt -> nks)
    //   return : addr of key (if successful) or invalid addr (if it fails)
    kmAddr4 RequestAddr(kmNetKey key, kmMacAddr mac)
    {
        return _ptc_nkey.ReqAddr(key, mac);
    };

    // send sig to nks (svr -> nks)
    //  return : -1 (timeout) or echo time (msec, if received ack)
    float SendNksSig()
    {
        return _ptc_nkey.SendSig(_pkey, _mac);
    };

    // reply nks signal (nks -> svr)
    //   flg 0 : addr was not changed, 1: changed
    void ReplyNksSig(kmAddr4 addr, int flg)
    {
        _ptc_nkey.ReplySig(addr, flg);
    };
};

#endif /* __km7Net_H_INCLUDED_2021_05_31__ */
