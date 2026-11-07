#pragma once
#include "PluginProcessor.h"

class TrapchikAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit TrapchikAudioProcessorEditor(TrapchikAudioProcessor&);
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    TrapchikAudioProcessor& processor;
    juce::Label title, subtitle, recommendation;
    juce::Slider low, thresh, ratio, attack, release, room, decay, reverbMix, output, ceiling;
    juce::Label labels[10];
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachments[10];
    void setupSlider(juce::Slider&, juce::Label&, const juce::String&, const juce::String&, int);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrapchikAudioProcessorEditor)
};
