platform_src = [
    'enabledispatch.cpp',
    'knob.cpp',
    'logger.cpp',
    'node.cpp',
    'nodefactoryregistry.cpp',
    'nodestorage.cpp',
    'value.cpp',
    'valuetype.cpp'
]

platform = static_library(
    'platform',
    platform_src,
    include_directories: root_include_dir,
    dependencies: [base_dep, tbb_dep]
)

platform_dep = declare_dependency(link_with: platform)
