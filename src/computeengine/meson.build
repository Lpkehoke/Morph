tbb_dep = dependency('TBB', method: 'pkg-config')

compute_engine_src = [
    'computeengine.cpp',
    'computetask.cpp',
    'computetaskdagbuilder.cpp'
]

compute_engine = static_library(
    'compute_engine',
    compute_engine_src,
    include_directories: root_include_dir,
    dependencies: [base_dep, tbb_dep]
)

compute_engine_dep = declare_dependency(link_with: compute_engine)