#ifndef SCREEN_H
#define SCREEN_H

#include "stacked/types.h"

class UiScreen
{
public:
    UiScreen() = default;

    void Update(UiInt width, UiInt height);

    UiInt GetWidth() const;
    UiInt GetHeight() const;

private:
    UiVec2I m_dimensions;
};

#endif // SCREEN_H