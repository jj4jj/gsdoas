all: proto logc logd 

proto:
	cd logs/proto && make
logc:
	cd logs/logc && make && make install

logd:
	cd logs/logd && make && make install

