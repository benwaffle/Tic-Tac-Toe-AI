CFLAGS = -std=gnu11 -Wall -Wextra -Werror -g $(shell pkg-config --cflags glib-2.0) -Ofast
LDLIBS = $(shell pkg-config --libs glib-2.0)
ttt: ttt.c
