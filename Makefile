# ©2017 YUICHIRO NAKADA

CC = clang
CFLAGS = -Wall -Os
LDFLAGS = -lm

PROGRAM = cam2mpg
OBJS = cam2mpg.o

.SUFFIXES: .c .o

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(CFLAGS) $(LDFLAGS) $^

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	$(RM) $(PROGRAM) $(OBJS) *.o *.s
