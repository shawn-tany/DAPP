#include <stdio.h>
#include <string.h>
#include "config.h"

static json_t *config_root = NULL;

STATUS dapp_conf_init(const char *file_name)
{
    PTR_CHECK(file_name);

    json_error_t error;

    /*
     * Load json objects from a file
     */
    if (!(config_root = json_load_file(file_name, 0, &error))) {
        DAPP_TRACE("json error   :\n"
                   "    column   : %d\n"
                   "    line     : %d\n"
                   "    porition : %d\n"
                   "    source   : %s\n"
                   "    text     : %s\n", 
                   error.column, error.line, 
                   error.position, error.source, error.text);
        return DAPP_ERR_JSON_FMT;
    }

    if (!json_is_array(config_root)) {
        return DAPP_ERR_JSON_FMT;
    }

    return DAPP_OK;
}

void dapp_conf_uinit(void)
{
    /*
     * Release the json object before exiting
     */
    json_decref(config_root);
}

json_t *dapp_conf_root_get(void)
{
    return config_root;
}
