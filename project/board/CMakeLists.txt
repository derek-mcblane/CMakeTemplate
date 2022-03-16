add_library(Board "")
target_sources(Board
    PRIVATE
        board.cpp
        board.h
        #PUBLIC
        #$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/board.h
        #$<INSTALL_INTERFACE:board.h
)
target_include_directories(Board PRIVATE ${CMAKE_CURRENT_LIST_DIR})

install(TARGETS Board
    EXPORT OthelloCppConfig
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
export(TARGETS Board
    NAMESPACE OthelloCpp::
    FILE ${CMAKE_CURRENT_BINARY_DIR}/OthelloCppConfig.cmake
)
install(EXPORT OthelloCppConfig
    DESTINATION OthelloCpp/cmake
    NAMESPACE OthelloCpp::
)

add_executable(BoardTest "")
target_sources(BoardTest
    PRIVATE
        main_test.cpp
        board_test.cpp
)
target_include_directories(BoardTest PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(BoardTest PRIVATE gtest_main)
target_link_libraries(BoardTest PRIVATE Board)

gtest_discover_tests(BoardTest)

