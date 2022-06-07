#include "stdc++.h"

#include "net_io.h"

#ifdef _WIN32
#include <windows.h>

#pragma comment(lib,"ws2_32.lib") 
typedef int sockaddr_size_t;

#elif __linux__

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

typedef socklen_t sockaddr_size_t;
#endif


static void InitSocketSystem()
{
#ifdef _WIN32
    static bool bInit = false;
    if (!bInit)
    {
        WSADATA Ws;
        if (WSAStartup(0x202, &Ws) != 0)
        {
            printf("Init Windows Socket Failed\r\n");
        }
        bInit = true;
    }
#endif
}

int NetUdpIO::Open()
{
    assert(sock == INVALID_SOCKET_T);
    InitSocketSystem();

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = Param.bRefLocalPort ? htons(Param.LocalPort) : 0;
    address.sin_addr.s_addr = Param.bRefLocalIp ? inet_addr(Param.LocalIp.c_str()) : 0; // INADDR_ANY;

    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("can not bind socket");
        Close();
        return 0;
    }

    int time_out = 500; //ms
#ifdef _WIN32
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&time_out, sizeof(time_out));
#elif __linux__
    struct timeval time;
    time.tv_sec = time_out / 1000;         //set the rcv wait time
    time.tv_usec = time_out % 1000 * 1000; //100000us = 0.1s
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &time, sizeof(time));
#endif
    return 1;
}

int NetUdpIO::Close()
{
    if(sock == INVALID_SOCKET_T)
        return 1;
#ifdef _WIN32
    closesocket(sock);
#elif __linux__
    close(sock);
#endif
    sock = INVALID_SOCKET_T;
    return 1;
}

int NetUdpIO::Read(uint8_t *pData, int DataSize, int *pReadSize)
{
    sockaddr_in source;
    sockaddr_size_t source_size = sizeof(source);
    int Ret = ::recvfrom(sock, (char *)pData, DataSize, 0,
                       (sockaddr *)&source, &source_size);
    if (Ret < 0)
    {
        if (pReadSize)
            *pReadSize = 0;
        return 0;
    }
    else
    {
        if (pReadSize)
            *pReadSize = Ret;
        return 1;
    }
}

int NetUdpIO::Write(const uint8_t *pData, int DataSize, int *pWriteSize)
{
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(Param.RemotePort);
    server_addr.sin_addr.s_addr = inet_addr(Param.RemoteIp.c_str());
    int Ret = ::sendto(sock, (const char *)pData, DataSize, 0,
                     (sockaddr *)&(server_addr), sizeof(server_addr));

    if (Ret < 0)
    {
        if (pWriteSize)
            *pWriteSize = 0;
        return 0;
    }
    else
    {
        if (pWriteSize)
            *pWriteSize = Ret;
        return 1;
    }
}

int NetUdpIO::TransIp(std::string ipstr)
{
    return inet_addr(ipstr.c_str());
}

int NetUdpIO::recvfrom(SOCKET_T sock, void *buf, size_t size)
{
    sockaddr_in source;
    sockaddr_size_t source_size = sizeof(source);
    return ::recvfrom(sock, (char*)buf, size, 0,
                       (sockaddr *)&source, &source_size);
}

int NetUdpIO::sendto(SOCKET_T sock, const void *buf, size_t size, std::string remote_ip, int remote_port)
{
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(remote_port);
    server_addr.sin_addr.s_addr = inet_addr(remote_ip.c_str());
    return ::sendto(sock, (const char*)buf, size, 0,
                     (sockaddr *)&(server_addr), sizeof(server_addr));
}

// tcp
int NetTcpIO::GetSockError()
{
#ifdef _WIN32
    return GetLastError();
#else
    return errno;
#endif
}

bool NetTcpIO::Open()
{
	std::lock_guard<std::mutex> guard(m_OpenAct);
	if (!bOpen)
	{
        InitSocketSystem();
        assert(sock == INVALID_SOCKET_T);
        sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == INVALID_SOCKET_T)
            return 0;

        printf("bind\r\n");
        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_port = Param.bRefLocalPort ? htons(Param.LocalPort) : 0;
        address.sin_addr.s_addr = Param.bRefLocalIp ? inet_addr(Param.LocalIp.c_str()) : 0; // INADDR_ANY;
        if (bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
            printf("can not bind socketr\r\n");
            DoClose();
            return 0;
        }

        if(Param.bNoDelay)
        {
            printf("set tcp_nodelay\r\n");
#ifdef _WIN32
            char opt = 1;
#else
            int opt = 1;
#endif
            if(setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(opt)) < 0)
            {
                printf("set tcp_nodelay failed:%d\r\n", GetSockError());
                DoClose();
                return 0;
            }
        }

        if(SetRecvTimeout() == false)
        {
            DoClose();
            return 0;
        }

        bool ret = false;
        if(Param.bServer)
            ret = RunServer();
        else
            ret = ConnectServer();
        if(ret == false)
        {
            DoClose();
            return 0;
        }
        bOpen = true;
        return 1;
	}
	return 1;
}

bool NetTcpIO::Close()
{
	std::lock_guard<std::mutex> guard(m_OpenAct);
    return DoClose();
}

bool NetTcpIO::DoClose()
{
    bOpen = false;
    if(sock == INVALID_SOCKET_T)
        return 1;
#ifdef _WIN32
    closesocket(sock);
#elif __linux__
    close(sock);
#endif
    sock = INVALID_SOCKET_T;
    WaitServerFinish();
	return 1;
}

void NetTcpIO::WaitServerFinish()
{
    if(Param.bServer)
    { 
        if (listening)
        {
            listening->join();
        }
    }
}

bool NetTcpIO::ConnectServer()
{
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(Param.RemotePort);
    server_addr.sin_addr.s_addr = inet_addr(Param.RemoteIp.c_str());
    return ::connect(sock, (sockaddr *)&(server_addr), sizeof(server_addr)) != -1;
}

bool NetTcpIO::RunServer()
{
    int ret = ::listen(sock, 2);
    if(ret < 0)
        return false;
    std::cout << "listening..." << std::endl;
    auto listenfunc = [=]() {
        while (true)
        {
            sockaddr_in clientaddr;
            sockaddr_size_t clientaddrsize = sizeof(sockaddr_in);
            SOCKET_T newconnect = ::accept(sock, (sockaddr *)&(clientaddr), &clientaddrsize);

            if (newconnect == INVALID_SOCKET_T)
            {
                auto err = this->GetSockError();
                std::cout << "error:" << err << std::endl;
                break;
            }
            std::string clientip = inet_ntoa(clientaddr.sin_addr);
            int clientport = ntohs(clientaddr.sin_port);
            std::cout << clientip << ":" << clientport << " accept"<< std::endl;
            if(Param.ServerFunc)
                Param.ServerFunc([=, &newconnect](char* pbuf, int bufsize) { return ::recv(newconnect, pbuf, bufsize, 0); },
                    [=, &newconnect](const char* pbuf, int bufsize) {return ::send(newconnect, pbuf, bufsize, 0); });
        }
    };
    listening = std::make_unique<std::thread>(listenfunc);
    return true;
}

bool NetTcpIO::SetRecvTimeout()
{
    if(Param.bRefRecvTimeout)
    {
        return SetRecvTimeout(sock, Param.RecvTimeout);
    }
    return 1;
}

bool NetTcpIO::IsErrorTimeout()
{
#ifdef _WIN32
    return GetLastError() == 10060;
#else
    return (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN);
#endif
}

bool NetTcpIO::Read(uint8_t* pData, int DataSize, int* pReadSize)
{
	if (Open())
	{
		int Ret = recv(sock, (char*)pData, DataSize, 0);
		if (Ret < 0)
		{
			if (pReadSize)
				*pReadSize = 0;
			if (!IsErrorTimeout())
			{
				Close();
				bOpen = false;
				return 0;
			}
			return 1;
		}
        else if(Ret == 0)
        {
			if (pReadSize)
				*pReadSize = 0;
			bOpen = false;
			Close();
			return 0;
        }
		else
		{
			if (pReadSize)
				*pReadSize = Ret;
			return 1;
		}
	}
	else
		return 0;
}

bool NetTcpIO::ReadClear()
{
	if (Open())
	{
		SetRecvTimeout(sock, 1);
		std::unique_ptr<uint8_t[]> buf = std::make_unique<uint8_t[]>(0x400);
		while (true)
		{
			int size = 0;
			int ret = Read(buf.get(), 0x400, &size);
			if (size == 0)
				break;
		}
		SetRecvTimeout();
		return 1;
	}
	else
		return 0;
}

bool NetTcpIO::Write(const uint8_t* pData, int DataSize, int* pWriteSize)
{
	if (Open())
	{
		int Ret = ::send(sock, (const char*)pData, DataSize, 0);

		if (Ret < 0)
		{
			if (pWriteSize)
				*pWriteSize = 0;
            if(!IsErrorTimeout())
            {
			    bOpen = false;
			    Close();
			    return 0;
            }
            return true;
		}
        else if(Ret == 0)
        {
			if (pWriteSize)
				*pWriteSize = 0;
			bOpen = false;
			Close();
			return 0;
        }
		else
		{
			if (pWriteSize)
				*pWriteSize = Ret;
			return 1;
		}
	}
	else
		return 0;
}

bool NetTcpIO::SetRecvTimeout(SOCKET_T s, int ms)
{
        int time_out = ms; //ms
    #ifdef _WIN32
        return setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&time_out, sizeof(time_out)) >= 0;
    #elif __linux__
        struct timeval time;
        time.tv_sec = time_out / 1000;         //set the rcv wait time
        time.tv_usec = time_out % 1000 * 1000; //100000us = 0.1s
        return setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &time, sizeof(time)) >= 0;
    #endif
}
