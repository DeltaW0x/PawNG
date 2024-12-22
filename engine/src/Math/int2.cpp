#include "Math/int2.h"

int2::int2() : m_dirty(true) {
  x = 0.0f;
  y = 0.0f;
}

int2::int2(int32_t value) : m_dirty(true) {
  x = value;
  y = value;
}

int2::int2(int32_t x, int32_t y) : m_dirty(true) {
  this->x = x;
  this->y = y;
}

int2::int2(int32_t x, int32_t y, bool dirty) : m_dirty(dirty) {
  this->x = x;
  this->y = y;
}

int2 int2::operator-() const { return int2(-x, -y, true); }

int2 int2::operator+(const int2 &v) const {
  return int2(x + v.x, y + v.y, true);
}

int2 int2::operator-(const int2 &v) const {
  return int2(x - v.x, y - v.y, true);
}

int2 int2::operator*(const int2 &v) const {
  return int2(x * v.x, y * v.y, true);
}

int2 int2::operator/(const int2 &v) const {
  return int2(x / v.x, y / v.y, true);
}

int2 &int2::operator+=(const int2 &v) {
  this->x += v.x;
  this->y += v.y;
  this->m_dirty = true;
  return *this;
}

int2 &int2::operator-=(const int2 &v) {
  this->x -= v.x;
  this->y -= v.y;
  this->m_dirty = true;
  return *this;
}

int2 &int2::operator*=(const int2 &v) {
  this->x *= v.x;
  this->y *= v.y;
  this->m_dirty = true;
  return *this;
}

int2 &int2::operator/=(const int2 &v) {
  this->x /= v.x;
  this->y /= v.y;
  this->m_dirty = true;
  return *this;
}
