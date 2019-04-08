platform_src = [
    'attribute.cpp',
    'enabledispatch.cpp',
    'knob.cpp',
    'knobschema.cpp',
    'logger.cpp',
    'node.cpp',
    'nodefactoryregistry.cpp',
    'nodestorage.cpp',
    'operations.cpp',
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
