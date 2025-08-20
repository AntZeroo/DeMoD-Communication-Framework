#include <dcf_sdk/dcf_plugin_manager.h>
#include <stdio.h>

int main() {
    PluginManager* manager = plugin_manager_new();
    if (!manager) {
        printf("Plugin manager creation failed\n");
        return 1;
    }
    DCFError err = plugin_manager_load(manager, "libcustom_transport.so");
    if (err != DCF_SUCCESS) {
        printf("Plugin load failed: %s\n", dcf_error_str(err));
        plugin_manager_free(manager);
        return 1;
    }
    ITransport* transport = plugin_manager_get_transport(manager);
    if (!transport || !transport->setup(transport, "localhost", 50051)) {
        printf("Transport setup failed\n");
        plugin_manager_free(manager);
        return 1;
    }
    plugin_manager_free(manager);
    printf("Plugin tests passed\n");
    return 0;
}
