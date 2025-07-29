add_executable(TEST test.cpp )
include_directories(${PROJECT_SOURCE_DIR}/Convention/[Runtime])
include_directories(${PROJECT_SOURCE_DIR}/Convention/nlohmann/include)
install(TARGETS TEST
        RUNTIME DESTINATION                 ${CMAKE_INSTALL_PREFIX}/bin)