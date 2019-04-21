core_src = [
    'attribute.cpp',
    'dict.cpp',
    'enabledispatch.cpp',
    'knob.cpp',
    'knobschema.cpp',
    'logger.cpp',
    'node.cpp',
    'nodestorage.cpp',
    'operations.cpp',
    'core.cpp',
    'pluginmanager.cpp',
    'value/defaultvalues.cpp',
    'value/value.cpp',
    'value/valuetype.cpp'
]

core = static_library(
    'core',
    core_src,
    include_directories: root_include_dir,
    dependencies: [foundation_dep, tbb_dep]
)

core_dep = declare_dependency(link_with: core)
