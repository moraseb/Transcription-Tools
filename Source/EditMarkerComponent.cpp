#include "../JuceLibraryCode/JuceHeader.h"
#include "EditMarkerComponent.h"

EditMarkerComponent::EditMarkerComponent(MarkerComponent* markerToEdit, double length) :
labelEditor(), descriptionEditor(),
positionSlider(Slider::SliderStyle::LinearHorizontal, Slider::TextEntryBoxPosition::TextBoxRight),
cancelButton("Cancel"), okButton("Ok")
{
  marker = markerToEdit;
  trackLength = length;
  
  labelEditor.setInputFilter(new TextEditor::LengthAndCharacterRestriction(1, String()), true);
  labelEditor.setText(marker->getLabel());
  descriptionEditor.setText(marker->getDescription());
  positionSlider.setRange(0, length, 0.01);
  positionSlider.setValue(marker->getPosition());
  positionSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
  positionSlider.setDoubleClickReturnValue(true, marker->getPosition());
  positionSlider.setPopupDisplayEnabled(false, false, nullptr);
  positionSlider.setTextValueSuffix("s");
  positionSlider.setColour(Slider::thumbColourId, Colour(85, 135, 215));
  positionSlider.setColour(Slider::trackColourId, Colour(80, 90, 100));
  positionSlider.setColour(Slider::backgroundColourId, Colour(35, 34, 35));
  
  cancelButton.onClick = [this] {
    
    // Closing window
    if (DialogWindow* dw = this->findParentComponentOfClass<DialogWindow>()) {
      dw->exitModalState(1234);
    }
    
  };
  
  okButton.onClick = [this] {
    
    // Updating values
    marker->setLabel(labelEditor.getText());
    marker->setDescription(descriptionEditor.getText());
    marker->setPosition(positionSlider.getValue());
    marker->getParentComponent()->resized();
    
    // Closing window
    if (DialogWindow* dw = this->findParentComponentOfClass<DialogWindow>()) {
      dw->exitModalState(1234);
    }
    
  };
  
  addAndMakeVisible(&labelEditor);
  addAndMakeVisible(&descriptionEditor);
  addAndMakeVisible(&positionSlider);
  addAndMakeVisible(&cancelButton);
  addAndMakeVisible(&okButton);
}

EditMarkerComponent::~EditMarkerComponent()
{
}

void EditMarkerComponent::paint (Graphics& g)
{
  g.fillAll(Colour(55, 60, 67));
  
  g.setColour(Colour(232, 232, 232));
  g.drawText("Label:", 10, 5, 80, 30, Justification::centredLeft);
  g.drawText("Description:", 10, 35, 80, 30, Justification::centredLeft);
  g.drawText("Position:", 10, 65, 80, 30, Justification::centredLeft);
}

void EditMarkerComponent::resized()
{
  labelEditor.setBounds(50, 10, 40, 20);
  descriptionEditor.setBounds(90, 40, 300, 20);
  positionSlider.setBounds(70, 70, 320, 20);
  cancelButton.setBounds(250, 105, 70, 25);
  okButton.setBounds(320, 105, 70, 25);
}

void EditMarkerComponent::mouseDown(const MouseEvent &event) {
  unfocusAllComponents();
}
