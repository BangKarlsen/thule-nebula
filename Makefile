# Make a demo
#
# Actually, this Makefile does not work on my machine, some $PATH is wrong and vc cannot
# find vasmm68k_mot even though it is right there...
#
# ..it's a mess.
#
# But running the command below in a shell does work!
# 
# vc -v -cpu=68020 -fpu=68882 -lamiga -c99 -o tuna
# 

VBCC = /opt/vbcc
PATH = $VBCC/bin:$PATH
NDK = /Users/hulkhojgaard/amiga/NDK_3.9   # <- that's probably different on your system
NDK_INC = $NDK/Include/include_h
NDK_LIB = $NDK/Include/linker_libs
NDK_INC_I = $NDK/Include/include_i

#OBJ = demo.o
OUT = tuna
CC = vc
CFLAGS = -c99 -cpu=68030 #-fpu=68882
LDFLAGS = -lamiga #-lm881
VASM = vasmm68k_mot
VASMFLAGS = -m68030 #-Felf -opt-fconst -nowarn=62 -dwarf=3 -quiet -x -I. -I$(SDKDIR)
RM = rm -f
OBJ = 	demo.o 	\
	main.o 		\

$(OUT):
	vc -vv -cpu=68020 -lamiga -c99 demo.s main.c -o $(OUT)

#$(OUT): $(OBJ)
#	$(CC) $(OBJ) $(LDFLAGS) -o $@
#
#%.o : %.c,%.s
#	$(CC) $(CFLAGS) -c $<
#
# %.o : %.s
# 	$(VASM) $(VASMFLAGS) $<

clean:
	$(RM) $(OUT) && find . -name "*.o" -type f -delete

