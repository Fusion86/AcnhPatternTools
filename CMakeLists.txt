cmake_minimum_required(VERSION 3.14)

project(AcnhPatternTools)

if(MSVC)
    add_compile_options(/W3 /WX /MP)
else()
    add_compile_options(-W -Wall)
endif()

if(APPLE)
    # -fno-stack-check is needed because of https://forums.developer.apple.com/thread/121887#382262
    add_compile_options(-fno-stack-check)
    include_directories("/usr/local/include")
    link_directories("/usr/local/lib/")
elseif(UNIX)
    # stdc++fs is needed because of Ubuntu
    link_libraries(stdc++fs)
endif()

add_subdirectory(horizonlib)
add_subdirectory(lappland_wx)
