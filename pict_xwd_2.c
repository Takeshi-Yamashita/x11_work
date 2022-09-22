#include <X11/XWDFile.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct struct_imElement {
  XImage *im;
  int width;
  int height;
} imElement;

int main(void) {
  Display *d;
  Window wr;
  GC gc;
  Pixmap pr;
  XEvent event;
  void XWDView(Display * d, char name[], imElement *im);
  imElement im1;

  d = XOpenDisplay(NULL);

  wr = XCreateSimpleWindow(d, RootWindow(d, 0), 0, 0, 800, 600, 0, 0, 0);
  XStoreName(d, wr, "show xwd picture");
  XSelectInput(d, wr, ExposureMask);
  XMapWindow(d, wr);
  gc = XCreateGC(d, wr, 0, 0);
  pr = XCreatePixmap(d, wr, 800, 600, DefaultDepth(d, 0));

  XSetForeground(d, gc, WhitePixel(d, 0));
  XFillRectangle(d, pr, gc, 0, 0, 800, 600);

  XWDView(d, "test.xwd", &im1);
  XPutImage(d, pr, gc, im1.im, 0, 0, 100, 50, im1.width, im1.height);

  while (1) {
    XNextEvent(d, &event);
    switch (event.type) {
    case Expose:
      XCopyArea(d, pr, wr, gc, 0, 0, 800, 600, 0, 0);
      XFlush(d);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void XWDView(Display *d, char name[], imElement *im) {
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

  im->width = fh.window_width;
  im->height = fh.window_height;

  im->im = XCreateImage(d, DefaultVisual(d, 0), DefaultDepth(d, 0), ZPixmap, 0,
                        0, im->width, im->height, 32, 0);

  if (DefaultDepth(d, 0) == 16) {
    for (i = 0; i < im->width * im->height; i++) {
      r0 = 31 * buff[3 * i + 0] / 255;
      r1 = 31 * buff[3 * i + 1] / 255;
      r2 = 31 * buff[3 * i + 2] / 255;
      buff[3 * i + 0] = 0x00;
      buff[3 * i + 1] = r0 << 3 | r1 >> 2;
      buff[3 * i + 2] = (r1 & 0x07) << 6 | r2;
    }
  }

  im->im->data = (char *)buff;
  im->im->xoffset = 0;
  im->im->byte_order = MSBFirst;
  im->im->bitmap_unit = 32;
  im->im->bitmap_bit_order = MSBFirst;
  im->im->bytes_per_line = fh.bytes_per_line;
  im->im->bits_per_pixel = fh.bits_per_pixel;
}

//////////////////////////////////////////////////////////////////////////////////

int change(unsigned int val) {
  val = (val << 24) + (val << 8 & 0xff0000) + (val >> 8 & 0xff00) + (val >> 24);
  return val;
}
