file(GLOB lappland_wx_SRC_FILES src/*.cpp)

find_package(wxWidgets REQUIRED COMPONENTS core base)

if(WIN32)
    add_executable(lappland_wx WIN32 ${lappland_wx_SRC_FILES})
else()
    add_executable(lappland_wx ${lappland_wx_SRC_FILES})
endif()

# Use `include` because `target_include_directories` doesn't work
include(${wxWidgets_USE_FILE})

target_include_directories(lappland_wx PRIVATE include)
target_link_libraries(lappland_wx PRIVATE horizonlib ${wxWidgets_LIBRARIES})
target_compile_features(lappland_wx PRIVATE cxx_std_17)

if(MSVC)
    target_compile_options(lappland_wx PRIVATE /permissive-)
endif()
