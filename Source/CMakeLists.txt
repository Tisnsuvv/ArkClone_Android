
set (SOURCE_FILES Main.cpp)

if (NOT URHO3D_WIN32_CONSOLE)
    set (PLATFORM WIN32)
endif ()

# Add executable.
set (TARGET_NAME ${CMAKE_PROJECT_NAME})
add_executable(${TARGET_NAME} ${PLATFORM} ${SOURCE_FILES})
web_executable(${TARGET_NAME})

target_link_libraries (${TARGET_NAME} PRIVATE
    Urho3D
)
