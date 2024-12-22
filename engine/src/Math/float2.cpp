#include "Math/float2.h"

float2::float2(){
  m_dirty = true;
  x = 0.0f;
  y = 0.0f;
}

float2::float2(float value){
  m_dirty = true;
  x = value;
  y = value;
}

float2::float2(float x, float y) {
  m_dirty = true;
  this->x = x;
  this->y = y;
}

float2 float2::operator-() const { return float2(-x, -y); }

float2 float2::operator+(const float2 &v) const {
  return float2(x + v.x, y + v.y);
}

float2 float2::operator-(const float2 &v) const {
  return float2(x - v.x, y - v.y);
}

float2 float2::operator*(const float2 &v) const {
  return float2(x * v.x, y * v.y);
}

float2 float2::operator/(const float2 &v) const {
  return float2(x / v.x, y / v.y);
}

float2 &float2::operator+=(const float2 &v) {
  this->x += v.x;
  this->y += v.y;
  this->m_dirty = true;
  return *this;
}

float2 &float2::operator-=(const float2 &v) {
  this->x -= v.x;
  this->y -= v.y;
  this->m_dirty = true;
  return *this;
}

float2 &float2::operator*=(const float2 &v) {
  this->x *= v.x;
  this->y *= v.y;
  this->m_dirty = true;
  return *this;
}

float2 &float2::operator/=(const float2 &v) {
  this->x /= v.x;
  this->y /= v.y;
  this->m_dirty = true;
  return *this;
}
