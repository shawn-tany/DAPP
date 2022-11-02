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

} dapp_context_t;

static void dapp_ws_lcore_set(dapp_workspace_t *ws, dapp_modules_type_t type, uint64_t lstart, uint8_t lnum)
{
    int i = 0;

    ws->modules[type].lcore.lcore_start = lstart;
    ws->modules[type].lcore.lcore_end = lstart + lnum - 1;
    ws->modules[type].lcore.lcore_num = lnum;

    ws->modules[type].status.reg = TRUE;
    ws->modules[type].status.running = TRUE;

    for (i = 0; i < lnum; ++i) {
        DAPP_MASK_SET(ws->lcores.lcore_mask, lstart + i);
    }
}

static STATUS dapp_workspace_init(dapp_conf_t *conf, dapp_workspace_t *ws)
{
    uint64_t bit_offset = 0;

    /*
     * lcore
     */
    dapp_ws_lcore_set(ws, DAPP_MODULES_CTRL, bit_offset, 1);
    bit_offset += 1;

    dapp_ws_lcore_set(ws, MODULES_PORT_TRANS, bit_offset, conf->port.thread_num);
    bit_offset += conf->port.thread_num;

    dapp_ws_lcore_set(ws, MODULES_PROTO_IDENTI, bit_offset, conf->proto_identi.thread_num);
    bit_offset += conf->proto_identi.thread_num;

    dapp_ws_lcore_set(ws, MODULES_RULE_MATCH, bit_offset, conf->rule_match.thread_num);
    bit_offset += conf->rule_match.thread_num;

    dapp_ws_lcore_set(ws, MODULES_FLOW_ISOTONIC, bit_offset, conf->flow_iotonic.thread_num);
    bit_offset += conf->flow_iotonic.thread_num;

    printf("lcore mask = %x\n", ws->lcores.lcore_mask);

    return DAPP_OK;
}

static STATUS dapp_context_init(int *argc, char **argv, dapp_context_t *context)
{
    if (!argc || !argv || !context) {
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
    while (-1 != (opt = getopt_long(*argc, argv, "c:r:h", long_options, NULL))) {
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
    snprintf(context->program, sizeof(context->program), "%s", argv[0]);
    
    /*
     * resolve startup configuration
     */
    if (DAPP_OK != (ret = dapp_conf_parse(&context->conf, conf_file))) {
        printf("dapp conf parse fail\n");
        return ret;
    }

    /*
     * debug, show static configuration
     */
    dapp_conf_dump(&context->conf);

    return DAPP_OK;
}

static char eal_args[DPDK_ARG_NUM][DPDK_ARG_SIZE];

int dpdk_args_parse_callback(int *argc, char *argv[], void *arg)
{
    if (!argc || !argv || !arg) {
        return DAPP_ERR_PARAM;
    }

    int narg = 0;
    dapp_context_t *ctx = arg;

    /* program */
    argv[narg] = eal_args[narg];
    snprintf(argv[narg++], DPDK_ARG_SIZE, "%s", ctx->program);

    /*
     * master process
     */
    argv[narg] = eal_args[narg];
    snprintf(argv[narg++], DPDK_ARG_SIZE, "%s", "--proc-type=primary");

    /*
     * lcore
     */
    argv[narg] = eal_args[narg];
    snprintf(argv[narg++], DPDK_ARG_SIZE, "-c%x", ws->lcores.lcore_mask);

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

static int dapp_loop(__attribute__((unused)) void *arg)
{
    int i = 0;

    unsigned lcore_id;
    lcore_id = dpdk_lcore_id();

    dapp_workspace_t *ws = arg;

    for (i = 0; i < ITEM(ws->modules); ++i) {
        if (ws->modules[i].status.reg &&
            lcore_id >= ws->modules[i].lcore.lcore_start &&
            lcore_id <= ws->modules[i].lcore.lcore_end) {

            printf("dapp loop lcore(%u) modules : %s\n", lcore_id, dapp_modules_name_get(i));
        }
    }
    
    return 0;
}

int main(int argc, char *argv[])
{
    STATUS ret = DAPP_OK;

    dapp_context_t context;
    memset(&context, 0, sizeof(context));

    /*
     * Parse command line parameters
     */
    if (DAPP_OK != (ret = dapp_context_init(argc, argv, &context))) {
        printf("dapp args parse fail\n");
        return ret;
    }
    
    dpdk_init(&context, dpdk_args_parse_callback);

    dpdk_run(dapp_loop, NULL);

    dpdk_exit();

    return DAPP_OK;
}
