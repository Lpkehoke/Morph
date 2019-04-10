bindplatform_src = [
    'platform/bindlogger.cpp',
    'platform/bindnodestorage.cpp',
    'platform/bindplatformcore.cpp',
    'platform/bindpluginmanager.cpp',
    'platform/module.cpp',
    'platform/pythondictconversion.cpp'
]

bindcomputeengine_src = [
    'computeengine/module.cpp'
]

bindplatform = shared_library(
    '_platform',
    bindplatform_src,
    include_directories: root_include_dir,
    dependencies: [
        platform_dep,
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
        platform_dep,
        pybind_dep,
        python_dep],
    name_prefix: ''
)

