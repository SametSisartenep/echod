CC=cc
CFLAGS=-Wall -Wno-missing-braces -Wno-parentheses -Wno-switch -Wno-pointer-to-int-cast -fno-diagnostics-color -I/usr/local/include -ggdb -c -O2
LDFLAGS=-L/usr/local/lib -pthread -lfmt -lutf
O=o

TARG=udp7
OFILES=\
	main.$O\
	util.$O\

HFILES=\
	dat.h\
	fns.h\
	args.h\

.PHONY: all clean
all: $(TARG)

%.$O: %.c
	$(CC) $(CFLAGS) $<

$(OFILES): $(HFILES)

$(TARG): $(OFILES)
	$(CC) -o $@ $(OFILES) $(LDFLAGS)

clean:
	rm $(OFILES) $(TARG)
