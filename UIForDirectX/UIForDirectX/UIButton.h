#pragma once
#include <string>
#include <dwrite.h>

#include "UIComponent.h"
#pragma comment(lib, "Dwrite")

class UIComponent;

class UIButton : public UIComponent
{
public:
	UIButton(const std::wstring& text, const float nXpos, const float nYpos, const float fWidth, const float fHeight, size_t nTxtSize);
	void Draw() const;
	//virtual void DrawUIComponents() = 0;
	virtual void SetWidth(const float fValue);
	virtual void SetHeight(const float fValue);
	virtual float GetHeight();
	virtual float GetWidth();
	virtual void TakeRenderTarget(ID2D1HwndRenderTarget* RenderTarget);


private:
	//float m_fXpos;
	//float m_fYpos;
	//int FontSize = 10;
	std::wstring m_wcText;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	float m_fXPos;
	float m_fYPos;
	float m_fWidth;
	float m_fHeight;
	size_t m_nTxtSize;
	//float xPos = 0, yPos = 0, Width = 0, Height = 0;


};
