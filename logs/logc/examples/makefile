write_log: ./write_log.cpp ../logc.cpp ../../share/logs.pb.cc ./bulog.pb.cc
	g++ --std=c++11 $^ -L../../../../dcpots/lib/ -ldcbase -ldcutil -lprotobuf -lpbjson -o $@ -I../../../../

./bulog.pb.cc: ./bulog.proto
	protoc bulog.proto --cpp_out=./


clean:
	rm *.o -f
