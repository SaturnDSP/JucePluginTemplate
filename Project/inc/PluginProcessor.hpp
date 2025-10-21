#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

struct Audio_Processor final
    : juce::AudioProcessor
{
    Audio_Processor();
    ~Audio_Processor() override;

private: /* Constructor Helpers */
    // ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════
    // Buses
    static BusesProperties CreateBusesLayout()
    {
        BusesProperties layout;
        #if !JucePlugin_IsMidiEffect
            #if !JucePlugin_IsSynth
                layout = layout.withInput("Input", juce::AudioChannelSet::stereo(), true);
            #endif
            layout = layout.withOutput("Output", juce::AudioChannelSet::stereo(), true);
        #endif
        return layout;
    }

    // ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════
    // Parameters
    struct Parameter_Info;
    static juce::AudioProcessorValueTreeState::ParameterLayout CreateParameterLayout();

private: /* Overrides */
    // ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════
    // Processor Overrides
    void prepareToPlay(double sample_rate, int samples_per_block) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layout) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    // ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════
    // Editor Overrides
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    // ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════
    // Metadata Overrides
    const juce::String getName()  const override;
    bool acceptsMidi()            const override;
    bool producesMidi()           const override;
    bool isMidiEffect()           const override;
    double getTailLengthSeconds() const override;

    // ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════
    // Program Overrides
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& new_name) override;

    // ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════
    // State Overrides
    void getStateInformation(juce::MemoryBlock& dest_data) override;
    void setStateInformation(const void* data, int size_in_bytes) override;

    // ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════
    // Aa Overrides

private: /* Members */
    juce::AudioProcessorValueTreeState _parameter_tree;

    std::atomic<float> *_float_param  = nullptr;
    std::atomic<float> *_int_param    = nullptr;
    std::atomic<float> *_bool_param   = nullptr;
    std::atomic<float> *_choice_param = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Audio_Processor)
};
