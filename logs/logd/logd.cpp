#include "../../dcpots/base/dcsmq.h"
#include "../../dcpots/base/logger.h"
#include "../../dcpots/base/cmdline_opt.h"
#include "../../dcpots/base/dcutils.hpp"

#define LOGD_VERSION    "0.0.1"
#define MAX_WORKER_NUM  16

static struct  {
    time_t    last_dump_time{ 0 };
    string    prev_write_file_name;
    string    write_datetime;
    string    write_file_name;
    int       dump_fd{ -1 };
    int       worker_idx;
    string    data_file_pattern;
    cmdline_opt_t *   cmdl{ nullptr };
    FILE *    dump_fp{ nullptr };
} dump_file_env;

static int dump_fp(dcsmq_t *, uint64_t src, const dcsmq_msg_t & msg, void * ud) {
    time_t now = dcsutil::time_unixtime_s();
    if (!dcsutil::time_same_hour(dump_file_env.last_dump_time, now) ||
        dump_file_env.dump_fp == nullptr){
        dump_file_env.write_file_name = dump_file_env.data_file_pattern;
        dcsutil::strreplace(dump_file_env.write_file_name, "{worker}", std::to_string(dump_file_env.worker_idx));
        dcsutil::strreplace(dump_file_env.write_file_name, "{datetime}",
            dcsutil::strftime(dump_file_env.write_datetime, now, "%Y_%m_%d_%H"));
        if (dump_file_env.prev_write_file_name != dump_file_env.write_file_name){
            if (dump_file_env.dump_fp){
                fclose(dump_file_env.dump_fp);
                dump_file_env.dump_fp = nullptr;
            }
            string filename = dump_file_env.write_file_name;
            dump_file_env.dump_fp = fopen(filename.c_str(), "a+");
            dump_file_env.prev_write_file_name = dump_file_env.write_file_name;
        }
    }
    if (dump_file_env.dump_fp){
        fputs("{\"time\":\"", dump_file_env.dump_fp);
        fputs(dcsutil::strftime(dump_file_env.write_datetime, now), dump_file_env.dump_fp);
        fputs("\",\"event\":", dump_file_env.dump_fp);
        fwrite(msg.buffer, msg.sz, 1, dump_file_env.dump_fp);
        fputs("}\n", dump_file_env.dump_fp);
        fflush(dump_file_env.dump_fp);
    }
    return 0;
}

static void dump_worker(){
    logger_config_t global_log_conf;
    global_log_conf.dir = dump_file_env.cmdl->getoptstr("log-dir");
    global_log_conf.pattern = dump_file_env.cmdl->getoptstr("log-file");
    dcsutil::strreplace(global_log_conf.pattern, "{worker}", std::to_string(dump_file_env.worker_idx));
    global_log_conf.lv = INT_LOG_LEVEL(dump_file_env.cmdl->getoptstr("log-level"));
    global_log_conf.max_file_size = (dump_file_env.cmdl->getoptint("log-file-size"));
    global_log_conf.max_roll = (dump_file_env.cmdl->getoptint("log-roll"));
    global_logger_init(logger_config_t());

    dump_file_env.data_file_pattern = dump_file_env.cmdl->getoptstr("data-dir");
    dump_file_env.data_file_pattern += "/";
    dump_file_env.data_file_pattern += dump_file_env.cmdl->getoptstr("data-file");

    dcsmq_config_t dcsmq_conf;
    dcsmq_conf.attach = false;
    dcsmq_conf.keypath = dump_file_env.cmdl->getoptstr("msgq-key");
    dcsmq_conf.max_queue_buff_size = 1024 * 1024 * 100;
    dcsmq_conf.min_queue_buff_size = 0;
    dcsmq_conf.msg_buffsz = 1024 * 1024;
    dcsmq_conf.passive = true;
    dcsmq_t * dcsmq = dcsmq_create(dcsmq_conf);
    if (!dcsmq){
        GLOG_ERR("create msgq error !");
        return;
    }
    dcsmq_msg_cb(dcsmq, dump_fp, nullptr);
    while (true){
        dcsmq_poll(dcsmq, 1000 * 10);//10ms
        usleep(1000);
    }

    if (dump_file_env.dump_fd >= 0){
        dcsutil::closefd(dump_file_env.dump_fd);
    }
}


int main(int argc, char *argv[]){
    cmdline_opt_t cmdl(argc, argv);
    cmdl.parse("version:n:v:show version;"
               "log-dir:r::set log dir:/tmp;"
               "log-file:r::set log file pattern:/tmp/gslogd.{worker}.log;"
               "log-level:r::set log level[TRACE|DEBUG|INFO|WARN|ERROR]:INFO;"
               "log-file-size:r::set single log file size:1024*1024*10;"
               "log-roll:r::set log max roll num:20;"
               "data-dir:r::set dump data dir:/tmp;"
               "data-file:r::set dump data file pattern:bulog.{worker}.{datetime};"
               "worker:r::set worker thread num:1;"
               "msgq-key:r::communication with reporter msgq key path:/tmp;"
               "daemon:n:D:daemon mode running;"
               );
    if (cmdl.hasopt("version")){
        std::cout << LOGD_VERSION << std::endl;
        return 0;
    }
    if (cmdl.hasopt("daemon")){
        dcsutil::daemonlize();
    }
    int     children[MAX_WORKER_NUM];
    dump_file_env.cmdl = &cmdl;
    for (int i = 0; i < cmdl.getoptint("worker"); ++i){
        children[i] = fork();
        if (children[i] == 0){ //child thread            
            dump_file_env.worker_idx = i+1;
            dump_worker();
            return 0;
        }
        else {
            GLOG_IFO("create children i:%d -> pid:%d", i, children[i]);
        }
    }
    //waiting all children 
    while (true){
        //poll command
        wait();
    }
    return 0;
}
