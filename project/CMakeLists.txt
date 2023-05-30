add_executable(MyProgram
    main.cpp
)
target_compile_features(MyProgram PUBLIC cxx_std_20)

install(TARGETS MyProgram RUNTIME)

