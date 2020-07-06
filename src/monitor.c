
#include "flecs_dash.h"

static
void EcsUpdateFps(ecs_iter_t *it) {
    ecs_world_t *world = it->world;

    EcsFps *world_fps = ecs_column(it, EcsFps, 1);

    const ecs_world_info_t *info = ecs_get_world_info(world);
    world_fps->value = 1.0 / info->delta_time_raw;
    world_fps->count += world_fps->value;
    world_fps->target = info->target_fps;
}

static
void EcsUpdateLoad(ecs_iter_t *it) {
    ecs_world_t *world = it->world;

    EcsLoad *world_load = ecs_column(it, EcsLoad, 1);

    const ecs_world_info_t *info = ecs_get_world_info(world);

    world_load->total_time_count += info->delta_time_raw;
    world_load->frame_time_count = info->frame_time_total;
    world_load->system_time_count = info->system_time_total;
    world_load->merge_time_count = info->merge_time_total;
}

static
void EcsUpdateTick(ecs_iter_t *it) {
    ecs_world_t *world = it->world;

    EcsTick *world_tick = ecs_column(it, EcsTick, 1);

    const ecs_world_info_t *info = ecs_get_world_info(world);
    world_tick->count = info->frame_count_total;
}

static
void EcsUpdateClock(ecs_iter_t *it) {
    ecs_world_t *world = it->world;

    EcsWorldClock *world_clock = ecs_column(it, EcsWorldClock, 1);

    const ecs_world_info_t *info = ecs_get_world_info(world);
    world_clock->world_time = info->world_time_total;
    world_clock->world_time_raw = info->world_time_total_raw;
}

void FlecsDashMonitorImport(
    ecs_world_t *world,
    int flags)
{
    ECS_MODULE(world, FlecsDashMonitor);

    ECS_IMPORT(world, FlecsMeta, 0);

    ecs_set_name_prefix(world, "Ecs");

    ECS_META(world, EcsFps);
    ECS_META(world, EcsLoad);
    ECS_META(world, EcsTick);
    ECS_META(world, EcsWorldClock);

    /* System for keeping track of world stats */
    ecs_set(world, EcsWorld, EcsFps, {0});
    ECS_SYSTEM(world, EcsUpdateFps, EcsOnLoad, flecs.core.World:Fps);   

    ecs_set(world, EcsWorld, EcsLoad, {0});
    ECS_SYSTEM(world, EcsUpdateLoad, EcsOnLoad, flecs.core.World:Load); 

    ecs_set(world, EcsWorld, EcsTick, {0});
    ECS_SYSTEM(world, EcsUpdateTick, EcsOnLoad, flecs.core.World:Tick);  

    ecs_set(world, EcsWorld, EcsWorldClock, {0});
    ECS_SYSTEM(world, EcsUpdateClock, EcsOnLoad, flecs.core.World:WorldClock);
}
