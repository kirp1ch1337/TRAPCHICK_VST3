#include "PluginProcessor.h"
#include "PluginEditor.h"

static juce::AudioProcessorValueTreeState::ParameterLayout createLayout()
{
    using P = juce::AudioProcessorValueTreeState;
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> p;
    auto add = [&p](const juce::String& id, const juce::String& name,
                    float lo, float hi, float def, float step = 0.01f)
    {
        p.push_back(std::make_unique<juce::AudioParameterFloat>(
            juce::ParameterID{id, 1}, name,
            juce::NormalisableRange<float>(lo, hi, step), def));
    };
    add("LOW", "Низ", -12, 12, 0);
    add("MID", "Середина", -12, 12, 0);
    add("HIGH", "Верх", -12, 12, 0);
    add("THRESH", "Порог", -36, 0, -18);
    add("RATIO", "Ратио", 1, 10, 3);
    add("ATTACK", "Атака", 1, 100, 10);
    add("RELEASE", "Релиз", 10, 500, 80);
    add("ROOM", "Размер", 0, 100, 38);
    add("DECAY", "Затухание", 0.1f, 5, 1.6f);
    add("REVERBMIX", "Микс реверба", 0, 100, 15);
    add("OUTPUT", "Выход", -18, 6, 0);
    add("CEILING", "Потолок", -6, 0, -0.1f);
    return {p.begin(), p.end()};
}

TrapchikAudioProcessor::TrapchikAudioProcessor()
 : AudioProcessor(BusesProperties().withInput("Вход", juce::AudioChannelSet::stereo(), true)
                                  .withOutput("Выход", juce::AudioChannelSet::stereo(), true)),
   parameters(*this, nullptr, "PARAMETERS", createLayout())
{
}

bool TrapchikAudioProcessor::isBusesLayoutSupported(const BusesLayout& l) const
{
    return l.getMainInputChannelSet() == l.getMainOutputChannelSet()
        && (l.getMainOutputChannelSet() == juce::AudioChannelSet::mono()
         || l.getMainOutputChannelSet() == juce::AudioChannelSet::stereo());
}

void TrapchikAudioProcessor::prepareToPlay(double sr, int block)
{
    juce::dsp::ProcessSpec spec{sr, (juce::uint32) block, (juce::uint32)getTotalNumOutputChannels()};
    chain.prepare(spec);
    limiter.prepare(spec);
    juce::dsp::Reverb::Parameters rp;
    rp.roomSize = 0.38f; rp.damping = 0.5f; rp.wetLevel = 0.15f; rp.dryLevel = 0.85f;
    rp.width = 1.0f; rp.freezeMode = 0.0f;
    chain.get<2>().setParameters(rp);
}

void TrapchikAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    auto low = parameters.getRawParameterValue("LOW")->load();
    auto high = parameters.getRawParameterValue("HIGH")->load();
    auto thresh = parameters.getRawParameterValue("THRESH")->load();
    auto ratio = parameters.getRawParameterValue("RATIO")->load();
    auto attack = parameters.getRawParameterValue("ATTACK")->load();
    auto release = parameters.getRawParameterValue("RELEASE")->load();
    auto room = parameters.getRawParameterValue("ROOM")->load() / 100.0f;
    auto decay = parameters.getRawParameterValue("DECAY")->load();
    auto wet = parameters.getRawParameterValue("REVERBMIX")->load() / 100.0f;
    auto output = parameters.getRawParameterValue("OUTPUT")->load();
    auto ceiling = parameters.getRawParameterValue("CEILING")->load();

    auto coeff = juce::dsp::IIR::Coefficients<float>::makeLowShelf(getSampleRate(), 120.0, 0.707,
                      juce::Decibels::decibelsToGain(low));
    *chain.get<0>().coefficients = *coeff;
    auto& comp = chain.get<1>();
    comp.setThreshold(thresh); comp.setRatio(ratio);
    comp.setAttack(attack); comp.setRelease(release);
    juce::dsp::Reverb::Parameters rp;
    rp.roomSize = room; rp.damping = 0.5f; rp.wetLevel = wet;
    rp.dryLevel = 1.0f - wet; rp.width = 1.0f; rp.freezeMode = 0.0f;
    chain.get<2>().setParameters(rp);

    juce::dsp::AudioBlock<float> blockAudio(buffer);
    juce::dsp::ProcessContextReplacing<float> context(blockAudio);
    chain.process(context);
    auto gain = juce::Decibels::decibelsToGain(output);
    buffer.applyGain(gain);
    limiter.setThreshold(juce::Decibels::decibelsToGain(ceiling));
    limiter.process(context);
    juce::ignoreUnused(high); // High-band control is reserved for the next DSP pass.
}

juce::AudioProcessorEditor* TrapchikAudioProcessor::createEditor()
{ return new TrapchikAudioProcessorEditor(*this); }

void TrapchikAudioProcessor::getStateInformation(juce::MemoryBlock& dest)
{ auto state = parameters.copyState(); std::unique_ptr<juce::XmlElement> xml(state.createXml()); copyXmlToBinary(*xml, dest); }

void TrapchikAudioProcessor::setStateInformation(const void* data, int size)
{ std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, size)); if (xml && xml->hasTagName(parameters.state.getType())) parameters.replaceState(juce::ValueTree::fromXml(*xml)); }

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{ return new TrapchikAudioProcessor(); }
double TrapchikAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}