import os
from conan import ConanFile
from conan.tools.build import can_run


class z2m_testappTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def test(self):
        pass
        #if can_run(self):
            #self.run("z2m_testapp", env="conanrun")
