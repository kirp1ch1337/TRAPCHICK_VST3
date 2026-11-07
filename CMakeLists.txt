cmake_minimum_required(VERSION 3.22)
project(TRAPCHIK VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED JUCE_DIR)
    message(FATAL_ERROR "JUCE_DIR is not set. Run build_windows.ps1.")
endif()
add_subdirectory("${JUCE_DIR}" JUCE)

juce_add_plugin(TRAPCHIK
    COMPANY_NAME "TRAPCHIK Audio"
    BUNDLE_ID "com.trapchik.audio"
    PLUGIN_MANUFACTURER_CODE TrPk
    PLUGIN_CODE Tr01
    FORMATS VST3
    PRODUCT_NAME "TRAPCHIK"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

juce_generate_juce_header(TRAPCHIK)

target_sources(TRAPCHIK PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
)
target_compile_definitions(TRAPCHIK PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)
target_link_libraries(TRAPCHIK PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    PUBLIC juce::juce_recommended_config_flags
           juce::juce_recommended_lto_flags
           juce::juce_recommended_warning_flags
)
