#include "modules.h"
#include "dapp_code.h"

static dapp_modules_t MODULES;

char *dapp_module_name_get(dapp_modules_type_t type)
{
    int i = 0;

    if (DAPP_MODULES_TYPE_NUM <= type) {
        return "unknow";
    }

    return MODULES.module[type].reg.name;
}

void dapp_module_reg(dapp_modules_type_t type, 
                         const char *name, 
                         dapp_module_init init, 
                         dapp_module_exec exec, 
                         dapp_module_exit exit)
{
    if (DAPP_MODULES_TYPE_NUM <= type) {
        return ;
    }

    /*
     * initialize basic information of module
     */
    MODULES.module[type].reg.name = name;
    MODULES.module[type].reg.init = init;
    MODULES.module[type].reg.exec = exec;
    MODULES.module[type].reg.exit = exit;

    /*
     * Set module registration flag
     */
    MODULES.module[type].reg.reg = 1;
}

void dapp_module_unreg(dapp_modules_type_t type)
{
    if (DAPP_MODULES_TYPE_NUM <= type) {
        return ;
    }

    /*
     * Cancel module registration flag
     */
    MODULES.module[type].reg.reg = 0;
}

void dapp_module_lcore_init(dapp_modules_type_t type, uint16_t lcore_num)
{
    if (DAPP_MODULES_TYPE_NUM <= type) {
        return ;
    }

    int i = 0;
    uint64_t mask = 0;

    for (i = 0; i < lcore_num; ++i) {
        /*
         * Initialize Module lcore Mask
         */
        DAPP_MASK_SET(MODULES.module[type].lcore.lcore_mask, MODULES.lcore_num + i);
        /*
         * Initialize program lcore Mask
         */
        DAPP_MASK_SET(MODULES.lcore_mask, MODULES.lcore_num + i);
    }

    /*
     * Set module running flag
     */
    MODULES.module[type].lcore.running = 1;
    
    MODULES.lcore_num += lcore_num;
}

void dapp_module_lcore_uninit(dapp_modules_type_t type)
{
    if (DAPP_MODULES_TYPE_NUM <= type) {
        return ;
    }

    /*
     * Cancel module running flag
     */
    MODULES.module[type].lcore.running = 0;
}

void dapp_module_ws_init(dapp_modules_type_t type, void *data, uint32_t size)
{
    if (DAPP_MODULES_TYPE_NUM <= type) {
        return ;
    }

    /*
     * Add module workspace data
     */
    MODULES.module[type].ws.work_data = data;
    MODULES.module[type].ws.work_size = size;
}

void dapp_module_ws_uninit(dapp_modules_type_t type)
{
    if (DAPP_MODULES_TYPE_NUM <= type) {
        return ;
    }

    /*
     * Delete module workspace data
     */
    MODULES.module[type].ws.work_data = NULL;
    MODULES.module[type].ws.work_size = 0;
}