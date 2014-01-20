JSSOURCES=$(shell find bin lib -name '*.js' -print)
CPPSOURCES=$(shell echo src/*.{cc,h})
DOCDIRS=src lib

install:
	(unset tmp temp; npm i)

prepublish: lint doc test no-dos-endings check-coverage

lint: jslint cpplint doclint

cpplint:
	./node_modules/.bin/node-cpplint $(CPPSOURCES)

jslint:
	./node_modules/.bin/jslint --terse -- $(JSSOURCES) ; echo

doclint:
	./node_modules/.bin/yuidoc $(DOCDIRS) --lint

test: cpptest jstest

jstest:
	./node_modules/.bin/mocha

build:
	(unset tmp temp; npm run-script build)

cpptest: build
	./build/Release/test


no-dos-endings:
	file $(JSSOURCES) | grep -v CRLF > /dev/null

# coverage only for javascript at this point
cover: $(JSSOURCES)
	./node_modules/.bin/istanbul cover --print=both ./node_modules/mocha/bin/_mocha --

check-coverage: cover
	./node_modules/.bin/istanbul check-coverage --statements 90 --branches 90 --functions 90 --lines 90


doc:
	./node_modules/.bin/yuidoc $(DOCDIRS)

.PHONY: install jslint cpplint cpptest jstest doc no-dos-endings check-coverage build
