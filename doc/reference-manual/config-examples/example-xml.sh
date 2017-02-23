#!/bin/sh

for i in *.conf; do
    FILENAME=config-example-`echo $i| sed s/.conf$/.xml/`
    echo '<?xml version="1.0" encoding="UTF-8"?>' > $FILENAME
    echo '<programlisting><![CDATA[' >> $FILENAME
    cat $i |grep -v 'include common.conf' >> $FILENAME
    echo -n ']]></programlisting>' >> $FILENAME
done

for i in *.stmnt; do
    FILENAME=stmnt-example-`echo $i| sed s/.stmnt$/.xml/`
    echo '<?xml version="1.0" encoding="UTF-8"?>' > $FILENAME
    echo '<programlisting><![CDATA[' >> $FILENAME
    cat $i >> $FILENAME
    echo '' >> $FILENAME
    echo ']]></programlisting>' >> $FILENAME
done

for i in ../../../test/modules/processor/pattern/patterndb.xml          \
         ../../../test/modules/processor/evcorr/evcorr2.conf            \
         ../../../test/modules/processor/evcorr/testinput_evcorr2.txt   \
         ../../../test/modules/processor/evcorr/testoutput_evcorr2.txt  \
         ../../../test/modules/extension/multiline/xm_multiline1.conf   \
         ../../../test/modules/extension/multiline/xm_multiline1.in     \
         ../../../test/modules/extension/multiline/xm_multiline1.out    \
         ../../../test/modules/extension/multiline/xm_multiline2.conf   \
         ../../../test/modules/extension/multiline/xm_multiline2.in     \
         ../../../test/modules/extension/multiline/xm_multiline2.out    \
         ../../../test/modules/extension/multiline/xm_multiline3.conf   \
         ../../../test/modules/extension/multiline/xm_multiline3.in     \
         ../../../test/modules/extension/multiline/xm_multiline3.out    \
         ../../../test/modules/extension/multiline/xm_multiline4.conf   \
         ../../../test/modules/extension/multiline/xm_multiline4.in     \
         ../../../test/modules/extension/multiline/xm_multiline4.out    \
         ../../../test/modules/extension/multiline/xm_multiline5.conf   \
         ../../../test/modules/extension/multiline/xm_multiline5.in     \
         ../../../test/modules/extension/multiline/xm_multiline5.out    \
         ../../../test/modules/extension/perl/xm_perl.conf              \
         ../../../test/modules/extension/perl/processlogs.pl            \
         ../../../test/modules/extension/kvp/xm_kvp5.conf               \
         ../../../test/modules/extension/kvp/xm_kvp5.in                 \
         ../../../test/modules/extension/kvp/xm_kvp5.out                \
         ../../../test/modules/extension/kvp/apache_url.conf            \
         ../../../test/modules/extension/kvp/apache_url.in              \
         ../../../test/modules/extension/kvp/apache_url.out             \
         ../../../test/modules/extension/kvp/cisco_acs.conf             \
         ../../../test/modules/extension/kvp/cisco_acs.in               \
         ../../../test/modules/extension/kvp/cisco_acs.out              \
         ../../../test/modules/extension/kvp/sidewinder.conf            \
         ../../../test/modules/extension/kvp/sidewinder.in              \
         ../../../test/modules/extension/kvp/sidewinder.out             \
; do
    FILENAME=config-example-`basename $i`.xml
    echo '<?xml version="1.0" encoding="UTF-8"?>' > $FILENAME
    echo '<programlisting><![CDATA[' >> $FILENAME
    cat $i | grep -v 'include tmp/common.conf' >> $FILENAME
    echo '' >> $FILENAME
    echo ']]></programlisting>' >> $FILENAME
done
