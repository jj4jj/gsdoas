#include "../logc.h"
#include "../../../dcpots/base/logger.h"
int main(){
    global_logger_init(logger_config_t());
    gsdoas::logs::logc_init(gsdoas::logs::logc_config_t("/tmp","testing"));
    gsdoas::logs::logc_event(0, gsdoas::logs::event_actor_t(0, 1245, "haha"), "string data");
    gsdoas::logs::logc_event(0, gsdoas::logs::event_actor_t(1, 2355), 2041500);
    gsdoas::logs::logc_close();
    return 0;
}