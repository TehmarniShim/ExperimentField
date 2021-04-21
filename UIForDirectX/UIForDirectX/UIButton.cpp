#include "pch.h"
//#include <dwrite.h>
#include "UIButton.h"

UIButton::UIButton(const std::wstring& text, const float fXpos, const float fYpos, const float fWidth, const float fHeight, size_t nTxtSize)
	: m_fXPos(fXpos)
	, m_fYPos(fYpos)
	, m_fWidth(fWidth)
	, m_fHeight(fHeight)
	, m_wcText(text)
	, m_pRenderTarget(nullptr)
	, m_nTxtSize(nTxtSize)
{

}

void UIButton::Draw() const
{
	//m_pRenderTarget->DrawRectangle(m_nXPos, m_nYPos, 20.0f, 20.0f);
	//m_pRenderTarget->DrawRectangle(m_nXPos, m_nYPos, 20.0f, 20.0f, 140, 140, 140, 255,);
	/*RenderTarget->DrawRectangle(&rectangle, brushForBox, stroke);
	float red = ConvertUI8ToFloat(r);
	float green = ConvertUI8ToFloat(g);
	float blue = ConvertUI8ToFloat(b);
	float alpha = ConvertUI8ToFloat(a);
	ID2D1SolidColorBrush* brushForBox;
	RenderTarget->CreateSolidColorBrush(D2D1::ColorF(red, green, blue, alpha), &brushForBox);

	D2D1_RECT_F rectangle = D2D1::RectF(x, y, x + width, y + height);

	if (!filled)
	{
		RenderTarget->DrawRectangle(&rectangle, brushForBox, stroke);
	}
	else
	{
		RenderTarget->FillRectangle(&rectangle, brushForBox);
	}*/
	constexpr float fRedValue = 0.0f;
	constexpr float fGreenValue = 0.0f;
	constexpr float fBlueValue = 0.0f;
	constexpr float fAlphaValue = 1.0f;

	//constexpr float fBoxWidth = 20.0f;
	//constexpr float fBoxHeight = 20.0f;
	ID2D1SolidColorBrush* brushForBox;
	
	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(fRedValue, fGreenValue, fBlueValue, fAlphaValue), &brushForBox);
	D2D1_RECT_F rectangle = D2D1::RectF(m_fXPos, m_fYPos, m_fXPos + m_fWidth, m_fYPos + m_fHeight);
	if (nullptr != brushForBox)
	{
		m_pRenderTarget->DrawRectangle(&rectangle, brushForBox);
	}

	constexpr float fTextColorRed = 0.0f;
	constexpr float fTextColorGreen = 0.0f;
	constexpr float fTextColorBlue = 0.0f;
	ID2D1SolidColorBrush* brushForText;

	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(fTextColorRed, fTextColorGreen, fTextColorBlue), &brushForText);


	//float red = ConvertUI8ToFloat(r);
	//float green = ConvertUI8ToFloat(g);
	//float blue = ConvertUI8ToFloat(b);
	//float alpha = ConvertUI8ToFloat(a);
	//ID2D1SolidColorBrush* brushForBox;
	//RenderTarget->CreateSolidColorBrush(D2D1::ColorF(red, green, blue, alpha), &brushForBox);

	IDWriteFactory* WriteFactory;
	HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&WriteFactory);

	IDWriteTextFormat* format;
	//constexpr size_t nTxtSize = 20;
	hr = WriteFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, static_cast<float>(m_nTxtSize), L"", &format);

	//// Center the text horizontally and vertically.
	format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	if (format != nullptr && brushForText != nullptr)
	{
		m_pRenderTarget->DrawTextW(m_wcText.c_str(), m_wcText.size(), format, 
			D2D1::RectF(m_fXPos, m_fYPos, m_fXPos + m_fWidth, m_fYPos + m_fHeight), brushForText);
	}


}

void UIButton::SetWidth(const float fValue)
{
	m_fWidth = fValue;
}

void UIButton::SetHeight(const float fValue)
{
	m_fHeight = fValue;
}

float UIButton::GetHeight()
{
	return m_fHeight;
}

float UIButton::GetWidth()
{
	return m_fWidth;
}

void UIButton::TakeRenderTarget(ID2D1HwndRenderTarget* pRenderTarget)
{
	m_pRenderTarget = pRenderTarget;
}



//if (!this->RoundedCorners)
//{
//	this->srcWindow->GetGraphics()->DrawRectangle(xPos, yPos, Width, Height, color->r, color->g, color->b, color->a, Stroke, Filled); // button itself
//
//	this->srcWindow->GetGraphics()->DrawRectangle(xPos - 0.2f, yPos - 0.2f, Width + 0.2f, Height + 0.2f,
//		borderColor->r, borderColor->g, borderColor->b, borderColor->a, borderStroke, false); // border rectangle to indicate that its a button
//}
//else
//{
//	this->srcWindow->GetGraphics()->DrawRoundedRectangle(xPos, yPos, Width, Height, roundCornerRadiusX, roundCornerRadiusY,
//		color->r, color->g, color->b, color->a, Stroke, Filled); // button itself
//
//	this->srcWindow->GetGraphics()->DrawRoundedRectangle(xPos - 0.2f, yPos - 0.2f, Width + 0.2f, Height + 0.2f, roundCornerRadiusX, roundCornerRadiusY,
//		borderColor->r, borderColor->g, borderColor->b, borderColor->a, borderStroke, false); // border rectangle to indicate that its a button
//}
//
//this->srcWindow->GetGraphics()->drawText(std::wstring(Text), FontName, FontSize,
//	xPos + Margins, yPos + Margins, Width - Margins, Height - Margins, textColor->r, textColor->g, textColor->b, textColor->a); // text inside a button