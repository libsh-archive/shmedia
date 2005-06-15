VERSION = 0.7.8
DISTDIR = shmedia
DISTARCHIVE = shmedia-$(VERSION)

DISTFILES = Makefile default.mk ChangeLog README brdfs envmaps horizonmaps textures objs mats bumpmaps

SUBDIRS = textures

all: recurse-all

recurse-all:
	for i in $(SUBDIRS) ; do cd $$i && $(MAKE) && cd .. ; done

dist:   clean
	rm -rf $(DISTDIR)
	mkdir $(DISTDIR)
	cp -dpR $(DISTFILES) $(DISTDIR)
	rm -rf `find $(DISTDIR) -name ".depends" -o -name .done -o -name .svn -o -name CVS -o -name ".#*" -o -name "*~" -o -name nodist`
	cd $(DISTDIR) ; $(MAKE) all ; cd ..
	rm -rf `find $(DISTDIR) -name ".depends" -o -name .done -o -name .svn -o -name CVS -o -name ".#*" -o -name "*~" -o -name nodist -o -name dist`
	rm -f $(DISTARCHIVE).tar.gz
	tar cf $(DISTARCHIVE).tar $(DISTDIR)
	gzip $(DISTARCHIVE).tar
	rm -f $(DISTARCHIVE).zip
	zip -rq $(DISTARCHIVE).zip $(DISTDIR)
	rm -rf $(DISTDIR)

clean:
	rm -f *.tar.gz *.zip
	rm -rf $(DISTDIR)
