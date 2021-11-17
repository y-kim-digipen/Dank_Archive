/*
  basic_math.h

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim added lerp and tounitVec functions and some operator overloadings
  DoYoon Kim added get_Intersect_Point_From_2_Lines(), line class, projected_Point_On_Line().
  SeungGeon Kim rearranged and wrapped up all the operator overloadings

  All content © 2019 DigiPen (USA) Corporation, all vectors reserved.
*/

#pragma once

#include <vector>
using std::vector;



struct Vector {
	Vector(float x, float y) :x(x), y(y) {}
	Vector() {}
	float x{ 0.0f };
	float y{ 0.0f };

	Vector operator+(const Vector& vector)const;
	Vector operator-(const Vector& vector)const;
	Vector operator*(const float& vector)const;
	Vector operator/(const float& vector)const;

	Vector& operator+=(const Vector& vector);
	Vector& operator-=(const Vector& vector);
	Vector& operator*=(const float& floatParam);
	Vector& operator= (const Vector& vector);

	float cross(const Vector& vector)const;

	float operator*(const Vector& vector);
	float operator*(const Vector& vector)const;

	void toUnitVec();
	Vector getUnitVec();
	float length()const;
	float sqrLength()const;
};


void rotateVector(Vector& vector, const float radianDegrees);

float returnVectorLength(const Vector& vector);

template<typename T>
inline T lerp(T Start, T end, float point)
{
	if (point > 1.0f || point < 0.0f)
	{
		throw("point have to be int [0, 1]");
	}
	return T(Start + (end - Start) * point);
}

bool get_Intersect_Point_From_2_Lines(const Vector& p1, const Vector& p2, const Vector& p3, const Vector& p4, Vector& intersectingPoint);

class Line
{
public:
	Line(const Vector& p1, const Vector& p2);
	vector<Vector> getPoints() const;
	Vector getDirectionVector() const;
	float getLength()const;
	float getLengthSqr()const;
private:
	Vector p1;
	Vector p2;
};

bool get_Intersect_Point_From_2_Lines(const Line& line_1,const Line& line_2, Vector& intersectingPoint);

Vector projected_Point_On_Line(const Vector& p, const Line& line);