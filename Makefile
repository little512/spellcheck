.PHONY: all install uninstall

all:
	@g++ src/main.cpp -o spellcheck

install:
	cp spellcheck /usr/bin/spellcheck

uninstall:
	rm /usr/bin/spellcheck
