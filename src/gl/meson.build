gl_src = [
    'gl.cpp',
#    'renderer.cpp',
    'shader.cpp'
]

gl = static_library(
    'gl',
    gl_src,
    include_directories: root_include_dir,
    dependencies: [glad_dep]
)

gl_dep = declare_dependency(link_with: gl)
