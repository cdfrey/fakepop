# $Id: Makefile,v 1.6 2004/12/04 18:54:21 pzn Exp $

GLIB_CFLAGS  = $(shell pkg-config --cflags glib-2.0)
GLIB_LDFLAGS = $(shell pkg-config --libs glib-2.0)
CFLAGS       = -D_GNU_SOURCE -Wall -Wextra -Werror -O2 $(GLIB_CFLAGS)
LDFLAGS      = $(GLIB_LDFLAGS)

all: fakepop

clean:
	rm -f *.o *~ fakepop

distclean: clean
	rm -f fakepop install.tgz fakepop-src.tgz

fakepop: fakepop.o header.o id.o msg.o retrieve.o

dirs:
	install -d -o root -g root -m 0755 $(DESTDIR)/etc/fakepop
	install -d -o root -g root -m 0755 $(DESTDIR)/usr/sbin
	install -d -o root -g root -m 0755 $(DESTDIR)/usr/share/man/man8
	install -d -o root -g root -m 0755 $(DESTDIR)/usr/share/doc/fakepop/examples
	install -d -o root -g root -m 0755 $(DESTDIR)/usr/share/doc/fakepop/examples/xinetd

install: all dirs
	install -o root -g root -m 0755 fakepop $(DESTDIR)/usr/sbin/in.fakepop
	set -e; for i in README.*; do \
	  install -o root -g root -m 0644 $$i $(DESTDIR)/usr/share/doc/fakepop/examples; \
	done
	install -o root -g root -m 0644 xinetd/fakepop $(DESTDIR)/usr/share/doc/fakepop/examples/xinetd
	umask 022; echo "Please read /usr/share/doc/fakepop/examples/README.config for configuration instructions." > $(DESTDIR)/etc/fakepop/README
	install -o root -g root -m 0644 COPYRIGHT $(DESTDIR)/usr/share/doc/fakepop/copyright
	install -o root -g root -m 0644 in.fakepop.8 $(DESTDIR)/usr/share/man/man8
	gzip -9 $(DESTDIR)/usr/share/man/man8/in.fakepop.8
	cd $(DESTDIR)/usr/share/man/man8/; ln -s in.fakepop.8.gz fakepop.8.gz

tgz-aux:
	make install DESTDIR=`pwd`/install-tmp
	cd install-tmp; tar -zcvf ../install.tgz *
	rm -rf install-tmp

tgz: all
	rm -rfv install.tgz install-tmp
	mkdir install-tmp
	fakeroot make tgz-aux

src-tgz: distclean
	tar -zcvf fakepop-src.tgz `find . -type f | grep -v CVS | sort`
