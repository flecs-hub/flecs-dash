#ifndef FLECS_DASH_MONITOR_H
#define FLECS_DASH_MONITOR_H

/* This generated file contains includes for project dependencies */
#include "bake_config.h"

#ifdef __cplusplus
extern "C" {
#endif

ECS_STRUCT(EcsFps, {
    double value;
    double count;
    double target;
});

ECS_STRUCT(EcsTick, {
    double count;
});

ECS_STRUCT(EcsLoad, {
    double total_time_count;
    double frame_time_count;
    double system_time_count;
    double merge_time_count;
});

ECS_STRUCT(EcsWorldClock, {
    double world_time;
    double world_time_raw;
});

typedef struct FlecsDashMonitor {
    ECS_DECLARE_COMPONENT(EcsFps);
} FlecsDashMonitor;

FLECS_DASH_EXPORT
void FlecsDashMonitorImport(
    ecs_world_t *world,
    int flags);

#define FlecsDashMonitorImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, EcsFps);

#ifdef __cplusplus
}
#endif

#endif
