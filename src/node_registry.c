#include "psbbv/node_registry.h"

#include <string.h>

static int valid_id(const char *id) {
    size_t i;
    if (id == NULL || id[0] == '\0') return 0;
    for (i = 0; id[i] != '\0'; ++i) {
        const char c = id[i];
        if (i >= PSBBV_NODE_ID_CAPACITY - 1) return 0;
        if (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-')) return 0;
        if (c == '-' && (i == 0 || id[i - 1] == '-')) return 0;
    }
    return id[i - 1] != '-';
}

static int copy_text(char *dst, size_t capacity, const char *src) {
    size_t length;
    if (src == NULL || src[0] == '\0') return 0;
    length = strlen(src);
    if (length >= capacity) return 0;
    memcpy(dst, src, length + 1);
    return 1;
}

void psbbv_node_registry_init(psbbv_node_registry *registry) {
    if (registry != NULL) {
        memset(registry, 0, sizeof(*registry));
    }
}

const psbbv_node *psbbv_node_registry_find(
    const psbbv_node_registry *registry,
    const char *id) {
    size_t i;
    if (registry == NULL || id == NULL) return NULL;
    for (i = 0; i < registry->count; ++i) {
        if (strcmp(registry->nodes[i].id, id) == 0) return &registry->nodes[i];
    }
    return NULL;
}

psbbv_node_result psbbv_node_registry_add(
    psbbv_node_registry *registry,
    const char *id,
    const char *display_name,
    uint32_t api_version,
    uint64_t capabilities) {
    psbbv_node candidate;
    if (registry == NULL || !valid_id(id) ||
        display_name == NULL || display_name[0] == '\0') {
        return PSBBV_NODE_INVALID_ARGUMENT;
    }
    if (api_version != PSBBV_NODE_API_VERSION) return PSBBV_NODE_UNSUPPORTED_API;
    if (psbbv_node_registry_find(registry, id) != NULL) return PSBBV_NODE_DUPLICATE_ID;
    if (registry->count >= PSBBV_NODE_REGISTRY_CAPACITY) return PSBBV_NODE_REGISTRY_FULL;
    memset(&candidate, 0, sizeof(candidate));
    if (!copy_text(candidate.id, sizeof(candidate.id), id) ||
        !copy_text(candidate.display_name, sizeof(candidate.display_name), display_name)) {
        return PSBBV_NODE_INVALID_ARGUMENT;
    }
    candidate.api_version = api_version;
    candidate.capabilities = capabilities;
    candidate.enabled = 1;
    registry->nodes[registry->count++] = candidate;
    return PSBBV_NODE_OK;
}

psbbv_node_result psbbv_node_registry_set_enabled(
    psbbv_node_registry *registry,
    const char *id,
    int enabled) {
    size_t i;
    if (registry == NULL || id == NULL) return PSBBV_NODE_INVALID_ARGUMENT;
    for (i = 0; i < registry->count; ++i) {
        if (strcmp(registry->nodes[i].id, id) == 0) {
            registry->nodes[i].enabled = enabled ? 1 : 0;
            return PSBBV_NODE_OK;
        }
    }
    return PSBBV_NODE_INVALID_ARGUMENT;
}
