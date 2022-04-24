#pragma once

#include "../../include/SubInclude.hpp"

struct ThAndBuf
{
public:
    std::thread* th;
    char* buf;
    int index;
    
    ~ThAndBuf();
};

//TCPでメッセージを受け取る(複数接続可)
struct TCPManager : public EventHandler
{
    using OnMessageFunc = void (*)(int, int, char*);
    using OnChangeConnectFunc = void (*)();

private:
    std::map<int, ThAndBuf> tabMap = std::map<int, ThAndBuf>();
    std::thread connectThread;
    sockaddr_in addr;
    sockaddr_in client;
    socklen_t len;
    OnMessageFunc onMessage;
    OnChangeConnectFunc onChangeConnect;
    int port;
    int bufSize;
    int sock0;
    int sock;

    void Connect();
    void ResetIndex();
    
public:
    TCPManager(int port, int bufSize, OnMessageFunc onMessage, OnChangeConnectFunc onChangeConnect);

    //再度初期化処理を行う
    void Start() override;
    int GetConnectCount();
    int GetPort();
    char* GetLastBufByID(int id);
    char* GetLastBufByIndex(int index);
};
