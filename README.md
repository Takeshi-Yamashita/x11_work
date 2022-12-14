## X11開発

UbuntuのデスクトップマシンをVirtualBox上に構築する(以下参考)

開発用ライブラリのインストール

```bash
> sudo apt install libx11-dev
> sudo apt install build-essential
```

# 参考

- [参考サイト](https://qiita.com/tomson784/items/02d8c6e282aa865b1a87)
- [参考サイト](http://www.yam-web.net/c-programing/x11/index.html)

# Usage

1. リポジトリをクローンしてくる

```bash
> mkdir ~/git_work
> cd ~/git_work
> git clone https://github.com/Takeshi-Yamashita/x11_work.git
> cd x11_work
```

2. ビルドしたいターゲットに合わせてMakefileを編集する

> 例.xlib.cをビルドしたいなら
> TARGETS = xlib

```make
# Makefile for test
CC = gcc
CFLAGS = -O2 -I/usr/include
LIB = -L/usr/lib/x86_64-linux-gnu -lX11 -lm # ←LIBは開発環境によって変わるので、ビルドが通らない場合は参照先を変更する
TARGETS = xlib
SRC = $(TARGETS).c

$(TARGETS): $(SRC)
    $(CC) -o $@ $^ $(CFLAGS) $(LIB)

clean:
    rm -f *.o $(TARGETS)
```

3. ビルドを実行

```bash
> make
```

4. GUI環境でバイナリを実行する

```bash
> ./xlib
```

