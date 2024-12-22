#pragma once
#include <DirectXMath.h>
#include <nlohmann/json.hpp>
using namespace DirectX;
class int2 : public XMINT2 {
public:
  int2();
  int2(int32_t value);
  int2(int32_t x, int32_t y);

  int2 operator-() const;
  int2 operator+(const int2 &v) const;
  int2 operator-(const int2 &v) const;
  int2 operator*(const int2 &v) const;
  int2 operator/(const int2 &v) const;

  int2 &operator+=(const int2 &v);
  int2 &operator-=(const int2 &v);
  int2 &operator*=(const int2 &v);
  int2 &operator/=(const int2 &v);

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(int2, x, y)
private:
  int2(int32_t x, int32_t y, bool dirty);

private:
  bool m_dirty;
};