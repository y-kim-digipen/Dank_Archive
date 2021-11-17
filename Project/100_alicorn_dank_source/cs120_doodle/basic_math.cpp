/*
  basic_math.cpp

  GAM100 Prototype II
  Fall 2019

  YoonKi Kim added lerp and tounitVec functions and some operator overloadings
  DoYoon Kim implemented get_Intersect_Point_From_2_Lines(), line class, projected_Point_On_Line().
  SeungGeon Kim arranged the initial player class

  All content © 2019 DigiPen (USA) Corporation, all vectors reserved.
*/

#include "basic_math.h"
#include <math.h>



void rotateVector(Vector& vector, const float radian)
{
	vector.x = cosf(radian) * vector.x - sinf(radian) * vector.y;
	vector.y = sinf(radian) * vector.x + cosf(radian) * vector.y;
	vector.toUnitVec();
}

float returnVectorLength(const Vector& vector) {
	return sqrtf(vector.x * vector.x + vector.y * vector.y);
}

bool get_Intersect_Point_From_2_Lines(const Vector& p1, const Vector& p2, const Vector& p3, const Vector& p4,Vector& intersectingPoint)
{
	Vector line1Dir(p2 - p1);
	Vector line2Dir(p4 - p3);
	Vector perp_p1p2 = Vector(-line1Dir.y, line1Dir.x);
	Vector perp_p3p4 = Vector(-line2Dir.y, line2Dir.x);
	float t = (p1 - p3) * perp_p1p2 / (line2Dir * perp_p1p2);
	float s = (p3 - p1) * perp_p3p4 / (line1Dir * perp_p3p4);

	if (t > 0.f && t < 1.f && s > 0.f && s < 1.f)
	{
		intersectingPoint = p1 + line1Dir * s;
		return true;
	}
	
	return false;
}

bool get_Intersect_Point_From_2_Lines(const Line& line_1, const Line& line_2, Vector& intersectingPoint)
{
	const Vector& line1 = line_1.getDirectionVector();
	const Vector& line2 = line_2.getDirectionVector();
	const Vector& p1 = line_1.getPoints()[0];
	const Vector& p3 = line_2.getPoints()[0];

	float parallelDet = line1.cross(line2);

	if (abs(parallelDet) < FLT_EPSILON)
		return false;

	float t = (p3 - p1).cross(line2) / parallelDet;

	float line1_length = line1.length();
	if (t > line1_length)
		return false;

	intersectingPoint = p1 + line1 * t;
	return true;
}

Vector projected_Point_On_Line(const Vector& p, const Line& line)
{
	Vector line_p1 = line.getPoints()[0];
	Vector lineDir = line.getDirectionVector();
	return line_p1 + lineDir*(((p - line_p1) * (lineDir)) / line.getLengthSqr());
}

Line::Line(const Vector& p1, const Vector& p2)
	:p1(p1),p2(p2)
{
}

vector<Vector> Line::getPoints() const
{
	vector<Vector> points{ p1,p2 };
	return points;
}

Vector Line::getDirectionVector() const
{
	Vector dirVector(p2.x - p1.x, p2.y - p1.y);
	return dirVector;
}

float Line::getLength() const
{
	Vector segment = p2 - p1;
	return sqrtf(segment.x* segment.x+ segment.y* segment.y);
}

float Line::getLengthSqr() const
{
	Vector segment = p2 - p1;
	return segment.x * segment.x + segment.y * segment.y;
}

void Vector::toUnitVec() {
	float newX = x/ returnVectorLength({ this->x , this->y });
	float newY = y/ returnVectorLength({ this->x , this->y });
	this->x = newX;
	this->y = newY;
}

Vector Vector::getUnitVec()
{
	float length = this->length();
	return Vector(x/length,y/length);
}

float Vector::length() const
{
	return sqrtf(x * x + y * y);
}

float Vector::sqrLength() const
{
	return x*x+y*y;
}

Vector Vector::operator+(const Vector& vector)const
{
	Vector tempVector{ this->x, this->y };
	tempVector.x += vector.x;
	tempVector.y += vector.y;
	return tempVector;
}

Vector Vector::operator-(const Vector& vector)const
{
	Vector tempVector{ this->x, this->y };
	tempVector.x -= vector.x;
	tempVector.y -= vector.y;
	return tempVector;
}

Vector Vector::operator*(const float& vector)const
{
	Vector tempVector{ this->x, this->y };
	tempVector.x *= vector;
	tempVector.y *= vector;
	return tempVector;
}

Vector Vector::operator/(const float& vector)const
{
	Vector tempVector{ this->x, this->y };
	tempVector.x /= vector;
	tempVector.y /= vector;
	return tempVector;
}



Vector& Vector::operator+=(const Vector& vector)
{
	this->x += vector.x;
	this->y += vector.y;
	return *this;
}

Vector& Vector::operator-=(const Vector& vector)
{
	this->x -= vector.x;
	this->y -= vector.y;
	return *this;
}

Vector& Vector::operator*=(const float& floatParam)
{
	this->x *= floatParam;
	this->y *= floatParam;
	return *this;
}

Vector& Vector::operator=(const Vector& vector)
{
	x = vector.x;
	y = vector.y;

	return *this;
}

float Vector::cross(const Vector& vector)const
{
	return (this->x * vector.y) - (this->y * vector.x);
}


float Vector::operator*(const Vector& vector)
{
	float tempFloat = 0;
	tempFloat += this->x * vector.x;
	tempFloat += this->y * vector.y;
	return tempFloat;
}

float Vector::operator*(const Vector& vector) const
{
	float tempFloat = 0;
	tempFloat += this->x * vector.x;
	tempFloat += this->y * vector.y;
	return tempFloat;
}
