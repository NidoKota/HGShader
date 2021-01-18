#pragma once

#include <iostream>
#include <math.h>
#include <time.h>
#include "Defines.hpp"

struct vec3
{
    public:
    float x;
    float y;
    float z;

    vec3();
    vec3(const float f);
    vec3(const float x, const float y);
    vec3(const float x, const float y, const float z);
    vec3 SetAll(const float f);
    vec3 SetAll(const float x, const float y);
    vec3 SetAll(const float x, const float y, const float z);
    hgcolor ToHGColor();
};

vec3 operator + (const vec3  l, const vec3  r);
vec3 operator + (const float l, const vec3  r);
vec3 operator + (const vec3  l, const float r);
vec3 operator - (const vec3  l, const vec3  r);
vec3 operator - (const float l, const vec3  r);
vec3 operator - (const vec3  l, const float r);
vec3 operator * (const vec3  l, const vec3  r);
vec3 operator * (const float l, const vec3  r);
vec3 operator * (const vec3  l, const float r);
vec3 operator / (const vec3  l, const vec3  r);
vec3 operator / (const float l, const vec3  r);
vec3 operator / (const vec3  l, const float r);
vec3 operator - (const vec3  l);
void operator += (vec3 &l, const vec3  r);
void operator += (vec3 &l, const float r);
void operator -= (vec3 &l, const vec3  r);
void operator -= (vec3 &l, const float r);
void operator *= (vec3 &l, const vec3  r);
void operator *= (vec3 &l, const float r);
void operator /= (vec3 &l, const vec3  r);
void operator /= (vec3 &l, const float r);
int operator == (const vec3 l, const vec3 r);
int operator != (const vec3 l, const vec3 r);
std::ostream& operator << (std::ostream& l, const vec3& r);

template <typename T> T radians(const T d);
template <typename T> T degrees(const T r);
vec3 abs(const vec3 x);
vec3 sign(const vec3 x);
float sign(const float x);
vec3 floor(const vec3 x);
vec3 ceil(const vec3 x);
template <typename T> T fract(const T x);
template <typename T> T mod(const T x, const T y);
float min(const float x, const float y);
vec3 min(const vec3 x, const vec3 y);
float max(const float x, const float y);
vec3 max(const vec3 x, const vec3 y);
template <typename T> T clamp(const T x, const T a, const T b);
template <typename T> T mix(const T x, const T y, const T a);
float step(const float a, const float x);
vec3 step(const vec3 a, const vec3 x);
template <typename T> T smoothstep(const T a, const T b, const T t);
vec3 sin(const vec3 x);
vec3 cos(const vec3 x);
vec3 tan(const vec3 x);
vec3 asin(const vec3 x);
vec3 acos(const vec3 x);
vec3 atan2(const vec3 y, const vec3 x);
vec3 pow(const vec3 x, const vec3 y);
vec3 exp(const vec3 x);
vec3 log(const vec3 x);
vec3 exp2(const vec3 x);
vec3 log2(const vec3 x);
vec3 sqrt(const vec3 x);
vec3 inverssqrt(const vec3 x);
float length(const vec3 x);
float distance(const vec3 x, const vec3 y);
vec3 normalize(const vec3 x);
float dot(const vec3 x, const vec3 y);
vec3 cross(const vec3 v1, const vec3 v2);
vec3 faceforward(const vec3 N, const vec3 I, const vec3 R);
vec3 reflect(const vec3 I, const vec3 N);
vec3 refract(const vec3 I, const vec3 N, const float eta);
vec3 lessThan(const vec3 x, const vec3 y);
vec3 lessThanEqual(const vec3 x, const vec3 y);
vec3 greaterThan(const vec3 x, const vec3 y);
vec3 greaterThanEqual(const vec3 x, const vec3 y);
vec3 equal(const vec3 x, const vec3 y);
vec3 notEqual(const vec3 x, const vec3 y);
float any(const vec3 x);
float all(const vec3 x);
vec3 _not(const vec3 x);
vec3 hgcolorToRGB(const hgcolor hgc);
vec3 random2(const vec3 uv);
float gradationNoise(const vec3 uv);
vec3 mul3x333(const vec3 x, const vec3 y[3]);
vec3 rotate(const vec3 uv, const float angle);
vec3 vecRotateX(const vec3 vec, const float angle);
vec3 vecRotateY(const vec3 vec, const float angle);
vec3 vecRotateZ(const vec3 vec, const float angle);
float gauss(const float a, const float x);
float lengthsqr(const vec3 x);
float roundN(const float num, const int n);
template <typename T> T remap(const T v, const T inMin, const T inMax, const T outMin, const T outMax);
vec3 screenSynthesis(const vec3 def, const vec3 syn);
vec3 rainbow(const float v);