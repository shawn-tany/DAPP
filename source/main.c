#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <assert.h>

#include "common.h"
#include "config.h"
#include "dpdk.h"

#define STATIC_CONF_FILE "../install/config/static.conf"
#define DYNAMIC_CONF_FILE "../install/config/dynamic.conf"

#define CONF_FILE_NAME_SIZE (256)

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

static dapp_dpdk_args_alloc_set(dpdk_eal_args_t *dpdk_eal_args, char *arg)
{
    if (!dpdk_eal_args || !arg) {
        return DAPP_ERR_PARAM;
    }

    if (dpdk_eal_args->argc >= ITEM(dpdk_eal_args->argv))
    {
        return DAPP_ERR_EAL_PARAM;
    }

    dpdk_eal_args->argv[dpdk_eal_args->argc] = (char *)malloc(DPDK_EAL_ARGV_SIZE);
    assert(NULL != dpdk_eal_args->argv[dpdk_eal_args->argc]);
    snprintf(dpdk_eal_args->argv[dpdk_eal_args->argc], DPDK_EAL_ARGV_SIZE, "%s", arg);
    dpdk_eal_args->argc++;

    return DAPP_OK;
}

static STATUS dapp_dpdk_args_init(char *first, dapp_static_conf_t *static_conf, dpdk_eal_args_t **dpdk_eal_args)
{
    if (!first || !static_conf) {
        return DAPP_ERR_PARAM;
    }

    (*dpdk_eal_args) = (dpdk_eal_args_t *)malloc(sizeof(dpdk_eal_args_t));
    assert(NULL != (*dpdk_eal_args));
    memset((*dpdk_eal_args), 0, sizeof(dpdk_eal_args_t));

    /*
     * first arg
     */
    dapp_dpdk_args_alloc_set(*dpdk_eal_args, first);

    char cmd[DPDK_EAL_ARGV_SIZE];

    /*
     * arg lcore mask
     */
    int thread_num = 1;
    uint64_t lcore_mask = 7;
    thread_num += static_conf->port.thread_num;
    thread_num += static_conf->flow_iotonic.thread_num;
    thread_num += static_conf->proto_identi.thread_num;
    thread_num += static_conf->rule_match.thread_num;
    
    snprintf(cmd, sizeof(cmd), "-c%lu", lcore_mask);
    dapp_dpdk_args_alloc_set(*dpdk_eal_args, cmd);
    
    return DAPP_OK;
}

static void dapp_dpdk_args_exit(dpdk_eal_args_t *dpdk_eal_args)
{
    int i = 0;

    if (dpdk_eal_args) {
        for (i = 1; i < dpdk_eal_args->argc; ++i) {
            if (dpdk_eal_args->argv[i]) {
                free(dpdk_eal_args->argv[i]);
            }
        }

        free(dpdk_eal_args);
    }
}

static int dapp_business_loop(__attribute__((unused)) void *arg)
{
    unsigned lcore_id;
    lcore_id = dpdk_lcore_id();
    printf("dapp_business_loop lcore(%u)\n", lcore_id);
    return 0;
}

static int dapp_control_loop(__attribute__((unused)) void *arg)
{
    unsigned lcore_id;
    lcore_id = dpdk_lcore_id();
    printf("dapp_control_loop lcore(%u)\n", lcore_id);
    return 0;
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

    dpdk_eal_args_t *dpdk_eal_args = NULL;
    /*
     * set dpdk eal init args
     */
    if (DAPP_OK != (ret = dapp_dpdk_args_init(argv[0], &static_conf, &dpdk_eal_args))) {
        printf("dapp_dpdk_args_init fail\n");
        return ret;
    }
    
    dpdk_init(dpdk_eal_args);

    dpdk_run(dapp_business_loop, NULL, dapp_control_loop, NULL);

    dpdk_exit();

    dapp_dpdk_args_exit(dpdk_eal_args);

    return DAPP_OK;
}
