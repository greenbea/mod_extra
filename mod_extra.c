#include <switch.h>

SWITCH_MODULE_LOAD_FUNCTION(mod_extra_load);
SWITCH_MODULE_SHUTDOWN_FUNCTION(mod_extra_shutdown);
SWITCH_MODULE_DEFINITION(mod_extra, mod_extra_load, mod_extra_shutdown, NULL);

static struct {
	switch_memory_pool_t *pool;
} globals;


static switch_status_t filter_recv_dtmf_hook(switch_core_session_t *session, const switch_dtmf_t *dtmf, switch_dtmf_direction_t direction)
{
    switch_channel_t *channel = switch_core_session_get_channel(session);
    char *filter = switch_channel_get_private(channel, "__filter_dtmf__");

    if (!zstr(filter)) {
        if (strchr(filter, dtmf->digit)) {
            switch_log_printf(SWITCH_CHANNEL_SESSION_LOG(session), SWITCH_LOG_ERROR, "Dropping received DTMF %c\n", dtmf->digit);
            return SWITCH_STATUS_FALSE;
        }
    }
    
    return SWITCH_STATUS_NOUNLOAD;
}

SWITCH_STANDARD_APP(filter_dtmf)
{
    switch_channel_t *channel = switch_core_session_get_channel(session);
    char *filter = switch_channel_get_private(channel, "__filter_dtmf__");

    if (!filter) {
        if (zstr(data)) {
            switch_log_printf(SWITCH_CHANNEL_SESSION_LOG(session), SWITCH_LOG_ERROR, "Missing required arguments\n");

            return;
        }

        switch_channel_set_private(channel, "__filter_dtmf__", strdup(data));
        switch_core_event_hook_add_recv_dtmf(session, filter_recv_dtmf_hook);
    }
}


SWITCH_MODULE_LOAD_FUNCTION(mod_extra_load)
{
    switch_application_interface_t *app_interface;

    memset(&globals, 0, sizeof(globals));
	globals.pool = pool;

    *module_interface = switch_loadable_module_create_module_interface(pool, modname);

    SWITCH_ADD_APP(app_interface, "filter_dtmf", "filter_dtmf", NULL, filter_dtmf, NULL, SAF_NONE);
    
    return SWITCH_STATUS_SUCCESS;
}


SWITCH_MODULE_SHUTDOWN_FUNCTION(mod_extra_shutdown)
{    
    return SWITCH_STATUS_SUCCESS;
}
