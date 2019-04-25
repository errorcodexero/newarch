#!/bin/bash

TMP1FILE=$(mktemp)
TMP2FILE=$(mktemp)


# echo Using temp files $TMPFILE, $TMP2FILE, $TMP3FILE

cat roborio/params-$1.txt | sort > $TMP1FILE
cat roborio/params-$2.txt | sort > $TMP2FILE

echo Left column is file $1
echo Right column is file $2
sdiff -s -t -W -B -w 120 $TMP1FILE $TMP2FILE

rm $TMP2FILE
rm $TMP1FILE
