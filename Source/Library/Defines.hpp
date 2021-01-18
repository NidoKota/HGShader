#define WINDOW_SIZE 300  //ウィンドウの縦横の大きさ       デフォルトは300
#define MIN_PIX_COUNT 25 //画面に表示するピクセルの最低数  デフォルトは25
#define COLOR_COUNT 256  //色数削減するが5fpsぐらい落ちる デフォルトは256
#define TARGET_FPS 60    //目標とするFPS数の値          デフォルトは60

extern "C" { 
    #include <handy.h>
}