set (GRAPHICS_SRCS
    "Framework/Graphics/Buffer.cpp"
    "Framework/Graphics/Buffer.h"
    "Framework/Graphics/Graphics.cpp"
    "Framework/Graphics/Graphics.h"
    "Framework/Graphics/Renderer.cpp"
    "Framework/Graphics/Renderer.h"
    "Framework/Graphics/Shader.cpp"
    "Framework/Graphics/Shader.h")

set (UTILITY_SRCS
    "Framework/Utility/CommandArgsParser.cpp"
    "Framework/Utility/CommandArgsParser.h"
    "Framework/Utility/Exceptions.cpp"
    "Framework/Utility/Exceptions.h"
    "Framework/Utility/Module.h")

set (FRAMEWORK_SOURCS
    ${GRAPHICS_SRCS}
    ${UTILITY_SRCS})
