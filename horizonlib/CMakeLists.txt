file(GLOB_RECURSE horizonlib_SRC_FILES lib/*.cpp)

add_library(horizonlib STATIC ${horizonlib_SRC_FILES})

target_include_directories(horizonlib PUBLIC include)
target_compile_features(horizonlib PRIVATE cxx_std_17)

if(MSVC)
    target_compile_options(horizonlib PRIVATE /permissive-)
    target_include_directories(horizonlib PRIVATE "L:/Sources/mbedtls-2.16.6/include")
    target_link_libraries(horizonlib PRIVATE "L:/Sources/mbedtls-2.16.6/visualc/VS2010/x64/Debug/mbedTLS.lib")
else()
    target_link_libraries(horizonlib PRIVATE mbedcrypto)
endif()

add_executable(horizonlib_test test/main.cpp test/tests.cpp)
target_include_directories(horizonlib_test PRIVATE include)
target_compile_features(horizonlib_test PRIVATE cxx_std_17)
target_link_libraries(horizonlib_test PRIVATE horizonlib)
