logd: logd.cpp logd.pb.cc ../share/logs.pb.cc ../../../dcpots/lib/libdcbase.a
	g++ --std=c++11 $^ -I../../../ ${protol} ${protoi} -o $@ -ldl -g -O2
logd.pb.cc:
	${protoc} 

install: logd
	mv logd ../../bin/
	rm -f *.o
clean:
	rm -f *.o
