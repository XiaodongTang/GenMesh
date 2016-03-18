#ifndef VEC3_H
#define VEC3_H
#include <stdio.h>
#include <math.h>
#include <ostream>
#include <iostream>
class Vec2
{
public:
	inline Vec2(){}
	inline Vec2(double x,double y){loc[0] = x;loc[1] = y;}
	inline Vec2(const double * vec); // create a vector from the array of two doubles pointed to by "vec"
	inline Vec2(const Vec2 & vec);
	inline double getLoc(unsigned int iter){return loc[iter];}
protected:
	double loc[2];
};
Vec2::Vec2(const double *vec)
{
	loc[0] = vec[0];
	loc[1] = vec[1];
}
Vec2::Vec2(const Vec2 & vec)
{
	loc[0] = vec.loc[0];
	loc[1] = vec.loc[1];
}
class Vec3
{
public:
	inline Vec3(){}
	inline Vec3(double x,double y,double z){loc[0] = x;loc[1] = y;loc[2] = z;}
	inline Vec3(const double * vec); // create a vector from the array of three doubles pointed to by "vec"
	inline Vec3(const Vec3 & vec);

	inline Vec3 & operator=(const Vec3 & source);
	inline bool operator==(const Vec3 & source) const;
	inline Vec3 & operator+=(const Vec3 & source);
	inline const Vec3 operator+(const Vec3 & source) const;
	inline Vec3 & operator-=(const Vec3 & source);
	inline const Vec3 operator-(const Vec3 & source) const;
	inline Vec3 & operator *= (double scalar );
	inline const Vec3 operator * (double scalar) const;
	inline Vec3 & operator /= (double divisor);
	inline const Vec3  operator/ (double divisor) const;
	inline Vec3 normalize(const Vec3 & source);
	inline double getLoc(unsigned int iter){return loc[iter];}
	friend inline double dot(Vec3 &v1,Vec3 &v2);
	friend inline Vec3 cross( Vec3 &v1, Vec3 &v2);
	friend inline Vec3 normalize(const Vec3 & source);

protected:
	double loc[3];
};
inline Vec3::Vec3(const double * vec)
{
  loc[0] = vec[0];
  loc[1] = vec[1];
  loc[2] = vec[2];
}

inline Vec3::Vec3(const Vec3 & vec)
{
  loc[0] = vec.loc[0];
  loc[1] = vec.loc[1];
  loc[2] = vec.loc[2];
}
inline Vec3 & Vec3::operator=(const Vec3 & source)
{
	loc[0] = source.loc[0];
	loc[1] = source.loc[1];
	loc[2] = source.loc[2];
	return *this;
}

inline bool Vec3::operator==(const Vec3 & source)const
{
	return ((loc[0] == source.loc[0]) &&
		(loc[1] == source.loc[1]) &&
		(loc[2] == source.loc[2]));
}
inline Vec3 & Vec3::operator+=(const Vec3 & source)
{
	loc[0] += source.loc[0];
	loc[1] += source.loc[1];
	loc[2] += source.loc[2];
	return *this;
}
inline const Vec3 Vec3::operator+(const Vec3 & source) const
{
	Vec3 sum = *this;
	sum.loc[0] += source.loc[0];
	sum.loc[1] += source.loc[1];
	sum.loc[2] += source.loc[2];
	return sum;
}
inline Vec3 & Vec3::operator-=(const Vec3 & source)
{
	loc[0] -= source.loc[0];
	loc[1] -= source.loc[0];
	loc[2] -= source.loc[2];
	return *this;
}
inline const Vec3 Vec3::operator-(const Vec3 & source) const
{
	Vec3 result = *this;
	result.loc[0] -=source.loc[0];
	result.loc[1] -= source.loc[1];
	result.loc[2] -= source.loc[2];
	return result;
}
inline Vec3 & Vec3::operator *= (double scalar )
{
	loc[0] *= scalar;
	loc[1] *= scalar;
	loc[2] *= scalar;
	return *this;
}
inline const Vec3 Vec3::operator*(double scalar)const
{
	return (Vec3(loc[0]*scalar,loc[1]*scalar,loc[2]*scalar));
}
inline Vec3 & Vec3::operator /= (double divisor)
{
	loc[0] /= divisor;
	loc[1] /= divisor;
	loc[2] /= divisor;
	return *this;
}
inline const Vec3 Vec3::operator/(double divisor) const
{
	return (Vec3(loc[0]/divisor,loc[1]/divisor,loc[2]/divisor));
}
Vec3 cross( Vec3 &v1, Vec3 &v2)
{
	return Vec3(v1.getLoc(1) * v2.getLoc(2)-v1.getLoc(2) * v2.getLoc(1),
		-(v1.getLoc(0) * v2.getLoc(2) - v1.getLoc(2) * v2.getLoc(0)),
		v1.getLoc(0) * v2.getLoc(1)-v1.getLoc(1) * v2.getLoc(0));
}
double dot( Vec3 &v1, Vec3 &v2)
{
	return (v1.getLoc(0) * v2.getLoc(0)+ v1.getLoc(1) * v2.getLoc(1) + v1.getLoc(2) * v2.getLoc(2));
}
Vec3 normalize(const Vec3 & source)
{
	Vec3 nor = source;
	double s = dot(nor,nor);
	return nor/sqrt(s);
}
#endif