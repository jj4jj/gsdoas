PROJECT='gsdoas-example'
VERSION='0.0.1'
DEBUG = 1    #0/1
DEFS = []
VERBOSE = 'off'    #on/off
EXTRA_C_FLAGS = ''
EXTRA_CXX_FLAGS = '-std=c++11'
EXTRA_LD_FLAGS = '-ldl -lm -lrt -pthread'
LIBS = []
EXES = [
        {
            'name':'write_log',
            'subdir':'logd/examples/write_log',
            'includes':['/usr/local/include'],
            'linkpaths':['dcpots/lib'],
            'extra_srcs': ['logd'],
            'linklibs' : [
                'dcbase',
                'libprotobuf.a',
            ]
        },
        ]
