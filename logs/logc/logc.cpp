#include "logc.h"
#include "../../dcpots/base/logger.h"
#include "../../dcpots/base/dcsmq.h"
namespace gsdoas {
namespace logs {
static struct {
    dcsmq_t * msgq{ nullptr };
    int32_t   myid{ 0 };
} ENV;

logc_config_t::logc_config_t(const char * msgq_path, const char * report_name) :logd_msgq(msgq_path),
    channel_buffer_size(20 * 1024 * 1024),
    max_event_buffer_size(1024*1024){
    if (report_name){
        reporter_name = report_name;
    }
    else { //ip
        reporter_name = dcsutil::stripfromu32v4(dcsutil::localhost_getipv4());
    }
}

event_actor_t::event_actor_t(int type_, long long id_, const char * tag_ ) :type(type_), id(id_){
    if (tag_){
        tag = tag_;
    }
}

int logc_init(const logc_config_t & conf){
    dcsmq_config_t dcs_conf;
    dcs_conf.keypath = conf.logd_msgq;
    dcs_conf.max_queue_buff_size = conf.channel_buffer_size;
    dcs_conf.min_queue_buff_size = 0;
    dcs_conf.passive = false;
    dcs_conf.msg_buffsz = conf.max_event_buffer_size;
    ENV.msgq = dcsmq_create(dcs_conf);
    if (!ENV.msgq){
        GLOG_ERR("dcsmq create error !");
        return -1;
    }
    ENV.myid = getpid();
    return 0;
}
int logc_close(){
    if (ENV.msgq){
        dcsmq_destroy(ENV.msgq);
        ENV.msgq = nullptr;
    }
    return 0;
}
int logc_event(int type, const event_actor_t & actor, long long data, int flg , const char * tag ){
    GLOG_IFO("logger event type:%d actor:{type:%d id:%d tag:%s} data:%ld flag:%d tag:%s", type, actor.type,
        actor.id, actor.tag.c_str(), data, flg, tag?tag:"");
    //dcsmq_send
    dcsmq_send(ENV.msgq, ENV.myid, dcsmq_msg_t("hello, world 1!", 12));
    return 0;
}
int logc_event(int type, const event_actor_t & actor, const string & data, int flg, const char * tag){
    GLOG_IFO("logger event type:%d actor:{type:%d id:%d tag:%s} data:%s flag:%d tag:%s", type, actor.type,
        actor.id, actor.tag.c_str(), data.c_str(), flg, tag ? tag : "");
    dcsmq_send(ENV.msgq, ENV.myid, dcsmq_msg_t("hello, world 2!", 12));
    return 0;
}
}
}
