#!/bin/bash
proto_path=$1
#set -x
[ "x$proto_path" = "x" ] && make protoc=protoc protoi="/usr/local/include" protol="-lprotobuf" && exit $?
make protoc=${proto_path}/bin/protoc protoi=${proto_path}/include protol=${proto_path}/lib/libprotobuf.a
