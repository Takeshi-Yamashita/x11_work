## X11開発

UbuntuのデスクトップマシンをVirtualBox上に構築する(以下参考)

開発用ライブラリのインストール

```bash
> sudo apt install libx11-dev
> sudo apt install build-essential
```


[参考サイト](https://qiita.com/tomson784/items/02d8c6e282aa865b1a87)
[参考サイト](http://www.yam-web.net/c-programing/x11/index.html)

最小限のビルドに必要なMakefile

```make
# Makefile for test
CC = gcc
CFLAGS = -O2 -I/usr/include
LIB = -L/usr/lib/x86_64-linux-gnu -lX11 -lm
TARGETS = xlib # ビルドしたいターゲットを指定する(例. xlibなど)
SRC = $(TARGETS).c

$(TARGETS): $(SRC)
    $(CC) -o $@ $^ $(CFLAGS) $(LIB)

clean:
    rm -f *.o $(TARGETS)
```


