bindcore_src = [
    'core/binddict.cpp',
    'core/bindlogger.cpp',
    'core/bindnode.cpp',
    'core/bindnodefactory.cpp',
    'core/bindnodestorage.cpp',
    'core/bindcore.cpp',
    'core/bindpluginmanager.cpp',
    'core/module.cpp',
    'core/pythondictconversion.cpp'
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

bindgl_src = [
    'gl/module.cpp'
]

bindgl = shared_library(
    '_gl',
    bindgl_src,
    include_directories: root_include_dir,
    dependencies: [
        gl_dep,
        pybind_dep,
        python_dep
    ],
    name_prefix: ''
)

bindui_src = [
    'ui/module.cpp'
]

bindui = shared_library(
    '_ui',
    bindui_src,
    include_directories: root_include_dir,
    dependencies: [
        ui_dep,
        pybind_dep,
        python_dep
    ],
    name_prefix: ''
)

bindcomputeengine_src = [
    'computeengine/module.cpp'
]

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
