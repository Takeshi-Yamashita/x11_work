#include "zu.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

int main(void) {
  Display *d;
  Window wr;
  GC gc;
  XImage im;
  XColor col[8][8][8];
  Colormap cmap;
  XEvent event;
  int l, m, n;

  d = XOpenDisplay(NULL);
  cmap = DefaultColormap(d, 0);
  for (l = 0; l < 8; l++) {
    for (m = 0; m < 8; m++) {
      for (n = 0; n < 8; n++) {
        col[l][m][n].red = 65535 - (7 - l) * 65535 / 7;
        col[l][m][n].green = 65535 - (7 - m) * 65535 / 7;
        col[l][m][n].blue = 65535 - (7 - n) * 65535 / 7;
        XAllocColor(d, cmap, &col[l][m][n]);
      }
    }
  }

  wr = XCreateSimpleWindow(d, RootWindow(d, 0), 100, 50, 640, 400, 1,
                           col[0][0][0].pixel, col[7][7][7].pixel);
  XStoreName(d, wr, "show bitmap");
  XSelectInput(d, wr, ExposureMask);
  XMapWindow(d, wr);
  gc = XCreateGC(d, wr, 0, 0);

  XSetForeground(d, gc, col[0][7][0].pixel);
  XSetBackground(d, gc, col[7][0][0].pixel);

  im.format = XYBitmap;
  im.width = 640;
  im.height = 400;
  im.xoffset = 0;
  im.byte_order = LSBFirst;
  im.bitmap_unit = 8;
  im.bitmap_bit_order = LSBFirst;
  im.bitmap_pad = 8;
  im.depth = 1;
  im.bytes_per_line = (640 + 7) >> 3;
  im.data = (char *)zu_bits; //読み込み

  while (1) {
    XNextEvent(d, &event);
    switch (event.type) {
    case Expose:
      XRaiseWindow(d, wr);
      XPutImage(d, wr, gc, &im, 0, 0, 0, 0, 640, 400);

      XFlush(d);
    }
  }
}
