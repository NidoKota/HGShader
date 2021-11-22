#define WINDOW_SIZE 300  //ウィンドウの縦横の大きさ       デフォルトは300
#define MIN_PIX_COUNT 25 //画面に表示するピクセルの最低数  デフォルトは25
#define TARGET_FPS 60    //目標とするFPS数の値          デフォルトは60

#include "../src/lib/Event.hpp"
#include "../src/lib/ShaderUtility.hpp"

extern "C" 
{ 
    #include <handy.h>
}

#include <rapidjson/document.h>

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <time.h>
#include <map>
#include <float.h>
#include <chrono>
#include <sstream>
#include <vector>
#include <thread>
#include <future>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>