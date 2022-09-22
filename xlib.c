#include <X11/Xlib.h> // Xlibに必要なインクルード
/* #include <X11/Xutil.h> */
#include <stdio.h>

int main(void) {
  Display *dis;             // Display pointer
  Window win;               // Window  ID
  XSetWindowAttributes att; //窓属性の変数
  XEvent ev;                //イベント取り込み変数

  dis = XOpenDisplay(NULL); // Xserverとの接続
  win = XCreateSimpleWindow(dis, RootWindow(dis, 0), 100, 100, 256, 256, 3,
                            WhitePixel(dis, 0), BlackPixel(dis, 0)); //窓の生成

  att.backing_store = WhenMapped; //絵を保存する設定をする
  XChangeWindowAttributes(dis, win, CWBackingStore, &att);

  XMapWindow(dis, win); //窓の表示
  XFlush(dis);          //リクエストの強制送信

  XSelectInput(dis, win, ExposureMask);
  do { //窓が開くの待つループ
    XNextEvent(dis, &ev);
  } while (ev.type != Expose); // Exposeイベントが届くまでここを繰り返す

  // ここまで来たら真っ黒な窓が登場しているはず。
  getchar(); // リターンキーが押されるまで待つ。

  XDestroyWindow(dis, win); //窓の消去
  XCloseDisplay(dis);       // Xserverと断線

  return 0;
}
