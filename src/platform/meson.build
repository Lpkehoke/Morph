tbb_dep = dependency('TBB', method: 'pkg-config')

core_platform = [
    'computeengine.cpp',
    'nodefactoryregistry.cpp',
    'nodestorage.cpp',
    'taskqueue.cpp'
]

core = static_library(
    'platform',
    core_platform,
    include_directories: root_include_dir,
    dependencies: [tbb_dep]
)
