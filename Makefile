CC		= gcc
CFLAGS	= -fdiagnostics-color=always -g
target	= modpDES_main

.PHONY : all clean 

all: clean $(target)

$(target): main.c utils.c modpDES.c utils.h des.h tables.h
	$(CC) $(CFLAGS) main.c utils.c modpDES.c -o $(target)

clean:
	rm -f $(target)
