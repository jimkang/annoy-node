TOOLS_CC = g++
TOOLS_CFLAGS = -lm -pthread -Ofast -march=native -Wall -funroll-loops -Wno-unused-result -std=c++11

TESTDATADIR = tests/data

build-wrapper:
	node-gyp rebuild

test: tests/data/text8-vector.json
	node tests/smalltest.js
	node tests/smalltest-manhattan.js
	node tests/basictests.js basic-config.js

big-test: tests/data/GoogleNews-vectors-negative300.json
	node tests/basictests.js very-big-config.js

tests/data/text8-vector.bin:
	wget https://github.com/jimkang/nearest-neighbor-test-data/raw/master/text8-vector.bin -O $(TESTDATADIR)/text8-vector.bin

# If this fails, you end up with a 0-byte json file.
# When you run this target again, make will see that the
# file exists, then quit. To get it to run, delete
# tests/data/GoogleNews-vectors-negative300.json
# See README about where to get the bin file used in this
# target.
tests/data/GoogleNews-vectors-negative300.json: tools/w2v-to-json
	./tools/w2v-to-json "$(TESTDATADIR)/GoogleNews-vectors-negative300.bin" tests/data/GoogleNews-vectors-negative300.json

tools/w2v-to-json:
	$(TOOLS_CC) tools/w2v-to-json.c -o tools/w2v-to-json $(TOOLS_CFLAGS)

tests/data/text8-vector.json: tests/data/text8-vector.bin tools/w2v-to-json
	./tools/w2v-to-json tests/data/text8-vector.bin tests/data/text8-vector.json

test-word-index-db:
	node tools/test-word-index-db.js tests/data/GoogleNews-vectors-negative300.json tests/data/word-index-google-news.db tests/data/very-big-test.annoy

pushall:
	git push origin master && npm publish

prettier:
	prettier --single-quote --write "**/*.js"
