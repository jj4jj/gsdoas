#include "../logc.h"
#include "dcpots/base/logger.h"
#include "dcpots/base/dcutils.hpp"
#include "./bulog.pb.h"
using namespace gsdoas::logs;
int main(){
    global_logger_init(logger_config_t());
    gsdoas::logs::logc_init(gsdoas::logs::logc_config_t("/tmp", nullptr));
    gsdoas::logs::logc_event(0, gsdoas::logs::event_actor_t(0, 1245, "haha"), "string data");
    gsdoas::logs::logc_event(0, gsdoas::logs::event_actor_t(1, 2355), 2041500);

	int fd = dcsutil::openfd("file:///tmp/hello.txt");
	dcsutil::writefd(fd, "hello,world!", 0);
	dcsutil::closefd(fd);
	using namespace example;
	BagChange bc;
	BagChange::Change * ch = bc.add_change_list();
	ch->set_change(20);
	ch->set_type(2);
	ch->set_orgv(25);
	ch->set_newv(45);
	bc.set_tag("test");
	bc.set_reason(BagChange::CR_CHARGE);
	gsdoas::logs::logc_event(EVT_LOGIN, event_actor_t(0, 22245666, "player1"), bc);


	gsdoas::logs::logc_close();



	//dcsutil::time_call();
	
	return 0;
}
