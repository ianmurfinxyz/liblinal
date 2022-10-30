from conans import ConanFile, CMake

class LibLinalConan(ConanFile):
	name = "liblinal"
	version = "0.0.1"
	license = "GNU GPLv3"
	author = "Ian Murfin - github.com/ianmurfinxyz - ianmurfin@protonmail.com"
	url = "https://github.com/ianmurfinxyz/liblinal"
	description = "Linear algebra maths library"
	settings = "os", "compiler", "build_type", "arch"
	options = {"fPIC": [True, False]}
	default_options = {"fPIC": True}
	user = "ianmurfinxyz"
	channel = "stable"
	generators = "cmake"
	exports_sources = ["CMakeLists.txt", "src/*", "include/*"]

	def config_options(self):
		if self.settings.os == "Windows":
			del self.options.fPIC

	def requirements(self):
		self.requires("gtest/cci.20210126")

	def build(self):
		cmake = CMake(self)
		cmake.configure(build_folder=self.build_subfolder)
		cmake.build()

	def package(self):
		self.copy("*.h", dst="include", src="include", keep_path=True)

	def package_info(self):
		self.cpp_info.includedirs = ['include']
