project('hashmap', 'c',
  version: '0.1',
  license: 'MPL-2.0',
  default_options: ['warning_level=3'])

include = 'include'
src = 'src'
hashmap = library('hashmap', sources: [
  src/'hashmap.c',
  src/'fnv1_32.c',
  src/'fnv1_64.c'
], install: true, include_directories: include)
install_headers(include/'fnv1_32.h', include/'fnv1_64.h', include/'hashmap.h')

hashmap_dep = declare_dependency(link_with: hashmap, include_directories: include)

meson.override_dependency('hashmap', hashmap_dep)

executable('example', files(src/'example.c'), install: false, dependencies: [
  hashmap_dep
])
