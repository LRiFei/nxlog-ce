for i in ../../../src/modules/*/*/*-fields.xml; do
    ../fields.pl -lang en $i
done
../fields.pl -lang en ../../../src/common/core-fields.xml
