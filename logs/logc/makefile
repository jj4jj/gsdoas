liblogc.a: logc.cpp ../share/logs.pb.cc
	g++ --std=c++11 $^ -c -I../../../ ${protoi}
	ar -rcs liblogc.a *.o
install: example liblogc.a
	mv liblogc.a ../../lib/
	cp logc.h ../../include/
	rm *.o -f
clean:
	rm *.o examples/*.o -f

example: write_log

write_log: examples/write_log.cpp ../share/logs.pb.cc examples/bulog.pb.cc liblogc.a
        g++ --std=c++11 $^ -L../../lib -L../../../dcpots/lib/ -ldcbase -l'dcutil-drs' -lpbjson -o $@ -I../../../ -I${protoi} ${protol}

examples/bulog.pb.cc: examples/bulog.proto
	${protoc} bulog.proto --cpp_out=examples --proto_path=examples
	mv examples/write_log ../../bin/
