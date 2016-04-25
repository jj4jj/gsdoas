liblogc.a: logc.cpp ../share/logs.pb.cc
	g++ --std=c++11 $^ -c -I../../../dcpots -I../../../ -I../../../dcpots/3rd ../../../dcpots/base/logger.cpp ../../../dcpots/base/dcutils.cpp ../../../dcpots/base/dcsmq.cpp ../../../dcpots/utility/util_proto.cpp ../../../dcpots/utility/xml_doc.cpp ../../../dcpots/3rd/pbjson/pbjson.cpp -I${protoi}
	ar -rcs liblogc.a *.o
install: liblogc.a
	mv liblogc.a ../../lib/
	cp logc.h ../../include/
	rm *.o -f
clean:
	rm *.o -f
