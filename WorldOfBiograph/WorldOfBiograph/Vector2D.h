#pragma once
#include <DirectXMath.h>
#include <cmath>

enum class Quodrant
{
	ONE,
	TWO,
	THREE,
	FOUR
};

Quodrant GetQuodrantInfoNonVector(float fAngle);

class Vector2D
{

public:
	void Normalize();
	Vector2D operator+(const Vector2D& rhs) const;
	Vector2D operator*(const float fFactor) const;
	Vector2D RotateVector(const float fDegree) const;
	Quodrant GetQuodrantInfo() const;


	float fX;
	float fY;
};





