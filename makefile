
all: logc logd ex

logc:
	cd logs/logc && make && mv *.a ../../lib/ && make clean && cp logc.h ../../include/

logd:
	cd logs/logd && make && mv logd ../../bin/ && make clean

ex:
	cd logs/logc/examples/ && make && mv write_log ../../../bin && make clean
