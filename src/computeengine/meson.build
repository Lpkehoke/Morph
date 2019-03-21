compute_engine_src = [
    'computeengine.cpp',
    'computegraph.cpp',
    'computetask.cpp'
]

computeengine = static_library(
    'compute_engine',
    compute_engine_src,
    include_directories: root_include_dir,
    dependencies: [base_dep, tbb_dep]
)

computeengine_dep = declare_dependency(link_with: computeengine)