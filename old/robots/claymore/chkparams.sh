#!/bin/bash

TMPFILE=$(mktemp)
TMP2FILE=$(mktemp)
TMP3FILE=$(mktemp)

# echo Using temp files $TMPFILE, $TMP2FILE, $TMP3FILE

find . -name \*.cpp -exec grep 'getValue' \{} /dev/null \; | grep '".*"' > $TMPFILE
find executive -name \*.cpp -exec grep 'Drive(' \{} /dev/null \; | grep '".*"' >> $TMPFILE

cat $TMPFILE | sed 's/[^"]*"\([^"]*\)".*/\1/' | sort | uniq > $TMP2FILE

cat roborio/params-$1.txt | expand | cut -d ' ' -f 1 | sort | uniq > $TMP3FILE

echo Left column is source code
echo Right column is params file
sdiff -t -W -B -w 120 -s $TMP2FILE $TMP3FILE

rm $TMPFILE
rm $TMP2FILE
rm $TMP3FILE
