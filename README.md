# JUCE Plugin Template

A cleaner, more organized alternative to the default JUCE plugin template with better code separation.

## Features
- Organized sections with comments
- Cleaner constructor without preprocessor hell
- Bus layout and parameter-tree in separate utility functions

## Comparison

### This Template
```cpp
Audio_Processor::Audio_Processor()
    : AudioProcessor(CreateBusesLayout())
    , _parameter_tree(*this, nullptr, juce::Identifier(JucePlugin_Name), CreateParameterLayout())
{}
```

### Default JUCE Template
```cpp
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor (BusesProperties()
                    #if ! JucePlugin_IsMidiEffect
                     #if ! JucePlugin_IsSynth
                      .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                     #endif
                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                    #endif
                      ),
#else
    :
#endif
    parameters(*this, nullptr, juce::Identifier(jucePlugin_Name), 
    {
        std::make_unique<juce::AudioParameterFloat>("p_float", "Float", 
                                                    juce::NormalisableRange{ 0.F, 1.F, 0.1F, 1.f, false}, 
                                                    0.F),
        std::make_unique<juce::AudioParameterInt>("p_int", "Integer", 
                                                  0, 10, 0)})
{}
```

# Build: CMake
``` cmake
  git clone --recursive https://github.com/SaturnDSP/JucePluginTemplate.git
  cd JucePluginTemplate

  mkdir build
  cd build
  cmake -S ../ -B .  -G Ninja -DCMAKE_BUILD_TYPE=Debug
  cmake --build . --config Release -j32
```



























