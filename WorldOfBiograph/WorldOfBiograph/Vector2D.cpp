#include "Vector2D.h"






void Vector2D::Normalize()
{
	float fLength = std::sqrtf(fX * fX + fY * fY);
	fX /= fLength;
	fY /= fLength;
}

Vector2D Vector2D::operator+(const Vector2D& rhs)
{
	Vector2D result{};
	
	result.fX = this->fX + rhs.fX;
	result.fY = this->fY + rhs.fY;
	return result;
}

Vector2D Vector2D::operator*(const float fFactor)
{
	Vector2D result{};
	
	result.fX = fX * fFactor;
	result.fY = fY * fFactor;
	return result;
}

Vector2D Vector2D::RotateVector(const float degree)
{

	//Let's change the conditions that degree here is just between Zero to Pi
	using namespace DirectX;
Vector2D result{};
	Vector2D VMathDirection = Vector2D{ this->fX, -this->fY };
	Quodrant QuodrantInfo = VMathDirection.GetQuodrantInfo();
	XMVECTOR XMVMathDirection{ VMathDirection.fX, VMathDirection.fY };
	XMVECTOR XMVXAxis{ 1.0f, 0.0f };
	XMVECTOR XMVAngleBeforeRotationInRadian{};
	XMFLOAT2 F2AngleBeforeRotationInRadian{};
	float fAngleBeforeRotationInRadianToBeChanged = 0.0f;
	float fAngleBeforeRotationInDegreeToBeChanged = 0.0f;
	float fAngleBeforeRotationInDegree = 0.0f;
	switch (QuodrantInfo)
	{
	case Quodrant::ONE:
		
	case Quodrant::TWO:
		XMVAngleBeforeRotationInRadian = XMVector2AngleBetweenNormals(XMVMathDirection, XMVXAxis);
		XMStoreFloat2(&F2AngleBeforeRotationInRadian, XMVAngleBeforeRotationInRadian);
		fAngleBeforeRotationInDegree = XMConvertToDegrees(F2AngleBeforeRotationInRadian.x);
		break;
	case Quodrant::THREE:
		
		
	case Quodrant::FOUR:
		XMVECTOR XMAngleBeforeRotationInRadianToBeChanged = XMVector2AngleBetweenNormals(XMVMathDirection, XMVXAxis);
		XMFLOAT2 F2AngleBeforeRotationInRadianToBeChanged{};
		XMStoreFloat2(&F2AngleBeforeRotationInRadianToBeChanged, XMAngleBeforeRotationInRadianToBeChanged);
		fAngleBeforeRotationInRadianToBeChanged = F2AngleBeforeRotationInRadianToBeChanged.x;
		fAngleBeforeRotationInDegreeToBeChanged = XMConvertToDegrees(fAngleBeforeRotationInRadianToBeChanged);
		fAngleBeforeRotationInDegree = 360.0f - fAngleBeforeRotationInDegreeToBeChanged;
		break;
	//default:
		assert(false);
		break;
	}
	
	float fAngleAfterRotationInDegree = fAngleBeforeRotationInDegree + degree;
	float fAngleAfterRotationInRadian = XMConvertToRadians(fAngleAfterRotationInDegree);
	result.fX = std::cosf(fAngleAfterRotationInRadian);
	result.fY = -std::sinf(fAngleAfterRotationInRadian);

	return result;
}

Quodrant Vector2D::GetQuodrantInfo() const
{
	if (fX >= 0 && fY>=0)
	{
		return Quodrant::ONE;
	}

	else if (fX < 0 && fY >= 0)
	{
		return Quodrant::TWO;
	}

	else if (fX < 0 && fY < 0)
	{
		return Quodrant::THREE;
	}
	else  // fX > 0 && fY < 0
	{
		return Quodrant::FOUR;
	}
}

Quodrant GetQuodrantInfoNonVector(float fAngle)

{
	while (fAngle >= 360.0f)
	{
		fAngle -= 360.0f;
	}
	if (fAngle < 90.0f)
	{
		return Quodrant::ONE;
	}
	else if (fAngle < 180.0f)
	{
		return Quodrant::TWO;
	}
	else if (fAngle < 270.0f)
	{
		return Quodrant::THREE;
	}
	else // fAngle >=270.0f && fAngle < 360.0f
	{
		return Quodrant::FOUR;
	}

}