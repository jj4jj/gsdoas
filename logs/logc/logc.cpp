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
    std::string     json_msg_buff;
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
event_actor_t::event_actor_t(int type_, long long id_, const char * tag_, const ::google::protobuf::Message * base_):type(type_), id(id_), tag(tag_), base(base_) {
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
    ENV.json_msg_buff.reserve(1024*1024*4);
    return 0;
}
int logc_close(){
    if (ENV.msgq){
        dcsmq_destroy(ENV.msgq);
        ENV.msgq = nullptr;
    }
    return 0;
}
#define TEMP_STRING_REPLACE ("~<^;!;@#&)++}{/|0X7&24(5%?>m};|(*&)08;6#*^")
#define TEMP_STRING_REPLACE_LEN strlen(TEMP_STRING_REPLACE)
static inline void
construct_event(::logs::Event & evt, int type, const event_actor_t & actor, int flg, const char * tag) {
    evt.set_id(event_sn::next());
    evt.set_time(dcsutil::time_unixtime_ms());
    evt.set_type(type);
    evt.set_flag(flg);
    evt.set_src(ENV.report_src);
    evt.set_tag(tag ? tag : "");
    evt.mutable_actor()->set_type(actor.type);
    evt.mutable_actor()->set_id(actor.id);
    evt.mutable_actor()->set_tag(actor.tag ? actor.tag : "");
    evt.mutable_actor()->set_base(TEMP_STRING_REPLACE);
}
int logc_event(int type, const event_actor_t & actor, long long data, int flg , const char * tag ){
	::logs::Event evt;
	construct_event(evt, type, actor, flg, tag);
    string json;
    evt.set_data(TEMP_STRING_REPLACE);
    dcsutil::protobuf_msg_to_json_string(evt, json);
    if (actor.base) {
        std::string actor_base_json;
        dcsutil::protobuf_msg_to_json_string(*actor.base, actor_base_json);
        json.replace(json.find(TEMP_STRING_REPLACE)-1, TEMP_STRING_REPLACE_LEN+2, actor_base_json);
    }
    else {
        json.erase(json.find(TEMP_STRING_REPLACE), TEMP_STRING_REPLACE_LEN);
    }
    //data
    json.replace(json.find(TEMP_STRING_REPLACE) - 1, TEMP_STRING_REPLACE_LEN + 2, std::to_string(data));
    ::logs::EventMsg evmsg;
    evmsg.set_cmd(::logs::EventMsg_MsgCMD_CMD_LOG_DUMP);
    evmsg.set_flg(flg);
    evmsg.set_msg(json);
    if (evmsg.ByteSize() > (int)ENV.json_msg_buff.capacity()) {
        GLOG_ERR("error event msg size:%d", evmsg.ByteSize());
        return -1;
    }
    if (!evmsg.SerializeToArray((char*)ENV.json_msg_buff.data(), evmsg.ByteSize())) {
        GLOG_ERR("event msg pack error ! msg size:%d", evmsg.ByteSize());
        return -2;
    }
    dcsmq_msg_t dcmsg((char*)ENV.json_msg_buff.data(), evmsg.ByteSize());
	GLOG_DBG("%s", json.data());
	return dcsmq_send(ENV.msgq, ENV.smq_session_id, dcmsg);
}
int logc_event(int type, const event_actor_t & actor, const char * data, int flg, const char * tag){
    ::logs::Event evt;
    construct_event(evt, type, actor, flg, tag);
    string json;
    evt.set_data(data?data:"");
    dcsutil::protobuf_msg_to_json_string(evt, json);
    if (actor.base) {
        std::string actor_base_json;
        dcsutil::protobuf_msg_to_json_string(*actor.base, actor_base_json);
        json.replace(json.find(TEMP_STRING_REPLACE) - 1, TEMP_STRING_REPLACE_LEN + 2, actor_base_json);
    }
    else {
        json.erase(json.find(TEMP_STRING_REPLACE), TEMP_STRING_REPLACE_LEN );
    }
    ::logs::EventMsg evmsg;
    evmsg.set_cmd(::logs::EventMsg_MsgCMD_CMD_LOG_DUMP);
    evmsg.set_flg(flg);
    evmsg.set_msg(json);
    if (evmsg.ByteSize() > (int)ENV.json_msg_buff.capacity()) {
        GLOG_ERR("error event msg size:%d", evmsg.ByteSize());
        return -1;
    }
    if (!evmsg.SerializeToArray((char*)ENV.json_msg_buff.data(), evmsg.ByteSize())) {
        GLOG_ERR("event msg pack error ! msg size:%d", evmsg.ByteSize());
        return -2;
    }
    dcsmq_msg_t dcmsg((char*)ENV.json_msg_buff.data(), evmsg.ByteSize());
    GLOG_DBG("%s", json.data());
    return dcsmq_send(ENV.msgq, ENV.smq_session_id, dcmsg);
}
int logc_event(int type, const event_actor_t & actor, const ::google::protobuf::Message & data, int flg, const char * tag){
    ::logs::Event evt;
    construct_event(evt, type, actor, flg, tag);
    string json;
    evt.set_data(TEMP_STRING_REPLACE);
    dcsutil::protobuf_msg_to_json_string(evt, json);
    std::string temp_json_str;
    //first
    if (actor.base) {
        dcsutil::protobuf_msg_to_json_string(*actor.base, temp_json_str);
        json.replace(json.find(TEMP_STRING_REPLACE) - 1, TEMP_STRING_REPLACE_LEN + 2, temp_json_str);
    }
    else {
        json.erase(json.find(TEMP_STRING_REPLACE), TEMP_STRING_REPLACE_LEN);
    }
    //second
    temp_json_str = "";
    dcsutil::protobuf_msg_to_json_string(data, temp_json_str);
    json.replace(json.find(TEMP_STRING_REPLACE) - 1, TEMP_STRING_REPLACE_LEN + 2, temp_json_str);

    ::logs::EventMsg evmsg;
    evmsg.set_cmd(::logs::EventMsg_MsgCMD_CMD_LOG_DUMP);
    evmsg.set_flg(flg);
    evmsg.set_msg(json);
    if (evmsg.ByteSize() > (int)ENV.json_msg_buff.capacity()) {
        GLOG_ERR("error event msg size:%d", evmsg.ByteSize());
        return -1;
    }
    if (!evmsg.SerializeToArray((char*)ENV.json_msg_buff.data(), evmsg.ByteSize())) {
        GLOG_ERR("event msg pack error ! msg size:%d", evmsg.ByteSize());
        return -2;
    }
    dcsmq_msg_t dcmsg((char*)ENV.json_msg_buff.data(), evmsg.ByteSize());
    GLOG_DBG("%s", json.data());
    return dcsmq_send(ENV.msgq, ENV.smq_session_id, dcmsg);
}



}
}
