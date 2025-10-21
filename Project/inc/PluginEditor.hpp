#pragma once

#include "PluginProcessor.hpp"

struct Processor_Editor final
    : juce::AudioProcessorEditor
    , juce::Timer
{
    explicit Processor_Editor(Audio_Processor&);
    ~Processor_Editor() override;

private: /* Overrides */
    // ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════
    // Component
    void paint(juce::Graphics&) override;
    void paintOverChildren(juce::Graphics&) override;
    void resized() override;

    // ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════
    // Timer
    void timerCallback() override;

private: /* Members */
    Audio_Processor& _processor_ref;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Processor_Editor)
};
