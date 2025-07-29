message("Convention: --- ----- ----- ----- ----- --")

include_directories(${PROJECT_SOURCE_DIR}/Convention/nlohmann/include)
install(DIRECTORY [Runtime]
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        FILES_MATCHING PATTERN "*.*"
        PATTERN "*.cpp" EXCLUDE)

message("Convention: ----- ----- ----- ----- -----")