#!/bin/bash

protoc=protoc
protol="-lprotobuf"
evl="-lev"
if [ "x$1" = "xu1" ];then
    protoc="../../../protobuf/bin/protoc"
    protol="../../../protobuf/lib/libprotobuf.a"
    protoi="-I../../../protobuf/include"
    evl="../../../libev/lib/libev.a"
fi
make protoc="${protoc}" protoi="${protoi}" protol="${protol}"
