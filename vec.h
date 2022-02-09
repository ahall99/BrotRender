// Simple vector classes/helpers. Elementary operations are component-wise.

#pragma once

namespace vec
{
	struct vec2
	{
		vec2() : x(0.0), y(0.0) {}
		vec2(double d) : x(d), y(d) {}
		vec2(double x, double y) : x(x), y(y) {}

		double &operator[](size_t i);

		double x, y;
	};

	struct vec3
	{
		vec3() : x(0.0), y(0.0), z(0.0) {}
		vec3(double d) : x(d), y(d), z(d) {}
		vec3(double x, double y, double z) : x(x), y(y), z(z) {}

		double &operator[](size_t i);

		double x, y, z;
	};

	struct vec4
	{
		vec4() : x(0.0), y(0.0), z(0.0) {}
		vec4(double d) : x(d), y(d), z(d) {}
		vec4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

		double &operator[](size_t i);

		double x, y, z, w;
	};

	vec2 operator+(const vec2& lhs, const vec2& rhs);
	vec2 operator+(const vec2& lhs, const double rhs);
	vec2 operator+(const double lhs, const vec2& rhs);

	vec2 operator-(const vec2& lhs, const vec2& rhs);
	vec2 operator-(const vec2& lhs, const double rhs);
	vec2 operator-(const double lhs, const vec2& rhs);

	vec2 operator*(const vec2& lhs, const vec2& rhs);
	vec2 operator*(const vec2& lhs, const double rhs);
	vec2 operator*(const double lhs, const vec2& rhs);

	vec2 operator/(const vec2& lhs, const vec2& rhs);
	vec2 operator/(const vec2& lhs, const double rhs);
	vec2 operator/(const double lhs, const vec2& rhs);

	vec3 operator+(const vec3& lhs, const vec3& rhs);
	vec3 operator+(const vec3& lhs, const double rhs);
	vec3 operator+(const double lhs, const vec3& rhs);

	vec3 operator-(const vec3& lhs, const vec3& rhs);
	vec3 operator-(const vec3& lhs, const double rhs);
	vec3 operator-(const double lhs, const vec3& rhs);

	vec3 operator*(const vec3& lhs, const vec3& rhs);
	vec3 operator*(const vec3& lhs, const double rhs);
	vec3 operator*(const double lhs, const vec3& rhs);

	vec3 operator/(const vec3& lhs, const vec3& rhs);
	vec3 operator/(const vec3& lhs, const double rhs);
	vec3 operator/(const double lhs, const vec3& rhs);

	vec4 operator+(const vec4& lhs, const vec4& rhs);
	vec4 operator+(const vec4& lhs, const double rhs);
	vec4 operator+(const double lhs, const vec4& rhs);

	vec4 operator-(const vec4& lhs, const vec4& rhs);
	vec4 operator-(const vec4& lhs, const double rhs);
	vec4 operator-(const double lhs, const vec4& rhs);

	vec4 operator*(const vec4& lhs, const vec4& rhs);
	vec4 operator*(const vec4& lhs, const double rhs);
	vec4 operator*(const double lhs, const vec4& rhs);

	vec4 operator/(const vec4& lhs, const vec4& rhs);
	vec4 operator/(const vec4& lhs, const double rhs);
	vec4 operator/(const double lhs, const vec4& rhs);

	double dot(vec2 a, vec2 b);
	//double dot(vec3 a, vec3, b);
	//double dot(vec4 a, vec4 b);

	double length(vec2 v);
	//double length(vec3 v);
	//double length(vec4 v);
	
	vec2 unit(double theta);
	vec2 rotate(vec2 point, double theta);

	vec4 xyRotate(vec4 point, double theta);
	vec4 xzRotate(vec4 point, double theta);
	vec4 xwRotate(vec4 point, double theta);
	vec4 yzRotate(vec4 point, double theta);
	vec4 ywRotate(vec4 point, double theta);
	vec4 zwRotate(vec4 point, double theta);
}