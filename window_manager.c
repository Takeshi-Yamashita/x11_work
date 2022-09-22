#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main(void) {
  Display *d;
  Window wr, w1, w2;
  GC gc, gc1, gc2;
  XEvent event;
  unsigned long black, white;

  d = XOpenDisplay(NULL);

  black = BlackPixel(d, 0);
  white = WhitePixel(d, 0);

  wr = XCreateSimpleWindow(d, RootWindow(d, 0), 100, 50, 800, 530, 1, black,
                           white);
  w1 = XCreateSimpleWindow(d, wr, 10, 10, 780, 400, 1, black, white);
  w2 = XCreateSimpleWindow(d, wr, 10, 410, 780, 80, 1, black, white);

  XSelectInput(d, wr, ExposureMask); // Exposeイベントを設定

  XMapWindow(d, wr);
  XMapSubwindows(d, wr);

  gc = XCreateGC(d, wr, 0, 0);
  gc1 = XCreateGC(d, w1, 0, 0);
  gc2 = XCreateGC(d, w2, 0, 0);

  while (1) {
    XNextEvent(d, &event);
    switch (event.type) {
    case Expose:
      XDrawString(d, wr, gc, 500, 515, "test", 4);
      XFlush(d);

      //……………　　その他のイベント
    }
  }
}
