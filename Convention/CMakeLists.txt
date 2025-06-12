message("Convention: --- ----- ----- ----- ----- --")

install(DIRECTORY [Runtime]
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.*"
        PATTERN "*.cpp" EXCLUDE)

message("Convention: ----- ----- ----- ----- -----")