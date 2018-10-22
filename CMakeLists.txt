cmake_minimum_required(VERSION 3.4)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)

# Project name and version
project(CppPlayer VERSION 1.0.0 LANGUAGES CXX)

# Add feature summary
include(FeatureSummary)

# Add CTest
include(CTest)

# Option whether to build tests
option(BUILD_TESTS "Build tests." ON)

# Options for documentation
option(ENABLE_API_DOC "Enable building API Documentation" OFF)
add_feature_info("Enable API Documentation" ENABLE_API_DOC "Enables building the API documentation")

option(ENABLE_USER_DOC "Enable building User documentation" OFF)
add_feature_info("Enable User Documentation" ENABLE_USER_DOC "Enables building the User documentation")

# Options for cpack / deb packages production
option(ENABLE_CPACK "Enable cpack" ON)
add_feature_info("Enable cpack integration" ENABLE_CPACK "Enables producing of deb packages")

# Define GNU standard installation directories
include(GNUInstallDirs)

# C++ compiler flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

# Project targets
set(TARGET ${PROJECT_NAME})

# Target source files
file(GLOB_RECURSE PUBLIC_HEADERS  RELATIVE "${CMAKE_SOURCE_DIR}" "include/*.h")

# Declare library
add_library(${TARGET} INTERFACE)
# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(${TARGET} INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
		)

install(TARGETS ${TARGET} EXPORT "${TARGET}Config"
	ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})  # This is for Windows
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${TARGET})

install(EXPORT "${TARGET}Config" DESTINATION share/${TARGET}/cmake)

export(TARGETS ${TARGET} FILE "${TARGET}Config.cmake")

# The following pkg config file is for backwards compatibility
# Usually imported targets, like above should be used

# Use a pkg-config module for CMake
find_package(PkgConfig)
# Create pkgconfig file
configure_file(CppPlayer.pc.in CppPlayer.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CppPlayer.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

if(ENABLE_CPACK)
	include("CPack.cmake")
endif()

add_subdirectory("samples")