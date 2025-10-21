#include "PluginProcessor.hpp"
#include "PluginEditor.hpp"

#define USE_PLUGIN_EDITOR true

/* ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
 * Parameters
 */

enum class ParamType {
    Float,
    Int,
    Bool,
    Choice,
    None,
};

struct Audio_Processor::Parameter_Info
{
    // TODO: refactor this and add some type safety.
    //       maybe with a few template structs and std::variant??

    ParamType type = ParamType::None;
    juce::ParameterID id;
    juce::String name;
    juce::NormalisableRange<float> range;
    float default_value = 0.F;
    juce::StringArray choices{"No Choice"};
    int default_choice = 0;

    std::unique_ptr<juce::RangedAudioParameter> CreateParameter()
    {
        switch(type) {
            case ParamType::Float:
                return std::make_unique<juce::AudioParameterFloat>(id,name, range, default_value);

            case ParamType::Int:
                return std::make_unique<juce::AudioParameterInt>(id, name, range.start, range.end, default_value);

            case ParamType::Bool:
                return std::make_unique<juce::AudioParameterBool>(id, name, default_value > 0.5f);

            case ParamType::Choice:
                return std::make_unique<juce::AudioParameterChoice>(id, name, choices, default_choice);

            case ParamType::None:
                std::cerr << "nullptr: No parameters?" << std::endl;
                std::abort();
        }
        return nullptr;
    }
};

juce::AudioProcessorValueTreeState::ParameterLayout Audio_Processor::CreateParameterLayout()
{
    std::vector< std::unique_ptr<juce::RangedAudioParameter> > params;

    {   ///< Float Param
        Parameter_Info param;
        param.type = ParamType::Float;
        param.id            = "p_float";
        param.name          = "Float";
        param.range         = { 0.0F, 1.0F, 0.1F, 1.0F };
        params.push_back( param.CreateParameter() );
    }

    {   ///< Int Param
        Parameter_Info param;
        param.type = ParamType::Int;
        param.id            = "p_integer";
        param.name          = "Integer";
        param.range         = { 0.0F, 10.0F };
        params.push_back( param.CreateParameter() );
    }

    {   ///< Bool Param
        Parameter_Info param;
        param.type = ParamType::Bool;
        param.id            = "p_boolean";
        param.name          = "Boolean";
        param.default_value = true;
        params.push_back( param.CreateParameter() );
    }

    {   ///< Choice Param
        Parameter_Info param;
        param.type = ParamType::Choice;
        param.id            = "p_choice";
        param.name          = "Choice";
        param.choices       = { "Cool Choice", "PeePee", "PooPoo" };
        param.default_choice = 0;
        params.push_back( param.CreateParameter() );
    }

    return { params.begin(), params.end() };
}

/* ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
 * Constructor
 */

Audio_Processor::Audio_Processor()
    : AudioProcessor(CreateBusesLayout())
    , _parameter_tree(*this, nullptr, juce::Identifier(JucePlugin_Name), CreateParameterLayout())
{
    _float_param  = _parameter_tree.getRawParameterValue("p_float");
    _int_param    = _parameter_tree.getRawParameterValue("p_integer");
    _bool_param   = _parameter_tree.getRawParameterValue("p_boolean");
    _choice_param = _parameter_tree.getRawParameterValue("p_choice");
}

Audio_Processor::~Audio_Processor() = default;

/* ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
 * Processor
 */

void Audio_Processor::prepareToPlay(const double sample_rate, const int samples_per_block)
{
    (void)sample_rate,
    (void)samples_per_block;
}

void Audio_Processor::releaseResources()
{
}

bool Audio_Processor::isBusesLayoutSupported(const BusesLayout& layout) const
{
#if JucePlugin_IsMidiEffect
    (void)layout;
    return true;
#else
    const bool
            supported = layout.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
                        && layout.getMainOutputChannelSet() != juce::AudioChannelSet::stereo();
    if(supported) {
        return false;
    }
#if ! JucePlugin_IsSynth
    if(layout.getMainOutputChannelSet() != layout.getMainInputChannelSet())
        return false;
#endif
    return true;
#endif
}

void Audio_Processor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi_messages)
{
    (void)midi_messages;
    juce::ScopedNoDenormals noDenormals;

    const auto total_num_inp_channels = getTotalNumInputChannels();
    const auto total_num_out_channels = getTotalNumOutputChannels();

    for(auto i = total_num_inp_channels; i < total_num_out_channels; ++i) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    for(int channel = 0; channel < total_num_inp_channels; ++channel)
    {
        const auto read = buffer.getReadPointer(channel);
        const auto write = buffer.getWritePointer(channel);
        for(int sample{0}; sample < buffer.getNumSamples(); ++ sample) {
            write[sample] = read[sample] * *_float_param;
        }
    }
}

/* ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
 * Editor
 */

juce::AudioProcessorEditor* Audio_Processor::createEditor()
{
#if USE_PLUGIN_EDITOR
    return new Processor_Editor(*this);
#else
    return new juce::GenericAudioProcessorEditor(*this);
#endif
}

bool Audio_Processor::hasEditor() const
{
    return true;
}

/* ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
 * Metadata
 */

const juce::String Audio_Processor::getName() const
{
    return JucePlugin_Name;
}

bool Audio_Processor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool Audio_Processor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool Audio_Processor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double Audio_Processor::getTailLengthSeconds() const
{
    return 0.0;
}

/* ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
 * Program
 */

int Audio_Processor::getNumPrograms()
{
    return 1;
}

int Audio_Processor::getCurrentProgram()
{
    return 0;
}

void Audio_Processor::setCurrentProgram(const int index)
{
    (void)index;
}

const juce::String Audio_Processor::getProgramName(const int index)
{
    (void)index;
    return {};
}

void Audio_Processor::changeProgramName(const int index, const juce::String& new_name)
{
    (void)index,
    (void)new_name;
}

/* ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
 * State
 */

void Audio_Processor::getStateInformation(juce::MemoryBlock& dest_data)
{
    const auto state = _parameter_tree.copyState();
    const std::unique_ptr xml(state.createXml());
    copyXmlToBinary(*xml, dest_data);
}

void Audio_Processor::setStateInformation(const void* data, const int size_in_bytes)
{
    const std::unique_ptr xmlState(getXmlFromBinary(data, size_in_bytes));
    if (xmlState != nullptr) {
        if ( xmlState->hasTagName(_parameter_tree.state.getType()) ) {
            const juce::ValueTree state = juce::ValueTree::fromXml(*xmlState);
            _parameter_tree.replaceState(state);
        }
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Audio_Processor();
}

/* ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════
 * END ═════════════════════════════════════════════════════════════════════════════════════════════════════════════════
 */
