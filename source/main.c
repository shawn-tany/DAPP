#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <assert.h>
#include <signal.h>

#include "common.h"
#include "startup_conf.h"
#include "dpdk.h"
#include "dapp_code.h"
#include "modules.h"

#define DAPP_DEFAULT_INSTALL_PATH "/home/dapp"
#define DAPP_CONF_FILE "startup.conf"
#define DAPP_RULE_FILE "rule.conf"

#define CONF_FILE_NAME_SIZE (256)

#define TRUE    (1)
#define FALSE   (0)

typedef struct 
{
    char program[128];
    dapp_conf_t conf;
    UINT64_T lcore_mask;
} dapp_usrspace_t;

static void signal_handle(int sig)
{
    int i;

    /*
     * Stop all modules
     */
    for (i = 0; i < DAPP_MODULE_TYPE_NUM; ++i) {
        dapp_module_lcore_uninit(i);
    }
}

static STATUS dapp_user_init(int argc, char **argv, dapp_usrspace_t *usrspace)
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
#ifdef DAPP_CONFIG_PATH
    snprintf(conf_file, sizeof(conf_file), "%s/%s", DAPP_CONFIG_PATH, DAPP_CONF_FILE);
    snprintf(rule_file, sizeof(rule_file), "%s/%s", DAPP_CONFIG_PATH, DAPP_RULE_FILE);
#else
    snprintf(conf_file, sizeof(conf_file), "%s/%s", DAPP_DEFAULT_INSTALL_PATH, DAPP_CONF_FILE);
    snprintf(rule_file, sizeof(rule_file), "%s/%s", DAPP_DEFAULT_INSTALL_PATH, DAPP_RULE_FILE);
#endif

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
                exit(0);
            default :
                printf("invalid optoin!\n");
                printf("%s OPTIONS :\n"
                       "    -c, --config, set startup configuration file\n"
                       "    -r, --rule,   set rule configuration file\n"
                       "    -h, --help,   show optoins\n", argv[0]);
                exit(1);
        }
    }

    /*
     * program 
     */
    snprintf(usrspace->program, sizeof(usrspace->program), "%s", argv[0]);

    printf("working config : %s\n", conf_file);
    
    /*
     * resolve startup configuration
     */
    if (DAPP_OK != (ret = dapp_conf_parse(&usrspace->conf, conf_file))) {
        printf("ERROR : dapp conf parse fail\n");
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
    dapp_module_lcore_init(DAPP_MODULE_FILES, usrspace->conf.files.thread_num);

    usrspace->lcore_mask = dapp_modules_total_lcore_mask_get();

    /*
     * Update module relevance
     */
    dapp_module_rely_init(DAPP_MODULE_FLOWS, DAPP_MODULE_UNI_INIT, 1, DAPP_MODULE_PORT);
    dapp_module_rely_init(DAPP_MODULE_PROTOCOL, DAPP_MODULE_UNI_INIT, 1, DAPP_MODULE_FLOWS);
    dapp_module_rely_init(DAPP_MODULE_FILES, DAPP_MODULE_UNI_INIT, 1, DAPP_MODULE_FLOWS);

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

static int dapp_work(void *arg)
{
    int i = 0;

    unsigned lcore_id;
    lcore_id = dpdk_lcore_id();

    dapp_modules_type_t type;

    /*
     * Get module type according to lcore 
     */
    type = dapp_module_type_get_by_lcore(lcore_id);

    if (DAPP_MODULE_TYPE_NUM <= type) {
        DAPP_TRACE("invalid lcore id\n");
        return -1;
    }

    STATUS ret;

    /*
     * Initialize the module through the module initialization machine
     */
    ret = DAPP_MODL_INIT_MACHINE(type, arg);

    if (DAPP_OK != ret) {
        DAPP_TRACE("ERROR : module %d init fail! ERR : %d\n", type, ret);
        return DAPP_FAIL;
    }

    /*
     * The module is executed by the module executor
     */
    ret = DAPP_MODL_EXEC_MACHINE(type, arg);

    if (DAPP_OK != ret) {
        DAPP_TRACE("ERROR : module %d exec fail! ERR : %d\n", type, ret);
       return DAPP_FAIL;
    }

    /*
     * Exit the module through the module exit machine
     */
    ret = DAPP_MODL_EXIT_MACHINE(type, arg);

    if (DAPP_OK != ret) {
        DAPP_TRACE("ERROR : module %d exit fail! ERR : %d\n", type, ret);
        return DAPP_FAIL;
    }
    
    return 0;
}

int main(int argc, char *argv[])
{
    STATUS ret = DAPP_OK;

    dapp_usrspace_t usrspace;
    memset(&usrspace, 0, sizeof(usrspace));

    signal(SIGINT, signal_handle);
    signal(SIGTERM, signal_handle);

    /*
     * Parse command line parameters
     */
    if (DAPP_OK != (ret = dapp_user_init(argc, argv, &usrspace))) {
        DAPP_TRACE("ERROR : dapp args parse fail\n");
        return ret;
    }

    /*
     * Initialize the dpdk with user configuration
     */
    dpdk_init(&usrspace, dpdk_args_parse_callback);

    /*
     * Execute dpdk lcore
     */
    dpdk_run(dapp_work, NULL);

    /*
     * Release dpdk resources
     */
    dpdk_exit();

    return DAPP_OK;
}
