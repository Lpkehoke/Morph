project('morph', 'cpp', default_options: ['cpp_std=c++17'])

boost_dep = subproject('boost').get_variable('boost_dep')
tbb_dep = subproject('TBB').get_variable('tbb_dep')
googletest_dep = subproject('gtest').get_variable('gtest_dep')

thread_dep = dependency('threads')
python_dep = dependency('python3')

subdir('src')
