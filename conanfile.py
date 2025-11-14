from conan import ConanFile
from conan.tools.cmake import cmake_layout

class GraphEngineConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "gtest/1.17.0", "benchmark/1.9.4"
    generators = "CMakeDeps", "CMakeToolchain"
    
    def layout(self):
        cmake_layout(self)