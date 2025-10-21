#include "PluginProcessor.hpp"
#include "PluginEditor.hpp"

/* ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
 * Constructor
 */

Processor_Editor::Processor_Editor(Audio_Processor& parent_processor)
    : AudioProcessorEditor(&parent_processor)
    , _processor_ref(parent_processor)
{
    (void)_processor_ref;
    setSize(400, 300);
    setResizable(true, true);
    setResizeLimits(400, 300, 800, 600);
}

Processor_Editor::~Processor_Editor() = default;

/* ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
 * Paint
 */

void Processor_Editor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawFittedText("Wassup Bro", getLocalBounds(), juce::Justification::centred, 1);
}

void Processor_Editor::paintOverChildren(juce::Graphics &g)
{
    (void)g;
}

void Processor_Editor::resized()
{
}

/* ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
 * Timer
 */

void Processor_Editor::timerCallback()
{
}
