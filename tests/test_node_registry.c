#include "psbbv/node_registry.h"

#include <stdio.h>
#include <string.h>

#define CHECK(expression) do { \\
    if (!(expression)) { \\
        fprintf(stderr, "CHECK failed at %s:%d: %s\\n", __FILE__, __LINE__, #expression); \\
        return 1; \\
    } \\
} while (0)

int main(void) {
    psbbv_node_registry registry;
    const psbbv_node *node;
    size_t i;
    char id[PSBBV_NODE_ID_CAPACITY];

    psbbv_node_registry_init(&registry);
    CHECK(registry.count == 0);
    CHECK(psbbv_node_registry_add(&registry, "media", "Media", 1, 0x01) == PSBBV_NODE_OK);
    CHECK(psbbv_node_registry_add(&registry, "games", "Games", 1, 0x02) == PSBBV_NODE_OK);
    CHECK(psbbv_node_registry_add(&registry, "media", "Duplicate", 1, 0) == PSBBV_NODE_DUPLICATE_ID);
    CHECK(psbbv_node_registry_add(&registry, "Bad_ID", "Bad", 1, 0) == PSBBV_NODE_INVALID_ARGUMENT);
    CHECK(psbbv_node_registry_add(&registry, "future", "Future", 99, 0) == PSBBV_NODE_UNSUPPORTED_API);
    node = psbbv_node_registry_find(&registry, "media");
    CHECK(node != NULL && strcmp(node->display_name, "Media") == 0 && node->enabled == 1);
    CHECK(psbbv_node_registry_set_enabled(&registry, "media", 0) == PSBBV_NODE_OK);
    CHECK(psbbv_node_registry_find(&registry, "media")->enabled == 0);
    CHECK(psbbv_node_registry_set_enabled(&registry, "missing", 1) == PSBBV_NODE_INVALID_ARGUMENT);

    for (i = registry.count; i < PSBBV_NODE_REGISTRY_CAPACITY; ++i) {
        (void)snprintf(id, sizeof(id), "node-%zu", i);
        CHECK(psbbv_node_registry_add(&registry, id, "Test Node", 1, 0) == PSBBV_NODE_OK);
    }
    CHECK(psbbv_node_registry_add(&registry, "overflow", "Overflow", 1, 0) == PSBBV_NODE_REGISTRY_FULL);
    puts("PSBBV node registry tests passed");
    return 0;
}
