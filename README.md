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
  port for debugging. That way we don't lose the output when the machine crashes.
  Change `vc.config` so vlink can find `debug.lib` in the
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

To view the serial log, first configure a serial port in FS-UAE:
```
serial_port = /tmp/vser
```

Install `socat`:
```
brew install socat
```

Then, in a new terminal, start cat'ing the serial output:
```
socat pty,raw,echo=0,link=/tmp/vser -,raw,echo=0
```

* Debugging from FS-UAE:
    - Add `console_debugger = 1` to your config
    - Run FS-UAE from terminal with the config specified: 
    ```
    /Applications/FS-UAE\ Launcher.app/Contents/MacOS/fs-uae-launcher ~/FS-UAE/Configurations/faaret\ A1200.fs-uae
    ```
    - Press F11+d when demo is running to activate debugger
    - `?` for help

My FS-UAE confi looks like this:
```
# FS-UAE configuration saved by FS-UAE Launcher
# Last saved: 2023-09-30 20:02:02

[fs-uae]
accelerator = blizzard-1230-iv
amiga_model = A1200/1230
chip_memory = 2048
console_debugger = 1
floppy_drive_volume = 0
hard_drive_0 = /Users/der-er-en-bjørn-i-min-bil/amiga/dir/System
hard_drive_1 = /Users/der-er-en-bjørn-i-min-bil/amiga/dir/Work
hard_drive_2 = /Users/der-er-en-bjørn-i-min-bil/amiga/kode
hard_drive_3 = /Users/der-er-en-bjørn-i-min-bil/kode/amiga
kickstart_file = /Users/der-er-en-bjørn-i-min-bil/amiga/rom/uaerom/rom/kick310.rom
serial_port = /tmp/vser
zorro_iii_memory = 32768
```

Namaste.
  --faaret / altskalvæk
