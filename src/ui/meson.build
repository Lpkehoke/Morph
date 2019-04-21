ui_src = [
    'application.cpp',
    'orthocamera.cpp',
    'viewport.cpp',
    'window.cpp',
    'windowmanager.cpp'
]

ui = static_library(
    'ui',
    ui_src,
    include_directories: root_include_dir,
    dependencies: [foundation_dep, core_dep, gl_dep, sdl2_dep, eigen_dep]
)

ui_dep = declare_dependency(link_with: ui)
