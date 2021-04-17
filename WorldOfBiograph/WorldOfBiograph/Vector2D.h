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
	Vector2D operator+(const Vector2D& rhs);
	Vector2D operator*(const float fFactor);
	Vector2D RotateVector(const float degree);
	Quodrant GetQuodrantInfo() const;


	float fX;
	float fY;
};





