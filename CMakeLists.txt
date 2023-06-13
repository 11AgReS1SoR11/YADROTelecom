
cmake_minimum_required (VERSION 3.8)

project ("YADROTelecom")

add_executable (YADROTelecom "YADROTelecom.cpp" "YADROTelecom.h" "main.cpp")

set_target_properties(
	YADROTelecom PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
)
