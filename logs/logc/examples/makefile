write_log: ./write_log.cpp ../../share/logs.pb.cc ./bulog.pb.cc
	g++ --std=c++11 $^ -L../../../lib -llogc  -L../../../../dcpots/lib/ -ldcbase -l'dcutil-drs' -lpbjson -o $@ -I../../../../ -I${protoi} ${protol}

./bulog.pb.cc: ./bulog.proto
	${protoc} bulog.proto --cpp_out=./
install:
	mv write_log ../../../bin/
	rm *.o -f
clean:
	rm *.o -f
