all-local: stamp-examples

stamp-examples: $(wildcard *.conf) $(wildcard *.stmnt) example-xml.sh         \
                ../../../test/modules/processor/pattern/patterndb.xml         \
                ../../../test/modules/processor/evcorr/evcorr2.conf           \
                ../../../test/modules/processor/evcorr/testinput_evcorr2.txt  \
                ../../../test/modules/processor/evcorr/testoutput_evcorr2.txt \
                ../../../test/modules/extension/multiline/xm_multiline1.conf  \
                ../../../test/modules/extension/multiline/xm_multiline1.in    \
                ../../../test/modules/extension/multiline/xm_multiline1.out   \
                ../../../test/modules/extension/multiline/xm_multiline2.conf  \
                ../../../test/modules/extension/multiline/xm_multiline2.in    \
                ../../../test/modules/extension/multiline/xm_multiline2.out   \
                ../../../test/modules/extension/multiline/xm_multiline3.conf  \
                ../../../test/modules/extension/multiline/xm_multiline3.in    \
                ../../../test/modules/extension/multiline/xm_multiline3.out   \
                ../../../test/modules/extension/multiline/xm_multiline4.conf  \
                ../../../test/modules/extension/multiline/xm_multiline4.in    \
                ../../../test/modules/extension/multiline/xm_multiline4.out   \
                ../../../test/modules/extension/multiline/xm_multiline5.conf  \
                ../../../test/modules/extension/multiline/xm_multiline5.in    \
                ../../../test/modules/extension/multiline/xm_multiline5.out   \
                ../../../test/modules/extension/perl/xm_perl.conf             \
                ../../../test/modules/extension/perl/processlogs.pl           \
                ../../../test/modules/extension/kvp/xm_kvp5.conf              \
                ../../../test/modules/extension/kvp/xm_kvp5.in                \
                ../../../test/modules/extension/kvp/xm_kvp5.out               \
                ../../../test/modules/extension/kvp/apache_url.conf           \
                ../../../test/modules/extension/kvp/apache_url.in             \
                ../../../test/modules/extension/kvp/apache_url.out            \
                ../../../test/modules/extension/kvp/cisco_acs.conf            \
                ../../../test/modules/extension/kvp/cisco_acs.in              \
                ../../../test/modules/extension/kvp/cisco_acs.out             \
                ../../../test/modules/extension/kvp/sidewinder.conf           \
                ../../../test/modules/extension/kvp/sidewinder.in             \
                ../../../test/modules/extension/kvp/sidewinder.out

	-./example-xml.sh && touch stamp-examples

maintainer-clean-local:
	-$(RM) $(wildcard *.xml) stamp-examples

dist-hook:
	cp $(top_srcdir)/doc/reference-manual/config-examples/*.xml $(distdir)/

EXTRA_DIST = $(wildcard *.xml) $(wildcard *.conf) $(wildcard *.stmnt) stamp-examples example-xml.sh example-tester.sh
