message("Convention: --- ----- ----- ----- ----- --")

install(DIRECTORY [Runtime]
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        FILES_MATCHING PATTERN "*.*"
        PATTERN "*.cpp" EXCLUDE)

message("Convention: ----- ----- ----- ----- -----")