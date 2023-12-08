project(MyProgram)
add_executable(MyProgram "")
target_compile_features(MyProgram PUBLIC cxx_std_20)
target_compile_options(MyProgram PUBLIC ${PROJECT_WARNING_OPTIONS})
target_sources(MyProgram
PRIVATE
    main.cpp
)

install(TARGETS MyProgram EXPORT MyProjectTargets RUNTIME)
