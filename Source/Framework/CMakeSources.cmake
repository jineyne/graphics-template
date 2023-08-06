set (GRAPHICS_SRCS
    "Framework/Graphics/Buffer.cpp"
    "Framework/Graphics/Buffer.h"
    "Framework/Graphics/Graphics.cpp"
    "Framework/Graphics/Graphics.h"
    "Framework/Graphics/GraphicsAPI.cpp"
    "Framework/Graphics/GraphicsAPI.h"
    "Framework/Graphics/Shader.cpp"
    "Framework/Graphics/Shader.h"
    "Framework/Graphics/Texture.cpp"
    "Framework/Graphics/Texture.h"
    "Framework/Graphics/Window.cpp"
    "Framework/Graphics/Window.h")

set (IMPORTER_SRCS
    "Framework/Importer/Importer.cpp"
    "Framework/Importer/Importer.h"
    "Framework/Importer/SpecificImporter.cpp"
    "Framework/Importer/SpecificImporter.h")

set (MANAGER_SRCS
    "Framework/Manager/TextureManager.cpp"
    "Framework/Manager/TextureManager.h")

set (RESOURCE_SRCS
    "Framework/Resource/Resource.cpp"
    "Framework/Resource/Resource.h"
    "Framework/Resource/Resources.cpp"
    "Framework/Resource/Resources.h")

set (UTILITY_SRCS
    "Framework/Utility/CommandArgsParser.cpp"
    "Framework/Utility/CommandArgsParser.h"
    "Framework/Utility/EnumHelper.h"
    "Framework/Utility/Exceptions.cpp"
    "Framework/Utility/Exceptions.h"
    "Framework/Utility/ISerializable.h"
    "Framework/Utility/Module.h")

set (FRAMEWORK_SOURCS
    ${GRAPHICS_SRCS}
    ${IMPORTER_SRCS}
    ${MANAGER_SRCS}
    ${RESOURCE_SRCS}
    ${UTILITY_SRCS})
