FILE = kernel
NEWFILE = libkernel
CC = gcc
CFLAGS = -g -std=c99 -shared -O3 -Wall -Wvla
DFLAGS = -g -std=c99 -fPIC -shared -Wall -Wvla -fsanitize=address,undefined

$(NEWFILE): $(FILE).c
	$(CC) $(CFLAGS) -o $@.so $^

debug: $(FILE).c
	$(CC) $(DFLAGS) -o $(NEWFILE).so $^

clean:
	rm -rf $(NEWFILE) *.so *.a *.dylib *.dSYM
