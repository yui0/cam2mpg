# ©2017 YUICHIRO NAKADA

CC = clang
CFLAGS = -Wall -Os -lm

PROGRAM = cam2mpg
OBJS = cam2mpg.o

.SUFFIXES: .c .o

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(CFLAGS) $^

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	$(RM) $(PROGRAM) $(OBJS) *.o *.s
