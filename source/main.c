#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <assert.h>
#include <signal.h>

#include "common.h"
#include "dpdk.h"
#include "dapp_code.h"
#include "modules.h"

#define DAPP_DFT_CONFIG_PATH "/home/dapp/log"
#define DAPP_CONF_FILE "startup.conf"
#define DAPP_RULE_FILE "rule.conf"

#define CONF_FILE_NAME_SIZE (256)

#define TRUE    (1)
#define FALSE   (0)

typedef struct
{
    char conf_file[CONF_FILE_NAME_SIZE];
    char rule_file[CONF_FILE_NAME_SIZE];
} dapp_argsopt_t;

typedef struct 
{
    char program[128];
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

static STATUS dapp_args_paser(int argc, char **argv, dapp_argsopt_t *argsopt)
{
    PTR_CHECK(argv);
    PTR_CHECK(argsopt);

    struct option long_options[] = {
        {"config",      1,  0,  's'},
        {"rule",        1,  0,  'r'},
        {"version",     0,  0,  'v'},
        {"help",        0,  0,  'h'},
        {0,             0,  0,  0}
    };

    int opt = 0;

    /*
     * Parse command line parameters
     */
    while (-1 != (opt = getopt_long(argc, argv, "c:r:vh", long_options, NULL))) {
        switch (opt) {
            case 'c' :
                snprintf(argsopt->conf_file, sizeof(argsopt->conf_file), "%s", optarg);
                break;
            case 'r' :
                snprintf(argsopt->rule_file, sizeof(argsopt->rule_file), "%s", optarg);
                break;
            case 'v' :
                printf("\n"
                       " DAPP version : %s\n"
                       "\n",
                       DAPP_BUILD_VERSION);
                exit(0);
            case 'h' :
                printf("%s OPTIONS :\n"
                       "    -c, --config, set startup configuration file\n"
                       "    -r, --rule,   set rule configuration file\n"
                       "    -v, --version,show version\n"
                       "    -h, --help,   show optoins\n", argv[0]);
                exit(0);
            default :
                printf("invalid optoin!\n");
                printf("%s OPTIONS :\n"
                       "    -c, --config, set startup configuration file\n"
                       "    -r, --rule,   set rule configuration file\n"
                       "    -v, --version,show version\n"
                       "    -h, --help,   show optoins\n", argv[0]);
                exit(1);
        }
    }

    return DAPP_OK;
}

static STATUS dapp_user_init(int argc, char **argv, dapp_usrspace_t *usrspace)
{
    PTR_CHECK(argv);
    PTR_CHECK(usrspace);
    
    STATUS ret = DAPP_OK;
    dapp_argsopt_t argsopt = { 0 };

    /*
     * Set default parameters
     */
#ifdef DAPP_CONFIG_PATH
    snprintf(argsopt.conf_file, sizeof(argsopt.conf_file), "%s/%s", DAPP_CONFIG_PATH, DAPP_CONF_FILE);
    snprintf(argsopt.rule_file, sizeof(argsopt.rule_file), "%s/%s", DAPP_CONFIG_PATH, DAPP_RULE_FILE);
#else
    snprintf(argsopt.conf_file, sizeof(argsopt.conf_file), "%s/%s", DAPP_DFT_CONFIG_PATH, DAPP_CONF_FILE);
    snprintf(argsopt.rule_file, sizeof(argsopt.rule_file), "%s/%s", DAPP_DFT_CONFIG_PATH, DAPP_RULE_FILE);
#endif

    /*
     * paser args
     */
    if (DAPP_OK != (ret = dapp_args_paser(argc, argv, &argsopt)))
    {
        printf("ERROR : dapp args paser fail\n");
        return ret;
    }

    /*
     * program 
     */
    snprintf(usrspace->program, sizeof(usrspace->program), "%s", argv[0]);

    printf("working config : %s\n", argsopt.conf_file);
    
    /*
     * resolve startup configuration
     */
    if (DAPP_OK != (ret = DAPP_MODL_CONF_MACHINE(argsopt.conf_file, NULL))) {
        printf("ERROR : dapp conf parse fail\n");
        return ret;
    }

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
    snprintf(argv[narg++], DPDK_ARG_SIZE, "-c%llx", us->lcore_mask);

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
    unsigned lcore_id;
    lcore_id = dpdk_lcore_id();

    DAPP_MODULES_TYPE type;

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
