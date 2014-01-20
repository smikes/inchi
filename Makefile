JSSOURCES=index.js $(shell find bin lib -name '*.js' -print)

install:
	(unset tmp temp; npm i)

prepublish: jslint cpplint no-dos-endings check-coverage

lint: jslint cpplint

cpplint:
	echo "No c++ lint tool available"

jslint:
	./node_modules/.bin/jslint --terse $(JSSOURCES); echo

test:
	./node_modules/.bin/mocha

no-dos-endings:
	file $(JSSOURCES) | grep -v CRLF > /dev/null

cover: $(JSSOURCES)
	./node_modules/.bin/istanbul cover --print=both ./node_modules/mocha/bin/_mocha --

check-coverage: cover
	./node_modules/.bin/istanbul check-coverage --statements 90 --branches 90 --functions 90 --lines 90

.PHONY: install jslint cpplint test doc no-dos-endings check-coverage
