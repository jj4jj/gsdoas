all: proto logc logd ex

proto:
	cd logs/proto && make
logc:
	cd logs/logc && make && make install

logd:
	cd logs/logd && make && make install

ex:
	cd logs/logc/examples/ && make && make install
