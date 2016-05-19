#include "dcpots/base/logger.h"
#include "dcpots/base/dcsmq.h"
#include "dcpots/utility/drs/dcproto.h"
#include "dcpots/base/dcutils.hpp"
#include "dcpots/base/dcseqnum.hpp"
////////////////////////////////////
#include "../share/logs.pb.h"
////////////////////////////////////
#include "logc.h"

namespace gsdoas {
namespace logs {
static struct {
    dcsmq_t * msgq{ nullptr };
    int32_t   smq_session_id{ 0 };
	string	  report_src;
} ENV;
typedef dcsutil::sequence_number_t<8,24,false>	event_sn;

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
    ENV.smq_session_id = getpid();
	ENV.report_src = conf.reporter_name;
    return 0;
}
int logc_close(){
    if (ENV.msgq){
        dcsmq_destroy(ENV.msgq);
        ENV.msgq = nullptr;
    }
    return 0;
}
static inline void
construct_event(::logs::Event & evt, int type, const event_actor_t & actor, int flg, const char * tag){
	evt.set_id(event_sn::next());
	evt.set_time(dcsutil::time_unixtime_ms());
	evt.set_type(type);
	evt.set_flag(flg);
	evt.set_src(ENV.report_src);
	evt.set_tag(tag?tag:"");
	evt.mutable_actor()->set_type(actor.type);
	evt.mutable_actor()->set_id(actor.id);
	evt.mutable_actor()->set_tag(actor.tag);
}
int logc_event(int type, const event_actor_t & actor, long long data, int flg , const char * tag ){
	::logs::Event evt;
	flg |= EVT_FLAG_JMSG;
	construct_event(evt, type, actor, flg, tag);
	evt.set_data(std::to_string(data));
	string json;
	dcsutil::protobuf_msg_to_json_string(evt, json);
	dcsmq_msg_t dcmsg((char*)json.data(), json.length());
	GLOG_DBG("%s", json.data());
	return dcsmq_send(ENV.msgq, ENV.smq_session_id, dcmsg);
}
int logc_event(int type, const event_actor_t & actor, const string & data, int flg, const char * tag){
	::logs::Event evt;
	construct_event(evt, type, actor, flg, tag);
	evt.set_data(data);
	string json;
	dcsutil::protobuf_msg_to_json_string(evt, json);
	dcsmq_msg_t dcmsg((char*)json.data(), json.length());
	GLOG_DBG("%s", json.data());
	return dcsmq_send(ENV.msgq, ENV.smq_session_id, dcmsg);
}
int logc_event(int type, const event_actor_t & actor, const ::google::protobuf::Message & data, int flg, const char * tag){
	string json_data;
	dcsutil::protobuf_msg_to_json_string(data, json_data);
	flg |= EVT_FLAG_JMSG;
	return logc_event(type, actor, json_data, flg, tag);
}



}
}
