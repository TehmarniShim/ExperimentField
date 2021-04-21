#pragma once
#include <d2d1.h>
//#include <d2d1.h>
//#include "UI"

class UIComponent
{
public:
	UIComponent() {}
	virtual void Draw() const = 0;
	virtual void SetWidth(const float fValue) = 0;
	virtual void SetHeight(const float fValue) = 0;
	virtual float GetHeight() = 0;
	virtual float GetWidth() = 0;
	virtual void TakeRenderTarget(ID2D1HwndRenderTarget* pRenderTarget) = 0;

	//typedef void(*uiElement_global_callback_function)(UIComponent*);




};