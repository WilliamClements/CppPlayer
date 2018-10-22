set(TARGET IHFDM)

file(GLOB HEADERS_FILES "./include/*.h")
file(GLOB SOURCE_FILES "./*.cpp" "./*.h")

add_executable(${TARGET} ${SOURCE_FILES})
target_link_libraries(${TARGET} CppPlayer)
include_directories("include")