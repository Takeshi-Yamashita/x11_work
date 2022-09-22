#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>

int main(void) {
  Display *d;
  Window wr, w1, w2; // wrを親ウィンド、w1、w2をサブウィンドとする
  GC gc, gc1, gc2;
  XSetWindowAttributes attr;
  unsigned long black, white;

  d = XOpenDisplay(NULL);

  black = BlackPixel(d, 0);
  white = WhitePixel(d, 0);

  wr = XCreateSimpleWindow(d, RootWindow(d, 0), 100, 50, 800, 530, 1, black,
                           white); //親ウィンドの生成
  w1 = XCreateSimpleWindow(d, wr, 10, 10, 780, 400, 1, black,
                           white); //子ウィンドの生成
  w2 = XCreateSimpleWindow(d, wr, 10, 410, 780, 80, 1, black, white);

  attr.override_redirect = True;
  XChangeWindowAttributes(d, wr, CWOverrideRedirect, &attr);

  XMapWindow(d,
             wr); //親ウィンドのマップ（ディスプレイ上に写像して可視状態にする）
  XMapSubwindows(d, wr); //子ウィンド（w1、w2）のマップ

  gc = XCreateGC(
      d, wr, 0,
      0); //各ウィンドに対するＧＣ（このあとここに描画色や描画フォントなどの設定を行っていく）の生成
  gc1 = XCreateGC(d, w1, 0, 0);
  gc2 = XCreateGC(d, w2, 0, 0);

  //……………
  XDrawString(d, wr, gc, 500, 515, "test", 4);
  XFlush(d);
  sleep(10); // 10秒間表示
  return (0);
}
