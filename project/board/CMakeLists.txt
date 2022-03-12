add_library(Board
    board.cpp
)
target_include_directories(Board PUBLIC .)

add_executable(BoardTest
    main_test.cpp
    board_test.cpp
)
target_include_directories(BoardTest PUBLIC .)
target_link_libraries(BoardTest PRIVATE gtest_main)
target_link_libraries(BoardTest PRIVATE Board)

gtest_discover_tests(BoardTest)

