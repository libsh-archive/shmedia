VERSION = 0.0.20030807
DISTDIR = shmedia-$(VERSION)

DISTFILES = README brdfs envmaps textures

all: dist

dist:
	rm -rf $(DISTDIR)
	mkdir $(DISTDIR)
	cp -a $(DISTFILES) $(DISTDIR)
	rm -rf `find $(DISTDIR) -name CVS`
	rm -f $(DISTDIR).tar.gz
	tar czf $(DISTDIR).tar.gz $(DISTDIR)
	rm -rf $(DISTDIR)

clean:
	rm -f *.tar.gz
	rm -rf shmedia-*
