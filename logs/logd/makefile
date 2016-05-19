logd: logd.cpp logd.pb.cc ../share/logs.pb.cc ../../../dcpots/lib/libdcbase.a
	g++ --std=c++11 $^ -I../../../ ${protol} -I${protoi} -o $@
install:
	mv logd ../../bin/
	rm -f *.o
clean:
	rm -f *.o
