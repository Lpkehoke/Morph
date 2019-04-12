bindcore_src = [
    'core/bindlogger.cpp',
    'core/bindnode.cpp',
    'core/bindnodefactory.cpp',
    'core/bindnodestorage.cpp',
    'core/bindcore.cpp',
    'core/bindpluginmanager.cpp',
    'core/module.cpp',
    'core/pythondictconversion.cpp'
]

bindcomputeengine_src = [
    'computeengine/module.cpp'
]

bindcore = shared_library(
    '_core',
    bindcore_src,
    include_directories: root_include_dir,
    dependencies: [
        core_dep,
        pybind_dep,
        python_dep],
    name_prefix: ''
)

bindcomputeengine = shared_library(
    '_computeengine',
    bindcomputeengine_src,
    include_directories: root_include_dir,
    dependencies: [
        computeengine_dep,
        core_dep,
        pybind_dep,
        python_dep],
    name_prefix: ''
)

