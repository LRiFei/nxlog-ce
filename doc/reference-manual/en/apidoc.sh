for i in  ../../../src/modules/*/*/*-api.xml ../../../src/common/core-api.xml; do
    ../apidoc.pl -lang en $i
done
