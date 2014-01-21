#!/bin/sh
set -e

# test of local install
unset tmp temp

echo Making local install directory -n
cd ..
rm -fr inchi-test-install
mkdir inchi-test-install
cd inchi-test-install

echo Staring installation
npm install ../inchi

echo Running test
cp ../inchi/install-test/test.js .
node test.js

cd ..
rm -fr inchi-test-install
