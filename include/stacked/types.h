#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

typedef float UiFloat;
typedef int32_t UiInt;

struct UiVec2I
{
    union
    {
        UiInt x, w;
    };

    union
    {
        UiInt y, h;
    };
};

struct UiVec2F
{
    union
    {
        UiFloat x, w;
    };

    union
    {
        UiFloat y, h;
    };
};

struct UiVec3I
{
    union
    {
        UiInt x, w;
    };

    union
    {
        UiInt y, h;
    };

    UiInt z;
};

struct UiVec3F
{
    union
    {
        UiFloat x, w;
    };

    union
    {
        UiFloat y, h;
    };

    UiFloat z;
};

struct UiVec4I
{
    union
    {
        UiInt x, r;
    };
    union
    {
        UiInt y, g;
    };
    union
    {
        UiInt z, b;
    };
    union
    {
        UiInt w, a;
    };
};

struct UiVec4F
{
    union
    {
        UiFloat x, r;
    };
    union
    {
        UiFloat y, g;
    };
    union
    {
        UiFloat z, b;
    };
    union
    {
        UiFloat w, a;
    };
};

#endif // TYPES_H
