#include "vec.h"
#include <math.h>
using namespace vec;

double &vec2::operator[](size_t i)
{
	return *(&x + (i%2));
}

double &vec3::operator[](size_t i)
{
	return *(&x + (i%3));
}

double &vec4::operator[](size_t i)
{
	return *(&x + (i % 4));
}


vec2 vec::operator+(const vec2& lhs, const vec2& rhs)
{
	return vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}
vec2 vec::operator+(const vec2& lhs, const double rhs)
{
	return vec2(lhs.x + rhs, lhs.y + rhs);
}
vec2 vec::operator+(const double lhs, const vec2& rhs)
{
	return rhs + lhs;
}

vec2 vec::operator-(const vec2& lhs, const vec2& rhs)
{
	return vec2(lhs.x - rhs.x, lhs.y - rhs.y);
}
vec2 vec::operator-(const vec2& lhs, const double rhs)
{
	return vec2(lhs.x - rhs, lhs.y - rhs);
}
vec2 vec::operator-(const double lhs, const vec2& rhs)
{
	return vec2(lhs - rhs.x, lhs - rhs.y);
}

vec2 vec::operator*(const vec2& lhs, const vec2& rhs)
{
	return vec2(lhs.x * rhs.x, lhs.y * rhs.y);
}
vec2 vec::operator*(const vec2& lhs, const double rhs)
{
	return vec2(lhs.x * rhs, lhs.y * rhs);
}
vec2 vec::operator*(const double lhs, const vec2& rhs)
{
	return rhs * lhs;
}

vec2 vec::operator/(const vec2& lhs, const vec2& rhs)
{
	return vec2(lhs.x / rhs.x, lhs.y / rhs.y);
}
vec2 vec::operator/(const vec2& lhs, const double rhs)
{
	return lhs * (1.0 / rhs);
}
vec2 vec::operator/(const double lhs, const vec2& rhs)
{
	return vec2(lhs / rhs.x, lhs / rhs.y);
}


vec3 vec::operator+(const vec3& lhs, const vec3& rhs)
{
	return vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
vec3 vec::operator+(const vec3& lhs, const double rhs)
{
	return vec3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
}
vec3 vec::operator+(const double lhs, const vec3& rhs)
{
	return rhs + lhs;
}

vec3 vec::operator-(const vec3& lhs, const vec3& rhs)
{
	return vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
vec3 vec::operator-(const vec3& lhs, const double rhs)
{
	return vec3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
}
vec3 vec::operator-(const double lhs, const vec3& rhs)
{
	return vec3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
}

vec3 vec::operator*(const vec3& lhs, const vec3& rhs)
{
	return vec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}
vec3 vec::operator*(const vec3& lhs, const double rhs)
{
	return vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}
vec3 vec::operator*(const double lhs, const vec3& rhs)
{
	return rhs * lhs;
}

vec3 vec::operator/(const vec3& lhs, const vec3& rhs)
{
	return vec3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}
vec3 vec::operator/(const vec3& lhs, const double rhs)
{
	return lhs * (1.0 / rhs);
}
vec3 vec::operator/(const double lhs, const vec3& rhs)
{
	return vec3(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
}


vec4 vec::operator+(const vec4& lhs, const vec4& rhs)
{
	return vec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
vec4 vec::operator+(const vec4& lhs, const double rhs)
{
	return vec4(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
}
vec4 vec::operator+(const double lhs, const vec4& rhs)
{
	return rhs + lhs;
}

vec4 vec::operator-(const vec4& lhs, const vec4& rhs)
{
	return vec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
vec4 vec::operator-(const vec4& lhs, const double rhs)
{
	return vec4(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
}
vec4 vec::operator-(const double lhs, const vec4& rhs)
{
	return vec4(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w);
}

vec4 vec::operator*(const vec4& lhs, const vec4& rhs)
{
	return vec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}
vec4 vec::operator*(const vec4& lhs, const double rhs)
{
	return vec4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}
vec4 vec::operator*(const double lhs, const vec4& rhs)
{
	return rhs * lhs;
}

vec4 vec::operator/(const vec4& lhs, const vec4& rhs)
{
	return vec4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}
vec4 vec::operator/(const vec4& lhs, const double rhs)
{
	return lhs * (1.0 / rhs);
}
vec4 vec::operator/(const double lhs, const vec4& rhs)
{
	return vec4(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w);
}

double vec::dot(vec2 a, vec2 b)
{
	return a.x * b.x + a.y * b.y;
}

double vec::length(vec2 v)
{
	return sqrt(dot(v, v));
}

vec2 vec::unit(double theta)
{
	return vec2(cos(theta), sin(theta));
}

vec2 vec::rotate(vec2 point, double theta)
{
	vec2 cs = unit(theta);
	return point.x * vec2(cs.x, cs.y) + point.y * vec2(-cs.y, cs.x);
}

vec4 vec::xyRotate(vec4 point, double theta)
{
	vec2 xy(point.x, point.y);
	xy = rotate(xy, theta);
	return vec4(xy.x, xy.y, point.z, point.w);
}

vec4 vec::xzRotate(vec4 point, double theta)
{
	vec2 xz(point.x, point.z);
	xz = rotate(xz, theta);
	return vec4(xz.x, point.y, xz.y, point.w);
}

vec4 vec::xwRotate(vec4 point, double theta)
{
	vec2 xw(point.x, point.w);
	xw = rotate(xw, theta);
	return vec4(xw.x, point.y, point.z, xw.y);
}

vec4 vec::yzRotate(vec4 point, double theta)
{
	vec2 yz(point.y, point.z);
	yz = rotate(yz, theta);
	return vec4(point.x, yz.x, yz.y, point.w);
}

vec4 vec::ywRotate(vec4 point, double theta)
{
	vec2 yw(point.y, point.w);
	yw = rotate(yw, theta);
	return vec4(point.x, yw.x, point.z, yw.y);
}

vec4 vec::zwRotate(vec4 point, double theta)
{
	vec2 zw(point.z, point.w);
	zw = rotate(zw, theta);
	return vec4(point.x, point.y, zw.x, zw.y);
}