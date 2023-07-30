
find_package(Freetype REQUIRED)
find_package(FreeImage CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(unofficial-omniverse-physx-sdk CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

# graphics
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

# utility
find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

include (Framework/CMakeSources.cmake)

add_executable(${PROJECT_NAME} main.cpp ${FRAMEWORK_SOURCS})
target_link_libraries(${PROJECT_NAME} 
PRIVATE
    Freetype::Freetype
    freeimage::FreeImage freeimage::FreeImagePlus
    OpenAL::OpenAL
    unofficial::omniverse-physx-sdk::sdk
    assimp::assimp

    glfw OpenGL::GL glad

    spdlog::spdlog_header_only glm::glm)

target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)
target_include_directories(${PROJECT_NAME}
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>")


# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json)
