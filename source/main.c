#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <assert.h>

#include "common.h"
#include "config.h"
#include "dpdk.h"
#include "dapp_code.h"

#define CONF_FILE "../install/config/startup.conf"
#define RULE_FILE "../install/config/rule.conf"

#define CONF_FILE_NAME_SIZE (256)

#define RUNNING (1)
#define EXIT    (0)

enum 
{
    MODULES_CTRL = 0,
    MODULES_PORT_TRANS,
    MODULES_PROTO_IDENTI,
    MODULES_RULE_MATCH,
    MODULES_FLOW_ISOTONIC,
    MODULES_DATA_RESTORE,
    MODULES_FILE_IDENTI,
    MODULES_TYPE_NUM,
};

typedef struct {
    char *program;

    struct {
        uint64_t lcore_mask;
    } lcores;

    struct {
        struct {
            uint8_t lcore_start;
            uint8_t lcore_end;
            uint8_t lcore_num;
        } lcore;

        struct {
            uint8_t running;
        } status;
        
    } modules[MODULES_TYPE_NUM];
    
} dapp_ctrl_t;

static STATUS dapp_ctrl_init(dapp_conf_t *conf, dapp_ctrl_t *ctrl)
{
    uint64_t bit_offset = 0;
    uint64_t module_mask = 0;

    /*
     * lcore
     */
    ctrl->modules[MODULES_CTRL].lcore.lcore_start = 0;
    ctrl->modules[MODULES_CTRL].lcore.lcore_end = 0;
    ctrl->modules[MODULES_CTRL].lcore.lcore_num = 1;
    ctrl->modules[MODULES_CTRL].status.running = RUNNING;
    DAPP_MASK_SET(ctrl->lcores.lcore_mask, bit_offset);
    bit_offset += 1;
    
    ctrl->modules[MODULES_PORT_TRANS].lcore.lcore_start = bit_offset;
    ctrl->modules[MODULES_PORT_TRANS].lcore.lcore_end = bit_offset + conf->port.thread_num;
    ctrl->modules[MODULES_PORT_TRANS].lcore.lcore_num = conf->port.thread_num;
    ctrl->modules[MODULES_PORT_TRANS].status.running = RUNNING;
    DAPP_MASK_SET(ctrl->lcores.lcore_mask, bit_offset);
    bit_offset += conf->port.thread_num;

    ctrl->modules[MODULES_PROTO_IDENTI].lcore.lcore_start = bit_offset;
    ctrl->modules[MODULES_PROTO_IDENTI].lcore.lcore_end = bit_offset + conf->proto_identi.thread_num;
    ctrl->modules[MODULES_PROTO_IDENTI].lcore.lcore_num = conf->proto_identi.thread_num;
    ctrl->modules[MODULES_PROTO_IDENTI].status.running = RUNNING;
    DAPP_MASK_SET(ctrl->lcores.lcore_mask, bit_offset);
    bit_offset += conf->proto_identi.thread_num;

    ctrl->modules[MODULES_RULE_MATCH].lcore.lcore_start = bit_offset;
    ctrl->modules[MODULES_RULE_MATCH].lcore.lcore_end = bit_offset + conf->rule_match.thread_num;
    ctrl->modules[MODULES_RULE_MATCH].lcore.lcore_num = conf->rule_match.thread_num;
    ctrl->modules[MODULES_RULE_MATCH].status.running = RUNNING;
    DAPP_MASK_SET(ctrl->lcores.lcore_mask, bit_offset);
    bit_offset += conf->rule_match.thread_num;

    ctrl->modules[MODULES_FLOW_ISOTONIC].lcore.lcore_start = bit_offset;
    ctrl->modules[MODULES_FLOW_ISOTONIC].lcore.lcore_end = bit_offset + conf->flow_iotonic.thread_num;
    ctrl->modules[MODULES_FLOW_ISOTONIC].lcore.lcore_num = conf->flow_iotonic.thread_num;
    ctrl->modules[MODULES_FLOW_ISOTONIC].status.running = RUNNING;
    DAPP_MASK_SET(ctrl->lcores.lcore_mask, bit_offset);
    bit_offset += conf->flow_iotonic.thread_num;

    printf("lcore mask = %x\n", ctrl->lcores.lcore_mask);

    return DAPP_OK;
}

static STATUS dapp_args_parse(int argc, char *argv[], dapp_ctrl_t *ctrl)
{
    if (!argv || !ctrl) {
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

    dapp_conf_t conf;
    memset(&conf, 0, sizeof(conf));
    
    /*
     * resolve startup configuration
     */
    if (DAPP_OK != (ret = dapp_conf_parse(&conf, conf_file))) {
        printf("dapp conf parse fail\n");
        return ret;
    }

    /*
     * debug, show static configuration
     */
    dapp_conf_dump(&conf);

    /*
     * Initialize Console
     */
    ctrl->program = argv[0];
    if (DAPP_OK != (ret = dapp_ctrl_init(&conf, ctrl))) {
        printf("dapp ctrl init failed\n");
        return ret;
    }

    return DAPP_OK;
}

static char eal_args[DPDK_ARG_NUM][DPDK_ARG_SIZE];

int dpdk_args_parse_callback(int *argc, char *argv[], void *arg)
{
    if (!argc || !argv || !arg) {
        return DAPP_ERR_PARAM;
    }

    int narg = 0;
    dapp_ctrl_t *ctrl = arg;

    /* program */
    argv[narg] = eal_args[narg];
    snprintf(argv[narg++], DPDK_ARG_SIZE, "%s", ctrl->program);

    /*
     * lcore
     */
    argv[narg] = eal_args[narg];
    snprintf(argv[narg++], DPDK_ARG_SIZE, "-c%x", ctrl->lcores.lcore_mask);

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

static int dapp_business_loop(__attribute__((unused)) void *arg)
{
    unsigned lcore_id;
    lcore_id = dpdk_lcore_id();
    printf("dapp business loop lcore(%u)\n", lcore_id);
    return 0;
}

static int dapp_control_loop(__attribute__((unused)) void *arg)
{
    unsigned lcore_id;
    lcore_id = dpdk_lcore_id();
    printf("dapp control loop lcore(%u)\n", lcore_id);
    return 0;
}

int main(int argc, char *argv[])
{
    STATUS ret = DAPP_OK;

    dapp_ctrl_t ctrl;
    memset(&ctrl, 0, sizeof(ctrl));

    /*
     * Parse command line parameters
     */
    if (DAPP_OK != (ret = dapp_args_parse(argc, argv, &ctrl))) {
        printf("dapp args parse fail\n");
        return ret;
    }
        
    dpdk_init(&ctrl, dpdk_args_parse_callback);

    dpdk_run(dapp_business_loop, NULL, dapp_control_loop, NULL);

    dpdk_exit();

    return DAPP_OK;
}
