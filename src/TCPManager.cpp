#include "TCPManager.hpp"

ThAndBuf::~ThAndBuf()
{
    if(th != nullptr) 
    {
        //detachした後に解放しないと怒られる
        th->detach();
        delete th;
        delete buf;
    }
}

TCPManager::TCPManager(int port, int bufSize, OnMessageFunc onMessage, OnChangeConnectFunc onChangeConnect)
{
    this->port = port;
    this->bufSize = bufSize;
    this->onMessage = onMessage;
    this->onChangeConnect = onChangeConnect;
}

void TCPManager::Start()
{
    //親スレッド
    connectThread = std::thread(&TCPManager::Connect, this);
}

void TCPManager::Connect()
{
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_len = sizeof(addr);
 
    sock0 = socket(AF_INET, SOCK_STREAM, 0);
    if (sock0 < 0)
    {
        perror("socket");
        exit(1);
    }
 
    if (bind(sock0, (sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }
 
    int currentId = 0;
    while (true)
    {
        if (listen(sock0, 5) < 0)
        {
            perror("listen");
            exit(1);
        }

        len = sizeof(client);
        sock = accept(sock0, (struct sockaddr *)&client, &len);
        if (sock < 0)
        {
            perror("accept");
            exit(1);
        }

        //mapに追加後にコピーされるのでポインタはmapから取得する
        tabMap.emplace(currentId, ThAndBuf());
        ResetIndex();
        onChangeConnect();
        ThAndBuf* tabP = &tabMap.at(currentId);
        
        //子スレッド生成
        int currentSock = sock;
        tabP->buf = new char[bufSize];
        memset(tabP->buf, 0, bufSize);
        tabP->th = new std::thread([&, tabP, currentSock, currentId]()
        {
            char tmpBuf[bufSize];
            while (true)
            {
                memset(tmpBuf, 0, bufSize);
                if(read(currentSock, tmpBuf, bufSize) <= 0) break;
                
                strcpy(tabP->buf, tmpBuf);
                onMessage(currentId, tabP->index, tabP->buf);
            }
            
            tabMap.erase(currentId);
            ResetIndex();
            onChangeConnect();
        });
        
        currentId++;
    }
}

void TCPManager::ResetIndex()
{
    int i = 0;
    for (auto pair = tabMap.begin(); pair != tabMap.end(); pair++)
    {
        pair->second.index = i;
        i++;
    }
}

int TCPManager::GetConnectCount()
{
    return tabMap.size();
}

int TCPManager::GetPort()
{
    return port;
}

char* TCPManager::GetLastBufByIndex(int index)
{
    int i = 0;
    for (auto pair = tabMap.begin(); pair != tabMap.end(); pair++)
    {
        if(i == pair->second.index) return pair->second.buf;
        i++;
    }
    
    return nullptr;
}

char* TCPManager::GetLastBufByID(int id)
{
    try
    {
        return tabMap.at(id).buf;
    }
    catch(const std::out_of_range&) {}
    
    return nullptr;
}