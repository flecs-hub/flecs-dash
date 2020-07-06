#include <hello_dash.h>

ECS_STRUCT(Position, {
    float x;
    float y;
});

ECS_STRUCT(Velocity, {
    float x;
    float y;
});

void Move(ecs_iter_t *it) {
    Position *p = ecs_column(it, Position, 1);
    Velocity *v = ecs_column(it, Velocity, 2);

    for (int i = 0; i < it->count; i ++) {
        p[i].x += v[i].x * it->delta_time;
        p[i].y += v[i].y * it->delta_time;
    }
}

int main(int argc, char *argv[]) {
    ecs_world_t *world = ecs_init();
    
    /* Import dashboard module */
    ECS_IMPORT(world, FlecsDash, 0);

    /* Import HTTP server. Without this module, the dashboard server entities 
     * will be created, but there will be no HTTP server listening to them. */
    ECS_IMPORT(world, FlecsSystemsCivetweb, 0);

    /* Import the meta package, so application types show up in the dashboard */
    ECS_IMPORT(world, FlecsMeta, 0);

    /* Register components */
    ECS_META(world, Position);
    ECS_META(world, Velocity);

    /* Register system */
    ECS_SYSTEM(world, Move, EcsOnUpdate, Position, Velocity);

    /* Create dash server */
    ecs_set(world, 0, EcsDashServer, {.port = 8080});

    ECS_TYPE(world, Type, Position, Velocity);

    /* Create dummy data */
    ECS_ENTITY(world, E1, 0);
        ecs_set(world, E1, Position, {10, 20});
        ecs_set(world, E1, Velocity, {0, 1});

    ECS_ENTITY(world, E2, 0);
        ecs_set(world, E2, Position, {30, 40});
        ecs_set(world, E2, Velocity, {1, 0});

    ECS_ENTITY(world, E3, 0);
        ecs_set(world, E3, Position, {50, 60});
        ecs_set(world, E3, Velocity, {1, 1});

    ECS_ENTITY(world, E4, 0);
        ecs_set(world, E4, Position, {70, 80});
        ecs_set(world, E4, Velocity, {2, 1});

    ECS_ENTITY(world, E5, 0);
        ecs_set(world, E5, Position, {90, 100});
        ecs_set(world, E5, Velocity, {1, 2});

    ECS_ENTITY(world, E6, 0);
        ecs_set(world, E6, Position, {110, 120});
        ecs_set(world, E6, Velocity, {2, 2});

    /* Run main loop */
    while (ecs_progress(world, 0));

    ecs_fini(world);
}
