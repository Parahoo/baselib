#pragma once
#include <string>
#include <functional>

struct NetUdpPARAM
{
    int    bRefLocalIp = 0;
    int    bRefLocalPort = 0;
    int    bRefRecvTimeout = 0;

    std::string   LocalIp = "192.168.1.101";
    int    LocalPort = 28020;

    int    RecvTimeout = 100; // ms

    std::string   RemoteIp = "192.168.1.222";
    int    RemotePort = 28020;
};

typedef  uint32_t SOCKET_T;
const int INVALID_SOCKET_T = -1;

class NetUdpIO
{
    NetUdpPARAM    Param;
    SOCKET_T     sock = INVALID_SOCKET_T;
public:
    NetUdpIO() {}
    NetUdpIO(NetUdpPARAM param) { Param = param; }

    void  SetParam(NetUdpPARAM param) { Param = param; }

    SOCKET_T GetSocket(){return sock;}

    int CheckLinkOk() { return 1; }
    int Open();
    int Close();
    int Read(uint8_t *pData, int DataSize, int *pReadSize);
    int Write(const uint8_t *pData, int DataSize, int *pWriteSize);

public:

    static int TransIp(std::string ipstr);

    // recvfrom
    static int recvfrom(SOCKET_T s, void *buf, size_t size);

    // sendto
    static int sendto(SOCKET_T s, const void *buf, size_t size, std::string remote_ip, int remote_port);
};

// void(readfunc, writefunc);
typedef std::function<void(std::function<int(char*, int)>, std::function<int(const char*, int)>)> TcpSerFunc;

struct NetTcpPARAM
{
    int    bServer = 0;
    int    bRefLocalIp = 0;
    int    bRefLocalPort = 0;
    int    bRefRecvTimeout = 0;
    int    bNoDelay = 0;

    std::string   LocalIp = "192.168.183.2";
    int    LocalPort = 0;

    std::string   RemoteIp = "192.168.183.1";
    int    RemotePort = 1947;

    int    RecvTimeout = 100; // ms

    TcpSerFunc ServerFunc;
};

class NetTcpIO
{
    NetTcpPARAM    Param;
    SOCKET_T     sock = INVALID_SOCKET_T;
    bool           bOpen = false;
    int            Sock_Error = 0;
    std::mutex     m_OpenAct;
    std::unique_ptr<std::thread> listening;
protected:
    bool RunServer();
    bool ConnectServer();
    bool SetRecvTimeout();
    bool DoClose();
public:
    NetTcpIO() {}
    NetTcpIO(NetTcpPARAM param) { Param = param; }

    void  SetParam(NetTcpPARAM param) { Param = param; }

    static int  GetSockError();
    static bool IsErrorTimeout();
    bool CheckLinkOk() { return bOpen; }
    bool Open();
    bool Close();
    bool Read(uint8_t* pData, int DataSize, int* pReadSize);
    bool Write(const uint8_t* pData, int DataSize, int* pWriteSize);
    bool ReadClear();

    void WaitServerFinish();

    static bool SetRecvTimeout(SOCKET_T s, int ms);
};
