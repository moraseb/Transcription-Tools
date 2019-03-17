/*
  ==============================================================================

    ResizeBarComponent.h
    Created: 22 Dec 2018 4:17:56pm
    Author:  Sebastian Mora

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ResizeBarComponent    : public Component
{
public:
    ResizeBarComponent();
    ~ResizeBarComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResizeBarComponent)
};
