all: logs logd
logs: logs.proto
	${protoc} $^ --cpp_out=../share/

logd: logd.proto
	${protoc} $^ --cpp_out=../logd/

logc: logc.proto
	${protoc} $^ --cpp_out=../logc/
