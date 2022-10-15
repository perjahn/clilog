#!/bin/bash
set -e
clang testapp.c -o testapp
clang clilog.c -o clilog

echo 'Unbuffered...'
./testapp 11 | ./clilog -NoAnsi
echo 'Fully buffered...'
./testapp 12 | ./clilog -NoAnsi
echo 'Unuffered, as script...'
script -q -c "./testapp 11" | ./clilog -NoAnsi
echo 'Linebuffered, as script...'
script -q -c "./testapp 12" | ./clilog -NoAnsi
rm typescript
