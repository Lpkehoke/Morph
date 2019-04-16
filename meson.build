project('morph', 'cpp', default_options: ['cpp_std=c++17'])

pybind_dep = subproject('pybind11').get_variable('pybind11_dep')
tbb_dep = subproject('TBB').get_variable('tbb_dep')
googletest_dep = subproject('gtest').get_variable('gtest_dep')
glad_dep = subproject('glad').get_variable('glad_dep')

thread_dep = dependency('threads')
python_dep = dependency('python3')

subdir('src')
