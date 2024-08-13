#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <stack>
#include <vector>

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

    UiVec2I operator+=(UiVec2I other);
    UiVec2I operator-();
};

UiVec2I operator+(UiVec2I lhs, const UiVec2I &rhs);
UiVec2I operator-(UiVec2I lhs, UiVec2I rhs);

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

using UiId = size_t;

template <typename T>
using UiStack = std::stack<T>;

struct UiPair
{
    UiId key;
    union
    {
        int val_i;
        float val_f;
        void *val_p;
    };

    UiPair(UiId _key, int _val);
    UiPair(UiId _key, float _val);
    UiPair(UiId _key, void *_val);
};

bool operator<(UiPair pair, uint64_t val);

struct UiStorage
{
    std::vector<UiPair> data;

    void SetInt(UiId key, int val);
    int GetInt(UiId key);

    void SetFloat(UiId key, float val);
    float GetFloat(UiId key);

    void SetVoidPtr(UiId key, void *val);
    void *GetVoidPtr(UiId key);
};

#endif // TYPES_H