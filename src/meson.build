root_include_dir = include_directories('.')

boost_dep = dependency('boost', modules: ['python3'])
python_dep = dependency('python-3.6')
tbb_dep = dependency('TBB', method: 'pkg-config')
googletest_dep = dependency('gtest', method: 'pkg-config')
thread_dep = dependency('threads')

subdir('base')
subdir('platform')
subdir('computeengine')
subdir('python')
subdir('test')
