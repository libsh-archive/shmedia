VERSION = 0.7.2
DISTDIR = shmedia-$(VERSION)

DISTFILES = ChangeLog README brdfs envmaps horizonmaps textures objs mats bumpmaps sparse

SUBDIRS = textures

all: recurse-all

recurse-all:
	for i in $(SUBDIRS) ; do cd $$i && $(MAKE) && cd .. ; done

dist:
	rm -rf $(DISTDIR)
	mkdir $(DISTDIR)
	cp -a $(DISTFILES) $(DISTDIR)
	rm -rf `find $(DISTDIR) -name ".depends" -o -name .done -o -name .svn -o -name CVS -o -name ".#*" -o -name "*~" -o -name nodist -o -name dist`
	rm -f $(DISTDIR).tar.gz
	tar cf $(DISTDIR).tar $(DISTDIR)
	bzip2 -k $(DISTDIR).tar
	gzip $(DISTDIR).tar
	rm -f $(DISTDIR).zip
	zip -r $(DISTDIR).zip $(DISTDIR)
	rm -rf $(DISTDIR)

clean:
	rm -f *.tar.gz
	rm -rf shmedia-*

