all: .done

clean:
	rm -f $(GENERATED)

%: nodist/%
	cp -a $< $@

%: dist/%
	cp -a $< $@

.done:
	touch .done

.depends:
	echo "GENERATED = " > .depends
	if [ -d nodist ] ; then for i in nodist/* ; do if [ `basename $$i` != CVS ] ; then echo ".done: " `basename $$i` >> .depends ; echo "GENERATED += `basename $$i`" >> .depends ; fi ; done ; fi
	if [ -d dist ] ; then for i in dist/* ; do if [ `basename $$i` != CVS ] ; then echo ".done: " `basename $$i` >> .depends ; echo "GENERATED += `basename $$i`" >> .depends ; fi ; done ; fi

include .depends
