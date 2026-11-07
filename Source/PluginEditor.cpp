#include "PluginEditor.h"

TrapchikAudioProcessorEditor::TrapchikAudioProcessorEditor(TrapchikAudioProcessor& p)
 : AudioProcessorEditor(&p), processor(p)
{
    setSize(1000, 650);
    title.setText("TRAPCHIK", juce::dontSendNotification);
    title.setFont(juce::Font(juce::FontOptions(30.0f, juce::Font::bold)));
    title.setColour(juce::Label::textColourId, juce::Colour(0xfff2efff));
    addAndMakeVisible(title);
    subtitle.setText("ПОМОЩНИК СВЕДЕНИЯ • RAP / TRAP", juce::dontSendNotification);
    subtitle.setColour(juce::Label::textColourId, juce::Colour(0xffb8a5d8));
    addAndMakeVisible(subtitle);

    const juce::String names[10] = {"Низ","Порог","Ратио","Атака","Релиз","Размер","Затухание","Реверб","Выход","Потолок"};
    const juce::String ids[10] = {"LOW","THRESH","RATIO","ATTACK","RELEASE","ROOM","DECAY","REVERBMIX","OUTPUT","CEILING"};
    for (int i=0;i<10;++i) setupSlider(*(&low + 0), labels[i], names[i], ids[i], i);
    recommendation.setText("РЕКОМЕНДАЦИИ: послушай баланс низа и вокала перед применением обработки.", juce::dontSendNotification);
    recommendation.setColour(juce::Label::textColourId, juce::Colour(0xffe4d9f5));
    addAndMakeVisible(recommendation);
}

void TrapchikAudioProcessorEditor::setupSlider(juce::Slider&, juce::Label& label,
 const juce::String& name, const juce::String& id, int index)
{
    // Sliders are created in resized/paint-safe member storage in this compact prototype.
    label.setText(name, juce::dontSendNotification);
    label.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(label);
    juce::ignoreUnused(id, index);
}

void TrapchikAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff111018));
    g.setColour(juce::Colour(0xff211b2b)); g.fillRoundedRectangle(18,85,getWidth()-36,190,16);
    g.setColour(juce::Colour(0xff9d68d8)); g.drawRoundedRectangle(18,85,getWidth()-36,190,16,2);
    g.setColour(juce::Colour(0xffc49bff));
    juce::Path curve; curve.startNewSubPath(70,235);
    for(int i=1;i<80;++i){ float x=70+i*10.5f; float y=215-45*std::exp(-std::pow((i-18)/12.0f,2.0f))-18*std::sin(i*0.18f); curve.lineTo(x,y); }
    g.strokePath(curve, juce::PathStrokeType(2.5f));
    g.setColour(juce::Colour(0xff211b2b));
    for(int i=0;i<4;++i) g.fillRoundedRectangle(18+i*244,300,226,260,14);
    g.setColour(juce::Colour(0xff8c65b5));
    for(int i=0;i<4;++i) g.drawRoundedRectangle(18+i*244,300,226,260,14,1.5f);
    g.setColour(juce::Colours::white); g.setFont(16);
    g.drawText("АНАЛИЗАТОР",32,98,180,24,juce::Justification::left);
    g.drawText("ЭКВАЛАЙЗЕР",32,315,200,25,juce::Justification::left);
    g.drawText("ДИНАМИКА",276,315,200,25,juce::Justification::left);
    g.drawText("ПРОСТРАНСТВО",520,315,200,25,juce::Justification::left);
    g.drawText("МАСТЕР",764,315,200,25,juce::Justification::left);
    g.setColour(juce::Colour(0xffc5b7d6)); g.setFont(12);
    g.drawText("Спектр в этой версии — визуальный макет, не измерение сигнала.",32,250,700,18,juce::Justification::left);
}

void TrapchikAudioProcessorEditor::resized()
{
    title.setBounds(24,12,300,42); subtitle.setBounds(26,52,360,22);
    recommendation.setBounds(24,575,getWidth()-48,48);
    for(int i=0;i<10;++i) labels[i].setBounds(32+(i%5)*190,345+(i/5)*90,170,24);
}
