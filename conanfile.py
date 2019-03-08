from conans import ConanFile, Meson, tools


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

    def build(self):
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
        tbb_lib_dir = self.deps_cpp_info["TBB"].libdirs[0]
        self.copy('*', src=tbb_lib_dir, dst='', keep_path=False)

        # copy boost libs
        boost_lib_dir = self.deps_cpp_info["boost"].libdirs[0]
        self.copy('*', src=boost_lib_dir, dst='', keep_path=False)
