#include "../JuceLibraryCode/JuceHeader.h"
#include "SidePanelComponent.h"

SidePanelComponent::SidePanelComponent(MainAudioComponent& audioCompToUse) : audioComp(audioCompToUse),
monoButton()
{
  
  /* Info */
  fileName = "";
  positionLabel.setEditable(false);
  positionLabel.setInterceptsMouseClicks(false, false);
  positionLabel.setJustificationType(Justification::centredRight);
  positionLabel.setText("", NotificationType::dontSendNotification);
  positionLabel.setColour(Label::ColourIds::textColourId, Colour(200, 200, 200));
  sectionLabel.setEditable(false);
  sectionLabel.setInterceptsMouseClicks(false, false);
  sectionLabel.setJustificationType(Justification::centredRight);
  sectionLabel.setText("", NotificationType::dontSendNotification);
  sectionLabel.setColour(Label::ColourIds::textColourId, Colour(200, 200, 200));
  
  /* Balance */
  balanceSlider.setRange(-100, 100, 1);
  balanceSlider.setValue(0);
  balanceSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
  balanceSlider.setDoubleClickReturnValue(true, 0);
  balanceSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
  balanceSlider.setPopupDisplayEnabled(true, true, nullptr);
  balanceSlider.setColour(Slider::thumbColourId, Colour(85, 135, 215));
  balanceSlider.setColour(Slider::trackColourId, Colour(80, 90, 100));
  balanceSlider.setColour(Slider::backgroundColourId, Colour(35, 34, 35));
  
  /* Rhythm */
  numeratorEditor.setText("4");
  numeratorEditor.setInputRestrictions(0, "0123456789");
  numeratorEditor.onReturnKey = [this] {
    int value = numeratorEditor.getText().getIntValue();
    if(value) {
      audioComp.rhythmComp.setNumerator(value);
      audioComp.rhythmComp.repaint();
    }
  };
  numeratorEditor.onFocusLost = [this] {
    numeratorEditor.setText(String(audioComp.rhythmComp.getNumerator()));
  };
  
  denominatorEditor.setText("4");
  denominatorEditor.setInputRestrictions(0, "0123456789");
  denominatorEditor.onReturnKey = [this] {
    int value = denominatorEditor.getText().getIntValue();
    if(value) {
      audioComp.rhythmComp.setDenominator(value);
      audioComp.rhythmComp.repaint();
    }
  };
  denominatorEditor.onFocusLost = [this] {
    denominatorEditor.setText(String(audioComp.rhythmComp.getDenominator()));
  };
  
  repetitionEditor.setText("0");
  repetitionEditor.setInputRestrictions(0, "0123456789");
  repetitionEditor.onReturnKey = [this] {
    int value = repetitionEditor.getText().getIntValue();
    audioComp.rhythmComp.setRepetitions(value);
    audioComp.rhythmComp.repaint();
  };
  repetitionEditor.onFocusLost = [this] {
    repetitionEditor.setText(String(audioComp.rhythmComp.getRepetitions()));
  };
  
  tempoEditor.setText("120");
  tempoEditor.setInputRestrictions(0, ".0123456789");
  tempoEditor.onReturnKey = [this] {
    double value = tempoEditor.getText().getDoubleValue();
    if(value) {
      audioComp.rhythmComp.setTempo(value);
      audioComp.rhythmComp.repaint();
    }
  };
  tempoEditor.onFocusLost = [this] {
    tempoEditor.setText(String(audioComp.rhythmComp.getTempo()));
  };
  
  addAndMakeVisible(&positionLabel);
  addAndMakeVisible(&sectionLabel);
  
  addAndMakeVisible(&balanceSlider);
  addAndMakeVisible(&monoButton);
  addAndMakeVisible(&phaseButton);
  
  addAndMakeVisible(&numeratorEditor);
  addAndMakeVisible(&denominatorEditor);
  addAndMakeVisible(&repetitionEditor);
  addAndMakeVisible(&tempoEditor);

  startTimer(75);
}

SidePanelComponent::~SidePanelComponent()
{
}

void SidePanelComponent::paint (Graphics& g)
{
  // Lightgrey Colour(80, 90, 100)
  // Grey Colour(55, 60, 67)
  // Darker Grey Colour(28, 38, 43)
  // Dark Grey Colour(35, 34, 35)
  // White Colour(200, 200, 200)
  
  // Background color
  g.fillAll(Colour(55, 60, 67));
  
  // Rectangles for sections
  g.setColour(Colour(35, 34, 35));
  g.fillRect(0, 0, getWidth(), 24);
  g.fillRect(0, 114, getWidth(), 24);
  g.fillRect(0, 208, getWidth(), 24);
  
  // Dividing lines
  g.setColour(Colour(80, 90, 100));
  g.fillRect(0, 24, getWidth(), 1);
  
  g.fillRect(0, 114, getWidth(), 1);
  g.fillRect(0, 138, getWidth(), 1);
  
  g.fillRect(0, 208, getWidth(), 1);
  g.fillRect(0, 232, getWidth(), 1);
  
  g.fillRect(0, 322, getWidth(), 1);
  
  // Font
  Font font = g.getCurrentFont();
  g.setColour(Colour(200, 200, 200));
  
  // Info
  font.setItalic(true);
  g.setFont(font);
  g.drawText("Info", 0, 0, getWidth(), 24, Justification::centred);
  
  font.setItalic(false);
  g.setFont(font);
  g.drawText("Filename:", 10, 29, getWidth() - 20, 20, Justification::centredLeft);
  g.drawFittedText(fileName, 70, 32, getWidth() - 80, 40, Justification::topLeft, 2, 1.0);
  g.drawText("Position:", 10, 69, getWidth() - 20, 20, Justification::centredLeft);
  g.drawText("Section Length:", 10, 89, getWidth() - 20, 20, Justification::centredLeft);
  
  // Balance
  font.setItalic(true);
  g.setFont(font);
  g.drawText("Mix", 0, 114, getWidth(), 24, Justification::centred);
  
  font.setItalic(false);
  g.setFont(font);
  g.drawText("L", 10, 143, getWidth() - 20, 20, Justification::centredLeft);
  g.drawText("R", 10, 143, getWidth() - 20, 20, Justification::centredRight);
  g.drawText("Mono:", 10, 163, getWidth() - 20, 20, Justification::centredLeft);
  g.drawText("Invert Phase:", 10, 183, getWidth() - 20, 20, Justification::centredLeft);
  
  // Rhythm
  font.setItalic(true);
  g.setFont(font);
  g.drawText("Rhythm", 0, 208, getWidth(), 24, Justification::centred);
  
  font.setItalic(false);
  g.setFont(font);
  g.drawText("Tempo:", 10, 237, 100, 20, Justification::centredLeft);
  g.drawText("Time Signature:", 10, 257, 100, 20, Justification::centredLeft);
  g.drawText("Repetitions:", 10, 297, 100, 20, Justification::centredLeft);
  
}

void SidePanelComponent::resized()
{
  
  positionLabel.setBounds(getWidth() - 200, 69, 190, 20);
  sectionLabel.setBounds(getWidth() - 200, 89, 190, 20);
  
  balanceSlider.setBounds(20, 143, getWidth() - 40, 20);
  monoButton.setBounds(getWidth() - 35, 163, 25, 20);
  phaseButton.setBounds(getWidth() - 35, 183, 25, 20);
  
  tempoEditor.setBounds(getWidth() - 50, 237, 40, 20);
  numeratorEditor.setBounds(getWidth() - 30, 257, 20, 20);
  denominatorEditor.setBounds(getWidth() - 30, 277, 20, 20);
  repetitionEditor.setBounds(getWidth() - 40, 297, 30, 20);
  
}

void SidePanelComponent::mouseDown(const MouseEvent& event) {
  getParentComponent()->getParentComponent()->grabKeyboardFocus();
}

void SidePanelComponent::setFileName(String name) {
  fileName = name;
  repaint();
}

double SidePanelComponent::getBalance() {
  return balanceSlider.getValue();
}

bool SidePanelComponent::isMono() {
  return monoButton.getToggleState();
}

bool SidePanelComponent::isInverted() {
  return phaseButton.getToggleState();
}

void SidePanelComponent::timerCallback() {
  positionLabel.setText(String(audioComp.transportSource.getCurrentPosition()) +" / "
                        + String(audioComp.transportSource.getLengthInSeconds()) + "s",
                        NotificationType::dontSendNotification);
  double sectionLength = audioComp.getEndPosition() - audioComp.getStartPosition();
  if(audioComp.sectionComp.getWidth() > 0) {
    sectionLabel.setText(String(sectionLength) + "s", NotificationType::dontSendNotification);
  }
  else {
    sectionLabel.setText("0s", NotificationType::dontSendNotification);
  }
}
