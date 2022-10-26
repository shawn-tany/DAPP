#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "common.h"
#include "config.h"

#define STATIC_CONF_FILE "../install/config/static.conf"
#define DYNAMIC_CONF_FILE "../install/config/dynamic.conf"

#define CONF_FILE_NAME_SIZE (256)
#define DAPP_DPDK_ARGC (32)

typedef struct {
    char static_conf_file[CONF_FILE_NAME_SIZE];
    char dynamic_conf_file[CONF_FILE_NAME_SIZE];
} dapp_ctrl_t;

static struct option long_options[] = {
    {"statis",      1,  0,  's'},
    {"dynamic",     1,  0,  'd'},
    {"help",        0,  0,  'h'},
    {0,             0,  0,  0}
};

static STATUS dapp_args_parse(int argc, char *argv[], dapp_ctrl_t *ctrl)
{
    if (!argv || !ctrl) {
        return DAPP_ERR_PARAM;
    }

    int opt = 0;

    /*
     * Set default parameters
     */
    snprintf(ctrl->static_conf_file, sizeof(ctrl->static_conf_file), "%s", STATIC_CONF_FILE);
    snprintf(ctrl->dynamic_conf_file, sizeof(ctrl->dynamic_conf_file), "%s", DYNAMIC_CONF_FILE);

    /*
     * Parse command line parameters
     */
    while (-1 != (opt = getopt_long(argc, argv, "s:d:h", long_options, NULL))) {
        switch (opt) {
            case 's' :
                snprintf(ctrl->static_conf_file, sizeof(ctrl->static_conf_file), "%s", optarg);
                break;
            case 'd' :
                snprintf(ctrl->dynamic_conf_file, sizeof(ctrl->dynamic_conf_file), "%s", optarg);
                break;
            case 'h' :
                printf("%s OPTIONS :\n"
                       "    -s, --static, set static configuration file\n"
                       "    -d, --dynamic, set dynamic configuration file\n"
                       "    -h, --help, show optoins\n", argv[0]);
                break;
            default :
                printf("invalid optoin!\n");
                printf("%s OPTIONS :\n"
                       "    -s, --static, set static configuration file\n"
                       "    -d, --dynamic, set dynamic configuration file\n"
                       "    -h, --help, show optoins\n", argv[0]);
                return DAPP_ERR_OPTION;
        }
    }

    return DAPP_OK;
}

static STATUS dapp_dpdk_args_set(int *argc, char *argv[], dapp_static_conf_t *static_conf)
{
    *argc = 1;
    
    /*
     * lcore
     */
    int thread_num = 0;
    thread_num += static_conf->port.thread_num;
    thread_num += static_conf->flow_iotonic.thread_num;
    thread_num += static_conf->proto_identi.thread_num;
    thread_num += static_conf->rule_match.thread_num;

    int i = 0;
    uint64_t lcore_mask = 0;
    for (i = 0; i < thread_num; ++i) {
        sprintf(argv[argc] + , "%d@(%d)", i, cfg->cpu[i]);
    }
    
    
    sprintf(argv[argc], "--lcores=", )
}

int main(int argc, char *argv[])
{
    dapp_ctrl_t dapp_ctrl;
    memset(&dapp_ctrl, 0, sizeof(dapp_ctrl));

    dapp_static_conf_t static_conf;
    memset(&static_conf, 0, sizeof(static_conf));

    STATUS ret = DAPP_OK;

    /*
     * Parse command line parameters
     */
    if (DAPP_OK != (ret = dapp_args_parse(argc, argv, &dapp_ctrl))) {
        printf("dapp_args_parse fail\n");
        return ret;
    }

    /*
     * resolve static configuration
     */
    if (DAPP_OK != (ret = dapp_static_conf_parse(&static_conf, dapp_ctrl.static_conf_file))) {
        printf("dapp_static_conf_parse fail\n");
        return ret;
    }

    /*
     * debug, show static configuration
     */
    dapp_static_conf_dump(&static_conf);

    int argc0 = 1;
    char *argv0[]
    /*
     * set dpdk eal init args
     */
    if (DAPP_OK != (ret = dapp_dpdk_args_set(&argc, argv, &static_conf))) {
        printf("dapp_dpdk_args_set fail\n");
        return ret;
    }
    

    return DAPP_OK;
}
