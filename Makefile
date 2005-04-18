VERSION = 0.7.4
DISTDIR = shmedia-$(VERSION)
HDR_DISTDIR = shmedia-hdr-$(VERSION)

DISTFILES = Makefile default.mk ChangeLog README brdfs envmaps horizonmaps largetextures textures objs mats bumpmaps sparse
HDR_DISTFILES = hdr

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
	rm -f $(DISTDIR).tar.gz
	tar cf $(DISTDIR).tar $(DISTDIR)
	bzip2 -k $(DISTDIR).tar
	gzip $(DISTDIR).tar
	rm -f $(DISTDIR).zip
	zip -r $(DISTDIR).zip $(DISTDIR)
	rm -rf $(DISTDIR)
	rm -rf $(HDR_DISTDIR)
	mkdir $(HDR_DISTDIR)
	cp -dpR $(HDR_DISTFILES) $(HDR_DISTDIR)
	rm -rf `find $(HDR_DISTDIR) -name ".depends" -o -name .done -o -name .svn -o -name CVS -o -name ".#*" -o -name "*~" -o -name nodist -o -name dist`
	rm -f $(HDR_DISTDIR).tar.gz
	tar cf $(HDR_DISTDIR).tar $(HDR_DISTDIR)
	bzip2 -k $(HDR_DISTDIR).tar
	gzip $(HDR_DISTDIR).tar
	rm -f $(HDR_DISTDIR).zip
	zip -r $(HDR_DISTDIR).zip $(HDR_DISTDIR)
	rm -rf $(HDR_DISTDIR)

clean:
	rm -f *.tar.gz
	rm -rf shmedia-*

