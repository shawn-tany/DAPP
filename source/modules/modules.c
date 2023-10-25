#include <stdio.h>
#include <stdarg.h>

#include "modules.h"
#include "dapp_code.h"

static DAPP_MODULES_TABLE MODULES;

#define DAPP_MODULE_WAIT_TIMES (15)

#define DAPP_MODULE_INIT_WAIT(init_type, wait_type) {                               \
                                                                                    \
    UINT32_T times = 0;                                                             \
                                                                                    \
    while (DAPP_MODULE_INIT_OK != MODULES.module[wait_type].rely.init_status) {     \
                                                                                    \
        if (DAPP_MODULE_INIT_FAIL == MODULES.module[wait_type].rely.init_status) {  \
            DAPP_MODULE_INIT_STATUS_SET(init_type, DAPP_MODULE_INIT_FAIL);          \
            return DAPP_FAIL;                                                       \
        }                                                                           \
                                                                                    \
        sleep(1);                                                                   \
                                                                                    \
        if (++times > DAPP_MODULE_WAIT_TIMES) {                                     \
            return DAPP_ERR_MODL_TIMEOUT;                                           \
        }                                                                           \
    }                                                                               \
}

#define DAPP_MODULE_INIT_STATUS_SET(type, status) {     \
    MODULES.module[type].rely.init_status = status;     \
}                                                       \

char *dapp_modules_name_get_by_type(DAPP_MODULES_TYPE type)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return "unknow";
    }

    return MODULES.module[type].reg.name;
}

UINT64_T dapp_modules_total_lcore_mask_get(void)
{
    return MODULES.lcore_mask;
}

UINT64_T dapp_module_lcore_mask_get(DAPP_MODULES_TYPE type)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return 0;
    }

    return MODULES.module[type].lcore.lcore_mask;
}

UINT16_T dapp_module_lcore_num_get_by_type(DAPP_MODULES_TYPE type)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return 0;
    }

    return MODULES.module[type].lcore.lcore_num;
}

DAPP_MODULES_TYPE dapp_module_type_get_by_lcore(UINT64_T lcore)
{
    int i = 0;

    for (i = 0; i < MODULES_ITEM(MODULES.module); ++i) {
        if (DAPP_MASK_TST(MODULES.module[i].lcore.lcore_mask, lcore)) {
            return i;
        }
    }

    return DAPP_MODULE_TYPE_NUM;
}

void dapp_module_reg(DAPP_MODULES_TYPE type, const char *name, DAPP_MODULE_OPS *module_ops)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return ;
    }

    /*
     * initialize basic information of module
     */
    snprintf(MODULES.module[type].reg.name, sizeof(MODULES.module[type].reg.name), "%s", name);
    MODULES.module[type].reg.module_ops = module_ops;

    /*
     * Set module registration flag
     */
    MODULES.module[type].reg.reg = 1;
}

void dapp_module_unreg(DAPP_MODULES_TYPE type)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return ;
    }

    /*
     * Cancel module registration flag
     */
    MODULES.module[type].reg.reg = 0;
}

void dapp_module_lcore_init(DAPP_MODULES_TYPE type, UINT16_T lcore_num)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return ;
    }

    if (!MODULES.module[type].reg.reg) {
        return ;
    }

    int i = 0;

    for (i = 0; i < lcore_num; ++i) {
        /*
         * Initialize Module lcore Mask
         */
        DAPP_MASK_SET(MODULES.module[type].lcore.lcore_mask, (MODULES.lcore_num + i));
        /*
         * Initialize program lcore Mask
         */
        DAPP_MASK_SET(MODULES.lcore_mask, (MODULES.lcore_num + i));
    }

    /*
     * Set module running flag
     */
    MODULES.module[type].lcore.running = 1;
    MODULES.module[type].lcore.lcore_num = lcore_num;
    MODULES.lcore_num += lcore_num;
}

void dapp_module_lcore_uninit(DAPP_MODULES_TYPE type)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return ;
    }

    if (!MODULES.module[type].reg.reg) {
        return ;
    }

    /*
     * Cancel module running flag
     */
    MODULES.module[type].lcore.running = 0;
}

void dapp_module_rely_init(DAPP_MODULES_TYPE type, UINT8_T multi_init, int rely_num, ...)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return ;
    }

    if (!MODULES.module[type].reg.reg) {
        return ;
    }

    va_list valist;

    va_start(valist, rely_num);

    DAPP_MODULES_TYPE rely_type;

    int i;

    for (i = 0; i < rely_num; ++i) {

        rely_type = va_arg(valist, DAPP_MODULES_TYPE);

        if (DAPP_MODULE_TYPE_NUM <= rely_type) {
            continue;
        }

        DAPP_MASK_SET(MODULES.module[type].rely.rely_mask, rely_type);
        MODULES.module[type].rely.rely_num++;
    }

    if (MODULES.module[type].rely.rely_num != rely_num) {
        printf("WARNING : exist unknow module type\n");
    }

    va_end(valist);

    MODULES.module[type].rely.multi_init = multi_init;
}

void dapp_module_rely_uninit(DAPP_MODULES_TYPE type)
{
    if (DAPP_MODULE_TYPE_NUM <= type) {
        return ;
    }

    if (!MODULES.module[type].reg.reg) {
        return ;
    }

    /*
     * Cancel module relevace
     */
    MODULES.module[type].rely.rely_mask = 0;
    MODULES.module[type].rely.rely_num = 0;
    MODULES.module[type].rely.multi_init = 0;
}

STATUS DAPP_MODL_CONF_MACHINE(const char *file_name, void *arg)
{
    STATUS ret = DAPP_OK;
    int i = 0;

    if (DAPP_OK != (ret = dapp_conf_init(file_name)))
    {
        return ret;
    }

    for (i = 0; i < DAPP_MODULE_TYPE_NUM; ++i)
    {
        if (MODULES.module[i].reg.module_ops->conf)
        {
            if (DAPP_OK != (ret = MODULES.module[i].reg.module_ops->conf(arg)))
            {
                goto DAPP_MODL_CONF_FAIL;
            }
        }
    }

DAPP_MODL_CONF_FAIL : 

    /* release config jasson root */
    dapp_conf_uinit();

    return ret;
}

STATUS DAPP_MODL_TASK_MACHINE(UINT16_T lcore)
{
    return DAPP_OK;
}

STATUS DAPP_MODL_INIT_MACHINE(DAPP_MODULES_TYPE init_type, void *arg)
{
    int ret;

    if (DAPP_MODULE_TYPE_NUM <= init_type) {
        return DAPP_ERR_MODL_TYPE;
    }

    if (!MODULES.module[init_type].reg.reg) {
        return DAPP_ERR_MODL_UNREG;
    }
    
    if (!MODULES.module[init_type].reg.module_ops->init) {
        return DAPP_ERR_MODL_INIT_NULL;
    }

    /*
     * initialized once
     */
    if (!MODULES.module[init_type].rely.multi_init) {
        ret = dapp_atomic32_cmpset(&MODULES.module[init_type].rely.has_initd, 0, 1);

        if (ret == 0) {
            DAPP_TRACE("port module only need to be initialized once!\n");
            return DAPP_OK;
        }
    }

    int i;

    for (i = 0; i < DAPP_MODULE_TYPE_NUM; ++i) {
        if (DAPP_MASK_TST(MODULES.module[init_type].rely.rely_mask, i)) {
            DAPP_MODULE_INIT_WAIT(init_type, i);
        }
    }

    DAPP_MODULE_INIT_STATUS_SET(init_type, DAPP_MODULE_INIT_START);

    ret = MODULES.module[init_type].reg.module_ops->init(arg);

    if (DAPP_OK != ret) {
        DAPP_MODULE_INIT_STATUS_SET(init_type, DAPP_MODULE_INIT_FAIL);
        return ret;
    }

    DAPP_MODULE_INIT_STATUS_SET(init_type, DAPP_MODULE_INIT_OK);

    return DAPP_OK;
}

STATUS DAPP_MODL_EXEC_MACHINE(DAPP_MODULES_TYPE exec_type, void *arg)
{
    int ret;

    if (DAPP_MODULE_TYPE_NUM <= exec_type) {
        return DAPP_ERR_MODL_TYPE;
    }

    if (!MODULES.module[exec_type].reg.reg) {
        return DAPP_ERR_MODL_UNREG;
    }
    
    if (!MODULES.module[exec_type].reg.module_ops->exec) {
        return DAPP_ERR_MODL_EXEC_NULL;
    }

    int i;

    DAPP_MODULE_INIT_WAIT(exec_type, exec_type);
    
    for (i = 0; i < DAPP_MODULE_TYPE_NUM; ++i) {
        if (DAPP_MASK_TST(MODULES.module[exec_type].rely.rely_mask, i)) {
            DAPP_MODULE_INIT_WAIT(exec_type, i);
        }
    }

    ret = MODULES.module[exec_type].reg.module_ops->exec(&MODULES.module[exec_type].lcore.running, arg);

    if (DAPP_OK != ret) {
        return ret;
    }

    return DAPP_OK;
}

STATUS DAPP_MODL_EXIT_MACHINE(DAPP_MODULES_TYPE exit_type, void *arg)
{
    int ret;

    if (DAPP_MODULE_TYPE_NUM <= exit_type) {
        return DAPP_ERR_MODL_TYPE;
    }

    if (!MODULES.module[exit_type].reg.reg) {
        return DAPP_ERR_MODL_UNREG;
    }

    if (!MODULES.module[exit_type].reg.module_ops->exit) {
        return DAPP_ERR_MODL_EXIT_NULL;
    }

    ret = MODULES.module[exit_type].reg.module_ops->exit(arg);

    if (DAPP_OK != ret) {
        return ret;
    }

    return DAPP_OK;
}

