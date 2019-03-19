from conans import ConanFile, Meson, tools

import glob
import os


class Morph(ConanFile):
    settings = 'os', 'compiler', 'build_type', 'arch', 'cppstd'

    requires = (
        'TBB/2019_U4@conan/stable',
        'gtest/1.8.1@bincrafters/stable',
        'boost/1.69.0@conan/stable'
    )

    generators = {'pkg_config'}

    def configure(self):
        self.options['boost'].without_python = False
        self.options['boost'].shared = True

        # Kludge to optimize build times. Will be resolved with #21.
        self.options['boost'].without_math = True
        self.options['boost'].without_wave = True
        self.options['boost'].without_container = True
        self.options['boost'].without_contract = True
        self.options['boost'].without_exception = True
        self.options['boost'].without_graph = True
        self.options['boost'].without_iostreams = True
        self.options['boost'].without_locale = True
        self.options['boost'].without_log = True
        self.options['boost'].without_program_options = True
        self.options['boost'].without_random = True
        self.options['boost'].without_regex = True
        self.options['boost'].without_mpi = True
        self.options['boost'].without_serialization = True
        self.options['boost'].without_coroutine = True
        self.options['boost'].without_fiber = True
        self.options['boost'].without_context = True
        self.options['boost'].without_timer = True
        self.options['boost'].without_thread = True
        self.options['boost'].without_chrono = True
        self.options['boost'].without_date_time = True
        self.options['boost'].without_atomic = True
        self.options['boost'].without_filesystem = True
        self.options['boost'].without_system = True
        self.options['boost'].without_graph_parallel = True
        self.options['boost'].without_stacktrace = True
        self.options['boost'].without_test = True
        self.options['boost'].without_type_erasure = True

    # Kludge to make Meson find boost python library. Will be resolved with #21.
    def create_boost_python_symlink(self):
        boost_libs_path = self.deps_cpp_info['boost'].lib_paths[0]

        path = glob.glob(boost_libs_path + '/*python3*.so')[0]

        new_name = os.path.basename(path)
        new_name = new_name[0:new_name.find('python3') + 7] + '.so'
        try:
            os.symlink(path, os.path.join(boost_libs_path, new_name))
        except:
            pass

    def build(self):
        self.create_boost_python_symlink()

        tools.os.environ.update(
            {
                'BOOST_INCLUDEDIR': self.deps_cpp_info['boost'].include_paths[0],
                'BOOST_LIBRARYDIR': self.deps_cpp_info['boost'].lib_paths[0]
            }
        )

        meson = Meson(self)
        meson.configure(build_folder='build')
        meson.build()

    def package(self):
        self.copy('*.so', dst='', keep_path=False)

        # copy tbb libs
        tbb_lib_dir = self.deps_cpp_info['TBB'].libdirs[0]
        self.copy('*.so*', src=tbb_lib_dir, dst='', keep_path=False)

        # copy boost libs
        boost_lib_dir = self.deps_cpp_info['boost'].libdirs[0]
        self.copy('*.so*', src=boost_lib_dir, dst='', keep_path=False)
