#include "dcpots/base/dcsmq.h"
#include "dcpots/base/logger.h"
#include "dcpots/base/cmdline_opt.h"
#include "dcpots/base/dcutils.hpp"

#define LOGD_VERSION    "0.0.1"
#define MAX_WORKER_NUM  16

static struct  {
    time_t    last_dump_time{ 0 };
    string    prev_write_file_name;
    string    write_datetime;
    string    write_file_name;
	int       worker_idx{ 0 };
    string    data_file_pattern;
    cmdline_opt_t *   cmdl{ nullptr };
	FILE *    dump_fp{ nullptr };
	bool	  stop{ false };
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
	while (!dump_file_env.stop){
        dcsmq_poll(dcsmq, 1000 * 10);//10ms
        usleep(1000);
    }

    if (dump_file_env.dump_fp){
		fclose(dump_file_env.dump_fp);
		dump_file_env.dump_fp = nullptr;
    }
}


int main(int argc, const char *argv[]){
    cmdline_opt_t cmdl(argc, argv);
    cmdl.parse("version:n:v:show version;"
               "log-dir:r::set log dir:/tmp;"
               "log-file:r::set log file pattern:/tmp/gslogd.{worker}.log;"
               "log-level:r::set log level[TRACE|DEBUG|INFO|WARN|ERROR]:INFO;"
               "log-file-size:r::set single log file size:1024*1024*10;"
               "log-roll:r::set log max roll num:20;"
               "data-dir:r::set dump data dir:/tmp;"
               "data-file:r::set dump data file pattern:bulog.{worker}.{datetime}.txt;"
               "worker:r::set worker thread num:1;"
               "msgq-key:r::communication with reporter msgq key path:/tmp;"
			   "pidfile:r::the running state file path dir:/tmp/{prog}.pid;"
			   "stop:n::stop the process;"
			   "daemon:n:D:daemon mode running;", LOGD_VERSION
               );
	string pidfile = cmdl.getoptstr("pidfile");
	dcsutil::strreplace(pidfile, "{prog}", dcsutil::path_base(argv[0]));
	if (cmdl.hasopt("stop")){
        GLOG_IFO("kill logd ...");
		return dcsutil::lockpidfile(pidfile, SIGTERM);
	}

    if (cmdl.hasopt("daemon")){
	    dcsutil::daemonlize(1, 0, pidfile.c_str());
	}
    else {
        int pidrunning = dcsutil::lockpidfile(pidfile);
        if (getpid() != pidrunning){
            GLOG_ERR("another process [%d] is running ...", pidrunning);
            return -1;
        }
    }
	//	typedef void(*sah_handler)(int sig, siginfo_t * sig_info, void * ucontex);
	auto sah = [](int sig, siginfo_t * sig_info, void * ucontex){
		GLOG_DBG("get a signal :%d", sig);
		if (SIGTERM == sig){
			dump_file_env.stop = true;
		}
	};
	dcsutil::signalh_push(SIGHUP, sah);
	dcsutil::signalh_push(SIGTERM, sah);

    int     children[MAX_WORKER_NUM];
	int		worker_num = cmdl.getoptint("worker");
    dump_file_env.cmdl = &cmdl;
	for (int i = 0; i < worker_num; ++i){
        children[i] = fork();
        if (children[i] == 0){ //child thread
            dump_file_env.worker_idx = i+1;
            dump_worker();
			GLOG_IFO("children i:%d -> pid:%d exit", i, getpid());
            return 0;
        }
        else {
            GLOG_IFO("create children i:%d -> pid:%d", i, children[i]);
        }
    }
	while (!dump_file_env.stop){
		sleep(1);
	}
	for (int i = 0; i < worker_num; ++i){
		GLOG_IFO("stop process [%d] success ...", children[i]);
		kill(children[i], SIGTERM); //notify children close
	}
    return 0;
}
