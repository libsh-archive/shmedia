VERSION = 0.0.20040211
DISTDIR = shmedia-$(VERSION)

DISTFILES = README brdfs envmaps textures objs mats bumpmaps

all: dist

dist:
	rm -rf $(DISTDIR)
	mkdir $(DISTDIR)
	cp -a $(DISTFILES) $(DISTDIR)
	rm -rf `find $(DISTDIR) -name CVS -o -name ".#*" -o -name "*~"`
	rm -f $(DISTDIR).tar.gz
	tar cf $(DISTDIR).tar $(DISTDIR)
	bzip2 -k $(DISTDIR).tar
	gzip $(DISTDIR).tar
	rm -rf $(DISTDIR)

clean:
	rm -f *.tar.gz
	rm -rf shmedia-*

