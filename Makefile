TESTDATADIR = tests/data

tests/data/text8-vector.bin:
	wget https://github.com/jimkang/nearest-neighbor-test-data/raw/master/text8-vector.bin -O $(TESTDATADIR)/text8-vector.bin
