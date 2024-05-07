import os
import re

from conan import ConanFile
from conan.tools.cmake import CMake
from conan.tools.files import rmdir, load


class Recipe(ConanFile):
    url = "https://tentris.dice-research.org"
    topics = ("triplestore", "sparql", "rdf", "sematic-web", "tensor")
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "with_exec_deps": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "with_exec_deps": False,
        "restinio/*:asio": "boost",
    }

    def requirements(self):
        public_reqs = [
           "boost/1.84.0",
           "fmt/8.1.1",
           "restinio/0.7.2",
           "expected-lite/0.6.3",  # overrides restinio dependency
           "hypertrie/0.9.5",
           "metall/0.21",
           "rdf4cpp/0.0.27",
           "dice-hash/0.4.0",
           "robin-hood-hashing/3.11.5",
           "dice-sparse-map/0.2.5",
           "cxxopts/2.2.1",
           "sparql-parser-base/0.3.0",
           "taskflow/3.4.0",
           "cppitertools/2.1",
           "spdlog/1.14.1",
           "rapidjson/cci.20220822",
        ]

        private_reqs = [
        ]

        exec_reqs = [
            "nlohmann_json/3.11.2",
            "vincentlaucsb-csv-parser/2.1.3",
        ]
        for req in public_reqs:
            self.requires(req)
        for req in private_reqs:
            self.requires(req, private=True)

        if self.options.get_safe("with_exec_deps"):
            for req in exec_reqs:
                self.requires(req)

    generators = ("cmake_find_package",)

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "libs/*", "CMakeLists.txt", "cmake/*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def set_name(self):
        if not hasattr(self, 'name') or self.version is None:
            cmake_file = load(self, os.path.join(self.recipe_folder, "CMakeLists.txt"))
            self.name = re.search(r"project\(\s*([a-z\-]+)\s+VERSION", cmake_file).group(1)

    def set_version(self):
        if not hasattr(self, 'version') or self.version is None:
            cmake_file = load(self, os.path.join(self.recipe_folder, "CMakeLists.txt"))
            self.version = re.search(r"project\([^)]*VERSION\s+(\d+\.\d+.\d+)[^)]*\)", cmake_file).group(1)
        if not hasattr(self, 'description') or self.description is None:
            cmake_file = load(self, os.path.join(self.recipe_folder, "CMakeLists.txt"))
            self.description = re.search(r"project\([^)]*DESCRIPTION\s+\"([^\"]+)\"[^)]*\)", cmake_file).group(1)

    _cmake = None

    def _configure_cmake(self):
        if self._cmake:
            return self._cmake
        self._cmake = CMake(self)
        self._cmake.definitions['CONAN_CMAKE'] = False
        self._cmake.configure()
        return self._cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()
        for dir in ("res", "share"):
            rmdir(os.path.join(self.package_folder, dir))

    def package_info(self):
        self.cpp_info.components["global"].set_property("cmake_target_name", "tentris::tentris")
        self.cpp_info.components["global"].names["cmake_find_package_multi"] = "tentris"
        self.cpp_info.components["global"].names["cmake_find_package"] = "tentris"
        self.cpp_info.components["global"].includedirs = [f"include/tentris/tentris"]
        self.cpp_info.components["global"].libdirs = []
        self.cpp_info.set_property("cmake_file_name", "tentris")
        self.cpp_info.components["global"].requires = [
            "node-store", "rdf-tensor", "sparql2tensor", "triple-store", "endpoint",
            "boost::boost",
            "fmt::fmt",
            "restinio::restinio",
            "hypertrie::hypertrie",
            "metall::metall",
            "rdf4cpp::rdf4cpp",
            "sparql-parser-base::sparql-parser-base",
            "dice-hash::dice-hash",
            "cxxopts::cxxopts",
            "robin-hood-hashing::robin-hood-hashing",
            "expected-lite::expected-lite",
            "restinio::restinio",
            "taskflow::taskflow",
            "cppitertools::cppitertools",
            "spdlog::spdlog",
        ]

        for component in ["node-store", "rdf-tensor", "sparql2tensor", "triple-store", "endpoint"]:
            self.cpp_info.components[f"{component}"].names["cmake_find_package_multi"] = f"{component}"
            self.cpp_info.components[f"{component}"].names["cmake_find_package"] = f"{component}"
            self.cpp_info.components[f"{component}"].includedirs = [f"include/tentris/{component}"]

        for component in ["node-store", "sparql2tensor", "triple-store", "endpoint"]:
            self.cpp_info.components[f"{component}"].libdirs = [f"lib/tentris/{component}"]
            self.cpp_info.components[f"{component}"].libs = [f"{component}"]

        self.cpp_info.components["rdf-tensor"].requires = [
            "rdf4cpp::rdf4cpp",
            "hypertrie::hypertrie",
            "boost::boost",
            "metall::metall",
        ]

        self.cpp_info.components["node-store"].requires = [
            "rdf-tensor",
        ]

        self.cpp_info.components["sparql2tensor"].requires = [
            "node-store",
            "robin-hood-hashing::robin-hood-hashing",
            "sparql-parser-base::sparql-parser-base",
        ]

        self.cpp_info.components["triple-store"].requires = [
            "sparql2tensor",
            "rdf-tensor",
        ]
        self.cpp_info.components["endpoint"].requires = [
            "rdf-tensor",
            "restinio::restinio",
            "taskflow::taskflow",
            "cppitertools::cppitertools",
            "spdlog::spdlog",
            "rapidjson::rapidjson",
        ]
        if self.options.get_safe("with_exec_deps"):
            self.cpp_info.components["global"].requires += [
                "vincentlaucsb-csv-parser::vincentlaucsb-csv-parser",
                "nlohmann_json::nlohmann_json"]
