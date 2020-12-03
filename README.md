# flecs-dash
Web-based dashboard for flecs. This project is still under development and may change a lot!

## Usage
The dashboard requires the following modules:

Module      | Description      
------------|------------------
[flecs.meta](https://github.com/flecs-hub/flecs-meta) | Reflection for Flecs components
[flecs.json](https://github.com/flecs-hub/flecs-json) | JSON serializer for Flecs components
[flecs.rest](https://github.com/flecs-hub/flecs-rest) | A REST interface for introspecting & editing entities
[flecs.monitor](https://github.com/flecs-hub/flecs-monitor) | Web-based monitoring
[flecs.player](https://github.com/flecs-hub/flecs-player) | Play, stop and pause simulations
[flecs.components.http](https://github.com/flecs-hub/flecs-components-http) | Components describing an HTTP server
[flecs.systems.civetweb](https://github.com/flecs-hub/flecs-systems-civetweb) | HTTP server implementation
[flecs.dash](https://github.com/flecs-hub/flecs-dash) | The dashboard (this project)

1. Add the module source & header file (from the repository root) to your project for each dependency
2. Copy the `flecs-dash/etc` directory in this repository to `etc/flecs-dash` in your project root
3. Configure a threading implementation in the OS API (see below)

To add the dashboard to a C application, do:
```c
ECS_IMPORT(world, FlecsDash);
ECS_IMPORT(world, FlecsSystemsCivetweb);

ecs_set(world, 0, EcsDashServer, {.port = 9090});
```
To add it to a C++ application, do:
```cpp
world.import<flecs::dash>();
world.import<flecs::systems::civetweb>();

ecs.entity().set<flecs::dash::Server>({9090});
```

Make sure to start your application from the project root. This ensures that the dashboard code can find the web resources. Go to `http://localhost:9090` to open the dashboard.

## Setting up OS API
The webserver requires a threading implementation to be configured in the Flecs OS API. You can do this yourself, or add this example as a dependency to your project:
https://github.com/SanderMertens/flecs/tree/master/examples/os_api/flecs-os_api-posix. When using this example, make sure to add this before creating the world:

```c
posix_set_os_api();
```

## Using flecs.dash with bake
When using bake, clone & build the above modules and add them as dependencies to your project. Additionally, build & add the bake OS API example from the flecs repository as well.

```json
{
  "id": "my_app"
  "type": "application",
  "value": {
    "use": [
      "flecs", 
      "flecs.dash",
      "flecs.systems.civetweb", 
       flecs.os_api.bake"
    ]
  }
}
```

Before creating the world, call this function to install the bake OS API:

```c
bake_set_os_api();
```

Add the above code for creating the server to your application. You can now run the application from anywhere with `bake run`. You don't need to copy the web files to your project root, as it will be automatically discovered by the bake runtime.

## Screenshot

<img width="1119" alt="Screen Shot 2020-12-03 at 11 02 08 AM" src="https://user-images.githubusercontent.com/9919222/101075545-02c7af80-3557-11eb-94eb-f11d7c7dc2f3.png">

<img width="1271" alt="Screen Shot 2020-12-02 at 12 00 42 AM" src="https://user-images.githubusercontent.com/9919222/101075145-7f0dc300-3556-11eb-9646-ab48fa7e6a72.png">

