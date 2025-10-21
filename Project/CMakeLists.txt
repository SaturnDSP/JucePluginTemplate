########################################################################################################################
## Juce Boilerplate
########################################################################################################################

set(JucePluginTemplate "AudioPlugin")
set(ICON "${CMAKE_CURRENT_SOURCE_DIR}/assets/Icon.png")
set(CMAKE_CXX_STANDARD 23)

juce_add_plugin(${PROJECT_NAME}
    ICON_BIG
        ${ICON}
    ICON_SMALL
        ${ICON}
    COMPANY_NAME
        SaturnDSP
    IS_SYNTH
        FALSE
    NEEDS_MIDI_INPUT
        FALSE
    NEEDS_MIDI_OUTPUT
        FALSE
    IS_MIDI_EFFECT
        FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS
        FALSE
    COPY_PLUGIN_AFTER_BUILD
        FALSE
    PLUGIN_MANUFACTURER_CODE
        Strn
    PLUGIN_CODE
        Strn
    FORMATS
        AU
        VST3
        Standalone
    PRODUCT_NAME ${PROJECT_NAME}
)

file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

file(GLOB_RECURSE HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/inc/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h"
)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${SOURCES}
        ${HEADERS}
)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        juce::juce_audio_utils
        juce::juce_opengl
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

########################################################################################################################
## Asset Loading: std::string path = std::string(ASSETS_DIR) + "/file.txt"
########################################################################################################################

set(ASSETS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSETS_DEST_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets")

file(GLOB_RECURSE ASSET_FILES "${ASSETS_SOURCE_DIR}/*")

add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ASSETS_SOURCE_DIR}"
        "${ASSETS_DEST_DIR}"
        DEPENDS ${ASSET_FILES}
        COMMENT "Copying updated assets..."
)

add_dependencies(${PROJECT_NAME} copy_assets)

target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        ASSETS_DIR="${ASSETS_DEST_DIR}"
)

########################################################################################################################
## Asset Loading End
########################################################################################################################
