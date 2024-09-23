#include "screen.h"

#include <iostream>

void UiScreen::Update(UiInt width, UiInt height)
{
    m_dimensions = {width, height};
    // TODO: Update projection matrix on screen update.
}

UiInt UiScreen::GetWidth() const
{
    return m_dimensions.x;
}

UiInt UiScreen::GetHeight() const
{
    return m_dimensions.y;
}