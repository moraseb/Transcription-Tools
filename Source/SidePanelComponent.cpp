#include "../JuceLibraryCode/JuceHeader.h"
#include "SidePanelComponent.h"

SidePanelComponent::SidePanelComponent(MainAudioComponent& audioCompToUse) : audioComp(audioCompToUse)
{
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
    int value = tempoEditor.getText().getIntValue();
    if(value) {
      audioComp.rhythmComp.setTempo(value);
      audioComp.rhythmComp.repaint();
    }
  };
  tempoEditor.onFocusLost = [this] {
    tempoEditor.setText(String(audioComp.rhythmComp.getTempo()));
  };
  
  
  addAndMakeVisible(&numeratorEditor);
  addAndMakeVisible(&denominatorEditor);
  addAndMakeVisible(&repetitionEditor);
  addAndMakeVisible(&tempoEditor);

}

SidePanelComponent::~SidePanelComponent()
{
}

void SidePanelComponent::paint (Graphics& g)
{
  // Lightgrey Colour(80, 90, 100)
  // Grey Colour(55, 60, 67)
  // White Colour(200, 200, 200)
  
  // Background color
  g.fillAll(Colour(55, 60, 67));
  
  g.setColour(Colour(200, 200, 200));
  
  Font font = g.getCurrentFont();
  font.setItalic(true);
  g.setFont(font);
  
  /*/ Balance section
  g.drawText("Balance", 0, 0, getWidth(), 24, Justification::centred);
  g.drawText("L", 10, 24, getWidth() - 20, 25, Justification::centredLeft);
  g.drawText("R", 10, 24, getWidth() - 20, 25, Justification::centredRight);
   */
  
  // Rythm Section
  g.drawText("Rhythm", 0, 0, getWidth(), 24, Justification::centred);
  
  font.setItalic(false);
  g.setFont(font);
  
  g.drawText("Tempo:", 10, 28, 100, 20, Justification::centredLeft);
  g.drawText("Time Signature:", 10, 48, 100, 20, Justification::centredLeft);
  g.drawText("Repetitions:", 10, 88, 100, 20, Justification::centredLeft);
  
  // Dividing lines
  g.setColour(Colour(80, 90, 100));
  g.fillRect(0, 24, getWidth(), 1);
  g.fillRect(0, 112, getWidth(), 1);
  //g.fillRect(0, 48, getWidth(), 1);
  //g.fillRect(0, 72, getWidth(), 1);
}

void SidePanelComponent::resized()
{
  tempoEditor.setBounds(getWidth() - 50, 28, 40, 20);
  numeratorEditor.setBounds(getWidth() - 30, 48, 20, 20);
  denominatorEditor.setBounds(getWidth() - 30, 68, 20, 20);
  repetitionEditor.setBounds(getWidth() - 40, 88, 30, 20);
  
}

void SidePanelComponent::mouseDown(const MouseEvent& event) {
  getParentComponent()->getParentComponent()->grabKeyboardFocus();
}
