#include <X11/XWDFile.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>

Display *d;
XImage *im;
int width1, height1;

int main(void) {
  Window wr, w1;
  GC gc, gc1;
  Pixmap pr, p1;
  XEvent event;
  void XWDView(char name[]);

  d = XOpenDisplay(NULL);

  wr = XCreateSimpleWindow(d, RootWindow(d, 0), 100, 50, 800, 600, 0, 0, 0);
  XStoreName(d, wr, "show xwd picture");
  XSelectInput(d, wr, ExposureMask);
  XWDView("test.xwd");
  w1 = XCreateSimpleWindow(d, wr, 10, 10, width1, height1, 0, 1, 0);
  XMapWindow(d, wr);
  XMapSubwindows(d, wr);
  gc = XCreateGC(d, wr, 0, 0);
  gc1 = XCreateGC(d, w1, 0, 0);
  pr = XCreatePixmap(d, wr, 800, 600, DefaultDepth(d, 0));
  p1 = XCreatePixmap(d, w1, width1, height1, DefaultDepth(d, 0));

  XSetForeground(d, gc, WhitePixel(d, 0));
  XFillRectangle(d, pr, gc, 0, 0, 800, 600);
  XPutImage(d, p1, gc1, im, 0, 0, 0, 0, width1, height1);

  while (1) {
    XNextEvent(d, &event);
    switch (event.type) {
    case Expose:
      XCopyArea(d, pr, wr, gc, 0, 0, 800, 600, 0, 0);
      XCopyArea(d, p1, w1, gc1, 0, 0, width1, height1, 0, 0);
      XFlush(d);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void XWDView(char name[]) {
  XWDFileHeader fh;
  int change(unsigned int val);

  FILE *fp;
  unsigned char buff[3 * 800 * 600];
  int i, flg;
  int r0, r1, r2;

  if ((fp = fopen(name, "r")) == NULL) {
    printf("can not open file!");
    exit(0);
  }
  flg = fread(&fh, sizeof(fh), 1, fp);
  fh.window_width = change(fh.window_width);
  fh.window_height = change(fh.window_height);
  fh.bits_per_pixel = change(fh.bits_per_pixel);
  fh.bytes_per_line = change(fh.bytes_per_line);
  flg = fread(buff, fh.bits_per_pixel / 8, fh.window_width * fh.window_height,
              fp);
  fclose(fp);

  width1 = fh.window_width;
  height1 = fh.window_height;

  im = XCreateImage(d, DefaultVisual(d, 0), DefaultDepth(d, 0), ZPixmap, 0, 0,
                    width1, height1, 32, 0);

  if (DefaultDepth(d, 0) == 16) {
    for (i = 0; i < width1 * height1; i++) {
      r0 = 31 * buff[3 * i + 0] / 255;
      r1 = 31 * buff[3 * i + 1] / 255;
      r2 = 31 * buff[3 * i + 2] / 255;
      buff[3 * i + 0] = 0x00;
      buff[3 * i + 1] = r0 << 3 | r1 >> 2;
      buff[3 * i + 2] = (r1 & 0x07) << 6 | r2;
    }
  }

  im->data = (char *)buff;
  im->xoffset = 0;
  im->byte_order = MSBFirst;
  im->bitmap_unit = 32;
  im->bitmap_bit_order = MSBFirst;
  im->bytes_per_line = fh.bytes_per_line;
  im->bits_per_pixel = fh.bits_per_pixel;
}

//////////////////////////////////////////////////////////////////////////////////

int change(unsigned int val) {
  val = (val << 24) + (val << 8 & 0xff0000) + (val >> 8 & 0xff00) + (val >> 24);
  return val;
}
