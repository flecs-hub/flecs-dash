#ifndef FLECS_DASH_H
#define FLECS_DASH_H

/* This generated file contains includes for project dependencies */
#include "flecs-dash/bake_config.h"
#include "flecs-dash/monitor.h"


////////////////////////////////////////////////////////////////////////////////
//// Module implementation
////////////////////////////////////////////////////////////////////////////////

#ifndef FLECS_LEGACY

ECS_STRUCT(EcsDashServer, {
    int16_t port;
});

ECS_STRUCT(EcsDashApp, {
    char *path;
    char *icon;
});

#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FlecsDash {
    ECS_DECLARE_COMPONENT(EcsDashServer);
    ECS_DECLARE_COMPONENT(EcsDashApp);
} FlecsDash;

FLECS_DASH_API
void FlecsDashImport(
    ecs_world_t *world);

#define FlecsDashImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, EcsDashServer);\
    ECS_IMPORT_COMPONENT(handles, EcsDashApp)

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#ifndef FLECS_NO_CPP

namespace flecs {

class dash : public FlecsDash {
public:
    using Server = EcsDashServer;
    using App = EcsDashApp;

    dash(flecs::world& world) {
        FlecsDashImport(world.c_ptr());

        flecs::module<flecs::dash>(world, "flecs::dash");

        flecs::component<Server>(world, "flecs::dash::Server");
        flecs::component<App>(world, "flecs::dash::App");
    }
};

}

#endif // FLECS_NO_CPP
#endif // __cplusplus

#endif
