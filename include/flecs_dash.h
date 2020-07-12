#ifndef FLECS_DASH_H
#define FLECS_DASH_H

/* This generated file contains includes for project dependencies */
#include "flecs-dash/bake_config.h"
#include "flecs-dash/monitor.h"

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
//// Module implementation
////////////////////////////////////////////////////////////////////////////////

ECS_STRUCT(EcsDashServer, {
    int16_t port;
});

ECS_STRUCT(EcsDashApp, {
    char *path;
    char *icon;
});

typedef struct FlecsDash {
    ECS_DECLARE_COMPONENT(EcsDashServer);
    ECS_DECLARE_COMPONENT(EcsDashApp);
} FlecsDash;

FLECS_DASH_EXPORT
void FlecsDashImport(
    ecs_world_t *world);

#define FlecsDashImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, EcsDashServer);\
    ECS_IMPORT_COMPONENT(handles, EcsDashApp)

#ifdef __cplusplus
}
#endif

#endif
