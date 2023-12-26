add_executable(MyProgram "")
target_compile_features(MyProgram PRIVATE cxx_std_20)
target_compile_options(MyProgram PRIVATE ${PROJECT_WARNING_OPTIONS})
target_sources(MyProgram
PRIVATE
    main.cpp
)
install(TARGETS MyProgram EXPORT MyProjectTargets)
