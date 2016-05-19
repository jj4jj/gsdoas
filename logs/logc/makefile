liblogc.a: logc.cpp ../share/logs.pb.cc
	g++ --std=c++11 $^ -c -I../../../ -I${protoi}
	ar -rcs liblogc.a *.o
install: liblogc.a
	mv liblogc.a ../../lib/
	cp logc.h ../../include/
	rm *.o -f
clean:
	rm *.o -f
