#pragma once
#include <DirectXMath.h>
#include <nlohmann/json.hpp>

using namespace DirectX;

class alignas(16) float2 {
public:
  float2();
  explicit float2(float value);
  float2(float x, float y);

  float2 operator-() const;
  float2 operator+(const float2 &v) const;
  float2 operator-(const float2 &v) const;
  float2 operator*(const float2 &v) const;
  float2 operator/(const float2 &v) const;

  float2 &operator+=(const float2 &v);
  float2 &operator-=(const float2 &v);
  float2 &operator*=(const float2 &v);
  float2 &operator/=(const float2 &v);

public:
  float x, y;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(float2, x, y)
  
private:
  bool m_dirty;
};