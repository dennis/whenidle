CFLAGS=-g
LDLIBS=-L/usr/X11R6/lib -lX11 -lXext -lXss

all: whenidle

clean:
	-rm -f whenidle
