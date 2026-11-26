#ifndef PSBBV_NODE_REGISTRY_H
#define PSBBV_NODE_REGISTRY_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PSBBV_NODE_API_VERSION 1u
#define PSBBV_NODE_REGISTRY_CAPACITY 32u
#define PSBBV_NODE_ID_CAPACITY 48u
#define PSBBV_NODE_NAME_CAPACITY 64u

typedef enum {
    PSBBV_NODE_OK = 0,
    PSBBV_NODE_INVALID_ARGUMENT,
    PSBBV_NODE_UNSUPPORTED_API,
    PSBBV_NODE_DUPLICATE_ID,
    PSBBV_NODE_REGISTRY_FULL,
    PSBBV_NODE_NOT_FOUND
} psbbv_node_result;

typedef struct {
    char id[PSBBV_NODE_ID_CAPACITY];
    char display_name[PSBBV_NODE_NAME_CAPACITY];
    uint32_t api_version;
    uint64_t capabilities;
    int enabled;
} psbbv_node;

typedef struct {
    psbbv_node nodes[PSBBV_NODE_REGISTRY_CAPACITY];
    size_t count;
} psbbv_node_registry;

void psbbv_node_registry_init(psbbv_node_registry *registry);
psbbv_node_result psbbv_node_registry_add(
    psbbv_node_registry *registry,
    const char *id,
    const char *display_name,
    uint32_t api_version,
    uint64_t capabilities);
const psbbv_node *psbbv_node_registry_find(
    const psbbv_node_registry *registry,
    const char *id);
psbbv_node_result psbbv_node_registry_remove(psbbv_node_registry *registry, const char *id);
psbbv_node_result psbbv_node_registry_set_enabled(
    psbbv_node_registry *registry,
    const char *id,
    int enabled);

#ifdef __cplusplus
}
#endif
#endif
