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

int UiStorage::GetInt(UiId key)
{
    std::vector<UiPair>::iterator it = std::lower_bound(data.begin(), data.end(), key);

    if (it == data.end() || it->key != key)
    {
        return 0;
    }

    return it->val_i;
}

void UiStorage::SetFloat(UiId key, float val)
{
    data.push_back(UiPair{key, val});
}

float UiStorage::GetFloat(UiId key)
{
    std::vector<UiPair>::iterator it = std::lower_bound(data.begin(), data.end(), key);

    if (it == data.end() || it->key != key)
    {
        return 0.0f;
    }

    return it->val_f;
}

void UiStorage::SetVoidPtr(UiId key, void *val)
{
    data.push_back(UiPair{key, val});
}

void *UiStorage::GetVoidPtr(UiId key)
{
    std::vector<UiPair>::iterator it = std::lower_bound(data.begin(), data.end(), key);

    if (it == data.end() || it->key != key)
    {
        return nullptr;
    }

    return it->val_p;
}