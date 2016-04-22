#pragma once
#include <string>
namespace gsdoas {
namespace logs {
using std::string;
struct logc_config_t {
   string  logd_msgq;
   int     channel_buffer_size;
   string  reporter_name;
   int     max_event_buffer_size;
   logc_config_t(const char * msgq_path, const char * reporter_name);
};
struct event_actor_t {
   int type;
   long long id;
   string tag;
   event_actor_t(int type_, long long id_, const char * tag_ = nullptr);
};

enum event_flag {
    EVT_FLAG_DIFF=0x01,
    EVT_FLAG_DUMP=0x02,
};

int logc_init(const logc_config_t & conf);
int logc_close();
int logc_event(int type, const event_actor_t & actor, long long data, int flg = EVT_FLAG_DIFF, const char * tag = nullptr);
int logc_event(int type, const event_actor_t & actor, const string & data, int flg = EVT_FLAG_DIFF, const char * tag = nullptr);
//int logger_event(int type, const event_actor_t && actor, const ::google::protobuf::Message & data, int flg = EVT_FLAG_DIFF, const char * tag = nullptr);


}

}
