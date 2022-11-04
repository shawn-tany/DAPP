#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <assert.h>

#include "common.h"
#include "config.h"
#include "dpdk.h"
#include "dapp_code.h"
#include "modules.h"

#define CONF_FILE "/etc/DAPP/config/startup.conf"
#define RULE_FILE "/etc/DAPP/config/rule.conf"

#define CONF_FILE_NAME_SIZE (256)

#define TRUE    (1)
#define FALSE   (0)

typedef struct 
{
    char program[128];
    dapp_conf_t conf;
    uint64_t lcore_mask;
} dapp_usrspace_t;

static STATUS dapp_usrspace_init(int argc, char **argv, dapp_usrspace_t *usrspace)
{
    if (!argc || !argv || !usrspace) {
        return DAPP_ERR_PARAM;
    }

    struct option long_options[] = {
        {"config",      1,  0,  's'},
        {"rule",        1,  0,  'r'},
        {"help",        0,  0,  'h'},
        {0,             0,  0,  0}
    };

    int opt = 0;
    STATUS ret = DAPP_OK;
    char conf_file[CONF_FILE_NAME_SIZE] = {0};
    char rule_file[CONF_FILE_NAME_SIZE] = {0};

    /*
     * Set default parameters
     */
    snprintf(conf_file, sizeof(conf_file), "%s", CONF_FILE);
    snprintf(rule_file, sizeof(rule_file), "%s", RULE_FILE);

    /*
     * Parse command line parameters
     */
    while (-1 != (opt = getopt_long(argc, argv, "c:r:h", long_options, NULL))) {
        switch (opt) {
            case 'c' :
                snprintf(conf_file, sizeof(conf_file), "%s", optarg);
                break;
            case 'r' :
                snprintf(rule_file, sizeof(rule_file), "%s", optarg);
                break;
            case 'h' :
                printf("%s OPTIONS :\n"
                       "    -c, --config, set startup configuration file\n"
                       "    -r, --rule,   set rule configuration file\n"
                       "    -h, --help,   show optoins\n", argv[0]);
                break;
            default :
                printf("invalid optoin!\n");
                printf("%s OPTIONS :\n"
                       "    -c, --config, set startup configuration file\n"
                       "    -r, --rule,   set rule configuration file\n"
                       "    -h, --help,   show optoins\n", argv[0]);
                return DAPP_ERR_OPTION;
        }
    }

    /*
     * program 
     */
    snprintf(usrspace->program, sizeof(usrspace->program), "%s", argv[0]);
    
    /*
     * resolve startup configuration
     */
    if (DAPP_OK != (ret = dapp_conf_parse(&usrspace->conf, conf_file))) {
        printf("dapp conf parse fail\n");
        return ret;
    }

    /*
     * debug, show static configuration
     */
    dapp_conf_dump(&usrspace->conf);

    /*
     * Update module lcore
     */
    dapp_module_lcore_init(DAPP_MODULE_CONTROL, 1);
    dapp_module_lcore_init(DAPP_MODULE_PORT, usrspace->conf.port.thread_num);
    dapp_module_lcore_init(DAPP_MODULE_FLOWS, usrspace->conf.flows.thread_num);
    dapp_module_lcore_init(DAPP_MODULE_PROTOCOL, usrspace->conf.protocol.thread_num);
    dapp_module_lcore_init(DAPP_MODULE_RULE, usrspace->conf.rule.thread_num);

    usrspace->lcore_mask = dapp_modules_total_lcore_mask_get();

    return DAPP_OK;
}

static char eal_args[DPDK_ARG_NUM][DPDK_ARG_SIZE];

int dpdk_args_parse_callback(int *argc, char *argv[], void *arg)
{
    if (!argc || !argv || !arg) {
        return DAPP_ERR_PARAM;
    }

    int narg = 0;
    dapp_usrspace_t *us = arg;

    /* program */
    argv[narg] = eal_args[narg];
    snprintf(argv[narg++], DPDK_ARG_SIZE, "%s", us->program);

    /*
     * master process
     */
    argv[narg] = eal_args[narg];
    snprintf(argv[narg++], DPDK_ARG_SIZE, "%s", "--proc-type=primary");

    /*
     * lcore
     */
    argv[narg] = eal_args[narg];
    snprintf(argv[narg++], DPDK_ARG_SIZE, "-c%x", us->lcore_mask);

    *argc = narg;

    /*
     * debug
     */
    int i = 0;
    for (i = 0; i < *argc; ++i) {
        DAPP_TRACE("argv[%d] = %s\n", i, argv[i]);
    }
    
    return 0;
}

static int dapp_work(__attribute__((unused)) void *arg)
{
    int i = 0;

    unsigned lcore_id;
    lcore_id = dpdk_lcore_id();

    dapp_module_t *module = NULL;

    module = dapp_module_get_by_lcore(lcore_id);

    if (!module) {
        printf("invalid lcore id\n");
        return -1;
    }

    printf("dapp lcore(%d) is running! module %s\n", lcore_id, module->reg.name);
    
    return 0;
}

int main(int argc, char *argv[])
{
    STATUS ret = DAPP_OK;

    dapp_usrspace_t usrspace;
    memset(&usrspace, 0, sizeof(usrspace));

    /*
     * Parse command line parameters
     */
    if (DAPP_OK != (ret = dapp_usrspace_init(argc, argv, &usrspace))) {
        printf("dapp args parse fail\n");
        return ret;
    }

    dpdk_init(&usrspace, dpdk_args_parse_callback);

    dpdk_run(dapp_work, NULL);

    dpdk_exit();

    return DAPP_OK;
}
