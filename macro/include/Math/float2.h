#pragma once
#include <DirectXMath.h>

using namespace DirectX;
class float2 : public XMFLOAT2A {
public:
    float2();
    float2(float value);
    float2(float x, float y);

    float2 operator-() const;
    float2 operator+(const float2& v) const;
    float2 operator-(const float2& v) const;
    float2 operator*(const float2& v) const;
    float2 operator/(const float2& v) const;

    float2& operator+=(const float2& v);
    float2& operator-=(const float2& v);
    float2& operator*=(const float2& v);
    float2& operator/=(const float2& v);

private:
    float2(float x, float y, bool dirty);
private:
    bool m_dirty;
};