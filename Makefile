VERSION = 0.0.20040322
DISTDIR = shmedia-$(VERSION)

DISTFILES = README brdfs envmaps textures objs mats bumpmaps

SUBDIRS = textures

all: recurse-all

recurse-all:
	for i in $(SUBDIRS) ; do cd $$i && $(MAKE) && cd .. ; done

dist:
	rm -rf $(DISTDIR)
	mkdir $(DISTDIR)
	cp -a $(DISTFILES) $(DISTDIR)
	rm -rf `find $(DISTDIR) -name CVS -o -name ".#*" -o -name "*~" -o -name nodist -o -name dist`
	rm -f $(DISTDIR).tar.gz
	tar cf $(DISTDIR).tar $(DISTDIR)
	bzip2 -k $(DISTDIR).tar
	gzip $(DISTDIR).tar
	rm -rf $(DISTDIR)

clean:
	rm -f *.tar.gz
	rm -rf shmedia-*

