#include <stdio.h>
#include <string.h>
#include "jansson.h"
#include "config.h"

static STATUS dapp_port_conf_parse(dapp_conf_t *conf, json_t *port)
{
    if (!conf || !port) {
        return DAPP_ERR_PARAM;
    }

    json_t *obj = NULL;
    json_t *sub_obj = NULL;

    if (!(obj = json_object_get(port, "thread_num"))) {
        return DAPP_ERR_JSON_CONF;
    }
    conf->port.thread_num = json_integer_value(obj);

    if (!(obj = json_object_get(port, "mempool"))) {
        return DAPP_ERR_JSON_CONF;
    }

    if (!(sub_obj = json_object_get(obj, "node_size"))) {
        return DAPP_ERR_JSON_CONF;
    }
    conf->port.mempool.node_size = json_integer_value(sub_obj);

    if (!(sub_obj = json_object_get(obj, "node_num"))) {
        return DAPP_ERR_JSON_CONF;
    }
    conf->port.mempool.node_num = json_integer_value(sub_obj);

    if (!(obj = json_object_get(port, "port_list"))) {
        return DAPP_ERR_JSON_CONF;
    }

    if (!json_is_array(obj)) {
        return DAPP_ERR_JSON_FMT;
    }

    uint32_t i = 0;
    uint32_t array_size = json_array_size(obj);

    for (i = 0; i < array_size; ++i) {
        if (!(sub_obj = json_array_get(obj, i))) {
            return DAPP_ERR_JSON_FMT;
        }
        
        memcpy(conf->port.port.ports[i], json_string_value(sub_obj), sizeof(conf->port.port.ports[i]));
        conf->port.port.port_num++;
    }

    return DAPP_OK;
}

static STATUS dapp_flows_conf_parse(dapp_conf_t *conf, json_t *flows)
{
    if (!conf || !flows) {
        return DAPP_ERR_PARAM;
    }

    json_t *obj = NULL;
    json_t *sub_obj = NULL;

    if (!(obj = json_object_get(flows, "thread_num"))) {
        return DAPP_ERR_JSON_CONF;
    }
    conf->flows.thread_num = json_integer_value(obj);

    if (!(obj = json_object_get(flows, "mempool"))) {
        return DAPP_ERR_JSON_CONF;
    }

    if (!(sub_obj = json_object_get(obj, "node_size"))) {
        return DAPP_ERR_JSON_CONF;
    }
    conf->flows.mempool.node_size = json_integer_value(sub_obj);

    if (!(sub_obj = json_object_get(obj, "node_num"))) {
        return DAPP_ERR_JSON_CONF;
    }
    conf->flows.mempool.node_num = json_integer_value(sub_obj);

    if (!(obj = json_object_get(flows, "window"))) {
        return DAPP_ERR_JSON_CONF;
    }
    conf->flows.window = json_integer_value(obj);

    return DAPP_OK;
}

static STATUS dapp_protocol_conf_parse(dapp_conf_t *conf, json_t *protocol)
{
    if (!conf || !protocol) {
        return DAPP_ERR_PARAM;
    }

    json_t *obj = NULL;

    if (!(obj = json_object_get(protocol, "thread_num"))) {
        return DAPP_ERR_JSON_CONF;
    }
    conf->protocol.thread_num = json_integer_value(obj);

    return DAPP_OK;
}

static STATUS dapp_rule_conf_parse(dapp_conf_t *conf, json_t *rule)
{
    if (!conf || !rule) {
        return DAPP_ERR_PARAM;
    }

    json_t *obj = NULL;

    if (!(obj = json_object_get(rule, "thread_num"))) {
        return DAPP_ERR_JSON_CONF;
    }
    conf->rule.thread_num = json_integer_value(obj);

    return DAPP_OK;
}

static STATUS _dapp_conf_parse(dapp_conf_t *conf, json_t *obj_conf)
{
    if (!conf || !obj_conf) {
        return DAPP_ERR_PARAM;
    }

    json_t *obj = NULL;

    /* port parse */
    if (!(obj = json_object_get(obj_conf, "port"))) {
        return DAPP_ERR_JSON_CONF;
    } else if (DAPP_OK != (dapp_port_conf_parse(conf, obj))) {
        return DAPP_ERR_JSON_CONF;
    }

    /* flows parse */
    if (!(obj = json_object_get(obj_conf, "flows"))) {
        return DAPP_ERR_JSON_CONF;
    } else if (DAPP_OK != (dapp_flows_conf_parse(conf, obj))) {
        return DAPP_ERR_JSON_CONF;
    }

    /* protocol parse */
    if (!(obj = json_object_get(obj_conf, "protocol"))) {
        return DAPP_ERR_JSON_CONF;
    } else if (DAPP_OK != (dapp_protocol_conf_parse(conf, obj))) {
        return DAPP_ERR_JSON_CONF;
    }

    /* rule parse */
    if (!(obj = json_object_get(obj_conf, "rule"))) {
        return DAPP_ERR_JSON_CONF;
    } else if (DAPP_OK != (dapp_rule_conf_parse(conf, obj))) {
        return DAPP_ERR_JSON_CONF;
    }

    return DAPP_OK;
}

STATUS dapp_conf_parse(dapp_conf_t *conf, const char *file_name)
{
    if (!conf || !file_name) {
        return DAPP_ERR_PARAM;
    }

    json_t *root = NULL;
    json_error_t error;

    /*
     * Load json objects from a file
     */
    if (!(root = json_load_file(file_name, 0, &error))) {
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

    if (!json_is_array(root)) {
        return DAPP_ERR_JSON_FMT;
    }

    uint32_t i = 0;
    uint32_t array_size = json_array_size(root);
    json_t *conf_obj = NULL;
    STATUS ret = DAPP_OK;

    for (i = 0; i < array_size; ++i) {
        if (!(conf_obj = json_array_get(root, i))) {
            ret = DAPP_ERR_JSON_FMT;
            goto STATIC_CONF_FAIL;
        }

        /*
         * Get the configuration through the json object
         */
        if (DAPP_OK != (ret = _dapp_conf_parse(conf, conf_obj))) {
            goto STATIC_CONF_FAIL;
        }
    }

    /*
     * Release the json object before exiting
     */
STATIC_CONF_FAIL:
    json_decref(root);
    return ret;
}

void dapp_conf_dump(dapp_conf_t *conf)
{
    if (conf) {
        printf("##\n"
               "static config :\n"
               "    port config :\n"
               "        thread num = %d\n"
               "        mempool :\n"
               "            node size = %d\n" 
               "            node num = %d\n" 
               "        port list:\n", 
               conf->port.thread_num, 
               conf->port.mempool.node_size , 
               conf->port.mempool.node_num);
        int i = 0;
        for (i = 0; i < conf->port.port.port_num; ++i) {
        printf("            %s\n", conf->port.port.ports[i]);
        }
        printf("\n");
        printf("    flows config :\n"
               "        thread num = %d\n"
               "        mempool :\n"
               "            node size = %d\n" 
               "            node num = %d\n" 
               "        window = %d\n", 
               conf->flows.thread_num, 
               conf->flows.mempool.node_size, 
               conf->flows.mempool.node_num, 
               conf->flows.window);
        printf("\n");
        printf("    protocol config :\n"
               "        thread num = %d\n", 
               conf->protocol.thread_num);
        printf("\n");
        printf("    rule config :\n"
               "        thread num = %d\n"
               "##\n",
               conf->rule.thread_num);
    }
}

