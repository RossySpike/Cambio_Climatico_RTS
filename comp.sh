#!/bin/sh
i686-w64-mingw32-g++ main.cpp -o game -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic
WINEDEBUG=-all,err+module WINEPATH=/usr/i686-w64-mingw32/sys-root/mingw/bin/ wine ./game.exe
