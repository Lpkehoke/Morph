tbb_dep = dependency('TBB', method: 'pkg-config')

base_src = [
    'taskqueue.cpp'
]

base = static_library(
    'base',
    base_src,
    include_directories: root_include_dir,
    dependencies: [tbb_dep]
)

base_dep = declare_dependency(link_with: base)
