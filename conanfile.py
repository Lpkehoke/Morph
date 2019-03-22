from conans import ConanFile, Meson, tools

import os


class Morph(ConanFile):
    settings = 'os', 'compiler', 'build_type', 'arch', 'cppstd'

    requires = (
        'TBB/2019_U4@conan/stable',
        'gtest/1.8.1@bincrafters/stable',
        'boost/1.69.0@conan/stable'
    )

    generators = {}

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

    def generate_meson_subproject(self, subdirs_path, name, libs, lib_dirs, include_dirs):
        project_dir = os.path.join(subdirs_path, name)
        project_file = os.path.join(project_dir, 'meson.build')

        if not os.path.exists(project_dir):
            os.mkdir(project_dir)
        
        with open(project_file, 'w') as project:
            print(f"project('{name}', 'cpp')", file=project)
            print("cpp = meson.get_compiler('cpp')", file=project)

            declared_lib_names = []
            for lib in libs:
                if (lib == 'pthreads'):
                    print(f"{lib} = dependency('threads')")
                else:
                    print(f"{lib} = cpp.find_library('{lib}', dirs: {str(lib_dirs)})", file=project)
                declared_lib_names.append(lib)

            print(f"include_dirs = include_directories({str(include_dirs)})", file=project)
            
            print(f"{name.lower()}_dep = declare_dependency(", file=project)
            deps_list = ', '.join(declared_lib_names)
            print(f"dependencies: [{deps_list}],", file=project)
            print(f"include_directories: include_dirs)", file=project)

    def create_meson_subprojects(self):
        subprojects_path = os.path.join(
            os.path.dirname(os.path.abspath(__file__)),
            'subprojects')

        if not os.path.exists(subprojects_path):
            os.mkdir(subprojects_path)
        
        deps = self.deps_cpp_info.deps

        for dependency in deps:
            libs = self.deps_cpp_info[dependency].libs
            lib_dirs = self.deps_cpp_info[dependency].lib_paths
            include_dirs = self.deps_cpp_info[dependency].include_paths
            self.generate_meson_subproject(
                subprojects_path,
                dependency,
                libs,
                lib_dirs,
                include_dirs)

    def build(self):
        self.create_meson_subprojects()

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
