html: nxlog-reference-manual.html
txt: nxlog-reference-manual.txt
pdf: nxlog-reference-manual.pdf

all-local: nxlog-reference-manual.txt nxlog-reference-manual.html nxlog-reference-manual.pdf

clean-local: clean-docs

version.txt:
	echo `cat ../../../VERSION`.`cat ../../../svn_version.txt` >version.txt

../config-examples/stamp-examples: $(wildcard ../config-examples/*.conf) \
                                   $(wildcard ../config-examples/*.stmnt) \
                                   ../config-examples/example-xml.sh \
                                   ../../../test/modules/processor/pattern/patterndb.xml
	cd ../config-examples && $(MAKE) stamp-examples
nxlog-reference-manual.html: $(DOC_SRC) ../config-examples/stamp-examples stamp-apidoc stamp-fields version.txt
	-xmlto html-nochunks nxlog-reference-manual.xml
nxlog-reference-manual.txt: $(DOC_SRC) ../config-examples/stamp-examples stamp-apidoc stamp-fields version.txt
	-xmlto txt nxlog-reference-manual.xml
nxlog-reference-manual.pdf: $(DOC_SRC) ../config-examples/stamp-examples stamp-apidoc stamp-fields version.txt
	-dblatex -P doc.collab.show=0 -P latex.output.revhistory=0 -P doc.lot.show="" nxlog-reference-manual.xml
#	-xmllint --xinclude nxlog-reference-manual.xml > nxlog-reference-manual-tmp.xml
#	-xsltproc -o nxlog-reference-manual.fo /usr/share/xml/docbook/stylesheet/nwalsh/fo/docbook.xsl nxlog-reference-manual-tmp.xml
#	-fop -pdf nxlog-reference-manual.pdf -fo nxlog-reference-manual.fo
#	-rm -f nxlog-reference-manual.fo
#	-rm -f nxlog-reference-manual-tmp.xml


CHAPTERS = architecture.xml language.xml quickstart.xml processing.xml forwarding.xml \
           configuration.xml introduction.xml offline.xml modules.xml sources.xml \
	   tips-tricks.xml troubleshooting.xml

MODULES = $(wildcard modules/extension/*.xml) \
          $(wildcard modules/input/*.xml) \
          $(wildcard modules/output/*.xml) \
          $(wildcard modules/processor/*.xml)

APIDOC_SRC := $(wildcard ../../../src/modules/*/*/*-api.xml) ../../../src/common/core-api.xml
stamp-apidoc: apidoc.sh $(APIDOC_SRC)
	-./apidoc.sh && touch stamp-apidoc

stamp-fields: fields.sh $(wildcard ../../../src/modules/*/*/*-fields.xml) ../../../src/common/core-fields.xml
	-./fields.sh && touch stamp-fields

DOC_SRC = nxlog-reference-manual.xml $(CHAPTERS) $(MODULES) \
          $(wildcard ../config-examples/*.conf) \
          $(wildcard ../config-examples/*.stmnt) $(figures_DATA) \
          ../../../svn_version.txt ../../../VERSION

clean-docs:
	-rm -f *.html
	-rm -f *.pdf

docbook_DATA	= nxlog-reference-manual.html nxlog-reference-manual.txt nxlog-reference-manual.pdf
docbookdir	= $(docdir)
figures_DATA	= $(wildcard ../figures/*.png)
figuresdir	= $(docdir)/figures

EXTRA_DIST = $(wildcard *.xml) $(docbook_DATA) $(CHAPTERS) $(MODULES) $(APIDOC) \
             apidoc.sh stamp-apidoc fields.sh stamp-fields
dist-hook:
	cp $(top_srcdir)/doc/reference-manual/en/*.xml $(distdir)/


maintainer-clean-local:
	-$(RM) $(docbook_DATA) $(wildcard apidoc-*.xml) stamp-apidoc \
	$(wildcard fields-*.xml) stamp-fields version.xml
