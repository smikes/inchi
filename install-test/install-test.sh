#!/bin/sh
set -e

unset tmp temp
npm link

cd ..
mkdir inchi-test-install
cd inchi-test-install
npm install ../inchi
cp ../inchi/install-test/test.js .

node test.js

cd ..
rm -fr inchi-test-install
