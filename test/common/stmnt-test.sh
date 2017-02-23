FAILED=0
for i in statements/good/*; do
    echo $i |grep ".conf" >/dev/null;
    if test $? = 1; then
	if ! ./stmnt-test $i 2>/dev/null >/dev/null; then
	    echo "$i failed"; 
	    FAILED=1;
	fi
    fi
done
if test $FAILED = 0; then
    echo "$0 OK"
else
    echo "Test failed"
    exit 1;
fi

