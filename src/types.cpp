#include "stacked/types.h"

UiVec2I UiVec2I::operator+=(UiVec2I other)
{
    x += other.x;
    y += other.y;
    return *this;
}

UiVec2I UiVec2I::operator-()
{
    x = -x;
    y = -y;
    return *this;
}

UiVec2I operator+(UiVec2I lhs, const UiVec2I &rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

UiVec2I operator-(UiVec2I lhs, UiVec2I rhs)
{
    return lhs + -rhs;
}

UiPair::UiPair(UiId _key, int _val)
    : key{_key},
      val_i{_val}
{
}

UiPair::UiPair(UiId _key, float _val)
    : key{_key},
      val_f{_val}
{
}

UiPair::UiPair(UiId _key, void *_val)
    : key{_key},
      val_p{_val}
{
}

bool operator<(UiPair pair, uint64_t val)
{
    return pair.key < val;
}

void UiStorage::SetInt(UiId key, int val)
{
    data.push_back(UiPair{key, val});
}

int UiStorage::GetInt(UiId key) const
{
    for (const auto &item : data)
    {
        if (item.key == key)
            return item.val_i;
    }

    return 0;
}

void UiStorage::SetFloat(UiId key, float val)
{
    data.push_back(UiPair{key, val});
}

float UiStorage::GetFloat(UiId key) const
{
    for (const auto &item : data)
    {
        if (item.key == key)
            return item.val_f;
    }

    return 0.0f;
}

void UiStorage::SetVoidPtr(UiId key, void *val)
{
    data.push_back(UiPair{key, val});
}

void *UiStorage::GetVoidPtr(UiId key) const
{
    for (const auto &item : data)
    {
        if (item.key == key)
            return item.val_p;
    }

    return nullptr;
}

size_t UiStorage::GetSize() const
{
    return data.size();
}