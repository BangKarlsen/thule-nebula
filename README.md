**WORK IN PROGRESS**

* I STOLE (almost) EVERYTHING FROM dodke! See https://github.com/itslauri/amiga-startup
for the original repository. Thanks for providing such comfy starting grounds.

* Features a track from b3nny. He doesnt know yet.

* The idea is to mix asm and c.

* It compiles with vbcc/vasm. I tried getting more fancy gcc12 to work but had
  weird linker issues. For now vbcc is good enough and nice and simple.

* I cant get the Makefile to work on my machine. Some $PATH issue..

* To compile do: `vc -v -cpu=68020 -lamiga -ldebug -c99 demo.s main.c -o tuna`

* Notice the `-ldebug` above. That allows using `kprintf` to log to serial
  port for debugging. Change `vc.config` so vlink can find `debug.lib` in the
  NDK, like `-L$NDK_LIB`. My `vc.config` looks like this:

```
-cc=vbccm68k -quiet %s -o= %s %s -O=%ld -I$VBCC/targets/m68k-amigaos/include -I$NDK_INC
-ccv=vbccm68k %s -o= %s %s -O=%ld -I$VBCC/targets/m68k-amigaos/include -I$NDK_INC
-as=vasmm68k_mot -quiet -m68020 -Fhunk -phxass -I$NDK_INC_I %s -o %s
-asv=vasmm68k_mot -Fhunk -phxass -I$NDK_INC_I %s -o %s
-rm=rm -f %s
-rmv=rm %s
-ld=vlink -bamigahunk -x -Bstatic -Cvbcc -nostdlib $VBCC/targets/m68k-amigaos/lib/startup.o %s %s -L$VBCC/targets/m68k-amigaos/lib -L$NDK_LIB -lvc -o %s
-l2=vlink -bamigahunk -x -Bstatic -Cvbcc -nostdlib %s %s -L$VBCC/targets/m68k-amigaos/lib -o %s
-ldv=vlink -bamigahunk -t -x -Bstatic -Cvbcc -nostdlib $VBCC/targets/m68k-amigaos/lib/startup.o %s %s -L$VBCC/targets/m68k-amigaos/lib -lvc -o %s
-l2v=vlink -bamigahunk -t -x -Bstatic -Cvbcc -nostdlib %s %s -L$VBCC/targets/m68k-amigaos/lib -o %s
-ldnodb=-s -Rshort
-ul=-l%s
-cf=-F%s
-ml=1000
```

To get the serial log, first configure a serial port in FS-UAE:
```
serial_port = /tmp/vser
```

Then, in a new terminal, start cat'ing the serial output:
```
socat pty,raw,echo=0,link=/tmp/vser -,raw,echo=0
```

(`brew install socat` if you don't have that already)

* Debugging from FS-UAE:
    - Add `console_debugger = 1` to your config
    - Run FS-UAE from terminal with the config specified: `/Applications/FS-UAE\ Launcher.app/Contents/MacOS/fs-uae-launcher ~/FS-UAE/Configurations/faaret\ A1200.fs-uae`
    - Press F11+d when demo is running to activate debugger
    - `?` for help. `g` to continue


  --faaret / altskalvæk
