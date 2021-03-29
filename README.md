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
2. Copy the `flecs-dash/etc` directory in this repository to `etc/flecs-dash` under your project root directory
3. Configure a threading implementation in the OS API (see below)

If your project root directory is `my_app`, then your directory structure should look like this:
```
my_app/etc/flecs-dash/apps
my_app/etc/flecs-dash/css
my_app/etc/flecs-dash/images
my_app/etc/flecs-dash/js
my_app/etc/flecs-dash/favicon.png
my_app/etc/flecs-dash/index.html
```

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

**Important**: Make sure to start your application from the project root directory. This ensures that the dashboard code can find the web resources. Go to `http://localhost:9090` to open the dashboard.

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
  "id": "my_app",
  "type": "application",
  "value": {
    "use": [
      "flecs", 
      "flecs.dash",
      "flecs.systems.civetweb", 
      "flecs.os_api.bake"
    ]
  }
}
```

Before creating the world, call this function to install the bake OS API:

```c
bake_set_os_api();
```

Add the above code for creating the server to your application. You can now run the application from anywhere with `bake run`. You don't need to copy the web files to your project root, as it will be automatically discovered by the bake runtime.

## Troubleshooting

### My application crashes when I import the flecs.dash!
This is likely because you did not set an OS API implementation for threading functions, as the server needs synchronization to safely access the world.  See [Setting up OS API](#setting_up_os_api) for how to add this to the application.

### I'm not using bake, can I still use the dashboard?
Absolutely, the dashboard has no dependencies on bake. As long as all the modules are cloned, and the server can find the dashboard web resources, it will work with any buildsystem.

### I'm using bake, do I still need to copy files to my project?
Not if you set the bake OS API (see Flecs examples folder) with `bake_set_os_api()`. This will register a function with Flecs that automatically finds the files.

### My application is running, but if I open a browser I get a 404 error (no page found)
Good news, the server is running! The most likely cause of this is that the server process cannot find the web resources. Make sure to copy the files to the right location as is described in [Usage](#usage). Also ensure that the working directory (typically the location from which the application is started) is the project root.

### My application is running, but if I open a browser I get a server error!
This means that the browser cannot find a server on the specified port. Make sure you're using the correct IP address and port.

### I need to store the resources in a different folder, is this possible?
Yes. If you cannot store your flecs dash resources in the location mentioned above, you can provide a custom location with the "module_to_etc" OS API function. This function overrides the default behavior of Flecs to locate files. To do this, add a function like this to your code:

```c
char* my_module_to_etc(const char* module_name) {
    // Simple fix to make flecs.dash work. Can be made more generic by using the module_name to generate the path
    return ecs_os_strdup("path_to_dash_files");
}

int main(int argc, char *argv[]) {
  // set OS API with *_set_os_api function
  
  // Override default module_to_etc
  ecs_os_api.module_to_etc_ = my_module_to_etc;
}
```

Verify that the path you provide points directly to the files, so if you do `path + "/index.html`, it points to the file.

## Screenshots

<img width="1119" alt="Screen Shot 2020-12-03 at 11 21 12 AM" src="https://user-images.githubusercontent.com/9919222/101077466-bb8eee00-3559-11eb-97b7-6524219dc0d2.png">
<img width="1119" alt="Screen Shot 2020-12-03 at 11 21 32 AM" src="https://user-images.githubusercontent.com/9919222/101077456-b92c9400-3559-11eb-8b6b-6d39bec4d56d.png">


