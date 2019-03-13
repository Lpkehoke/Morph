googletest_dep = dependency('gtest', method: 'pkg-config')
thread_dep = dependency('threads')

morph_test_src = [
    'immutabletest.cpp',
    'taskqueuetest.cpp'
]

morph_test = executable(
    'morphtest',
    morph_test_src,
    include_directories: root_include_dir,
    dependencies: [base_dep, googletest_dep, thread_dep]
)

test('morphtest', morph_test)
