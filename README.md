### C hashmap library

This is a simple hashmap library in C. It uses a linked list to handle collisions.

See the example.c file for usage.

### Installing to your meson project
Copy `hashmap.wrap` to your `subprojects` directory and add the following to your `meson.build` file:

```meson
subproject('hashmap')
```

Then you can use the library like this:

```meson
executable('example', sources: files('example.c'), dependencies: [
  dependency('hashmap')
])
```
