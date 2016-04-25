logd: logd.cpp logd.pb.cc ../share/logs.pb.cc
	g++ --std=c++11 $^ -I../../../ ../../../dcpots/base/*.cpp  ${protol} -I${protoi}  -o $@
install:
	mv logd ../../bin/
	rm -f *.o
clean:
	rm -f *.o
