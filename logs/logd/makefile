logd: logd.cpp logd.pb.cc ../share/logs.pb.cc
	g++ --std=c++11 $^ -I../../../ -L../../../dcpots/lib/ -ldcbase -lprotobuf  -o $@ 
clean:
	rm -f *.o
