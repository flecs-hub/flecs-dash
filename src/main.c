#include <flecs_dash.h>

static
bool request_this(
    ecs_world_t *world,
    ecs_entity_t entity,
    EcsHttpEndpoint *endpoint,
    EcsHttpRequest *request,
    EcsHttpReply *reply)
{
    ecs_entity_t server = ecs_get_parent_w_entity(world, entity, 0);
    if (server) {
        ecs_strbuf_t buf = ECS_STRBUF_INIT;
        char *path = ecs_get_fullpath(world, server);
        ecs_strbuf_append(&buf, "{\"server_id\":\"%s\"}", path);
        ecs_os_free(path);
        reply->body = ecs_strbuf_get(&buf);
        return true;
    } else {
        reply->body = ecs_os_strdup("{\"server_id\":\"unknown\"}");
        return false;
    }
}

static
bool request_files(
    ecs_world_t *world,
    ecs_entity_t entity,
    EcsHttpEndpoint *endpoint,
    EcsHttpRequest *request,
    EcsHttpReply *reply)
{
    const char *file = request->relative_url;
    char path[1024];

    if (!file || !strlen(file)) {
        file = "index.html";
    }

    sprintf(path, "etc/%s", file);

    FILE *f = fopen(path, "r");
    if (!f) {
        return false;
    } else {
        fclose(f);
    }

    reply->body = ecs_os_strdup(path);
    reply->is_file = true;

    return true;
}

static
void RunServer(ecs_iter_t *it) {
    ecs_world_t *world = it->world;

    EcsDashServer *server = ecs_column(it, EcsDashServer, 1);

    ecs_entity_t ecs_entity(EcsHttpEndpoint) = ecs_column_entity(it, 3);
    ecs_entity_t ecs_entity(EcsRestServer) = ecs_column_entity(it, 4);

    int32_t i;
    for (i = 0; i < it->count; i ++) {
        ecs_entity_t e = it->entities[i];
        EcsDashServer *s = &server[i];

        /* Create REST server */
        ecs_set(world, e, EcsRestServer, {.port = s->port});
        
        /* Add endpoint to server for serving up files */
        ecs_entity_t e_files = ecs_new_w_entity(world, ECS_CHILDOF | e);
            ecs_set(world, e_files, EcsName, {"e_files"});
            ecs_set(world, e_files, EcsHttpEndpoint, {
                .url = "",
                .action = request_files});

        /* Add endpoint to server that returns entity id of server */
        ecs_entity_t e_this = ecs_new_w_entity(world, ECS_CHILDOF | e);
            ecs_set(world, e_this, EcsName, {"e_this"});
            ecs_set(world, e_this, EcsHttpEndpoint, {
                .url = "this",
                .action = request_this});                
    }
}

ECS_STRUCT(Position, {
    float x;
    float y;
});

ECS_STRUCT(Velocity, {
    float x;
    float y;
});

void FlecsDashImport(
    ecs_world_t *world,
    int flags)
{
    ECS_MODULE(world, FlecsDash);

    ECS_IMPORT(world, FlecsMeta, 0);
    ECS_IMPORT(world, FlecsComponentsHttp, 0);
    ECS_IMPORT(world, FlecsSystemsRest, 0);

    ecs_set_name_prefix(world, "EcsDash");

    ECS_META(world, EcsDashServer);
    ECS_META(world, EcsDashApp);

    ECS_SYSTEM(world, RunServer, EcsOnSet, Server,
        :flecs.components.http.Server,
        :flecs.components.http.Endpoint,
        :flecs.systems.rest.Server);

    ECS_EXPORT_COMPONENT(EcsDashServer);
    ECS_EXPORT_COMPONENT(EcsDashApp);
}

int main(int argc, char *argv[]) {
    ecs_world_t *world = ecs_init();
    
    ECS_IMPORT(world, FlecsDash, 0);
    ECS_IMPORT(world, FlecsSystemsCivetweb, 0);
    ECS_IMPORT(world, FlecsMeta, 0);

    ECS_META(world, Position);
    ECS_META(world, Velocity);

    ECS_ENTITY(world, E1, 0);
        ecs_set(world, E1, Position, {10, 20});
        ecs_set(world, E1, Velocity, {1, 1});

    ECS_ENTITY(world, E2, 0);
        ecs_set(world, E2, Position, {30, 40});
        ecs_set(world, E2, Velocity, {1, 1});

    ECS_ENTITY(world, E3, CHILDOF | E2);
        ecs_set(world, E3, Position, {30, 40});
        ecs_set(world, E3, Velocity, {1, 1});

    // Create dash server
    ecs_entity_t dash = ecs_set(world, 0, EcsName, {"dash"});
        ecs_set(world, dash, EcsDashServer, {.port = 8080});

    // Create dash browser app
    ecs_entity_t dash_browser = ecs_new_w_entity(world, ECS_CHILDOF | dash);
        ecs_set(world, dash_browser, EcsName, {"browser"});
        ecs_set(world, dash_browser, EcsDashApp, {
            .path = "etc/apps/browser"
        });

    ecs_set_target_fps(world, 60);

    while (ecs_progress(world, 0));

    ecs_fini(world);
}