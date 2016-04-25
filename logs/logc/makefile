liblogc.a: logc.cpp ../share/logs.pb.cc
	g++ --std=c++11 $^ -c -I../../../dcpots -I../../../ -I../../../dcpots/3rd ../../../dcpots/base/logger.cpp ../../../dcpots/base/dcutils.cpp ../../../dcpots/base/dcsmq.cpp ../../../dcpots/utility/util_proto.cpp
	ar -rcs liblogc.a *.o

clean:
	rm *.o -f
