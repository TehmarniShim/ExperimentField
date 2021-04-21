#pragma once
#include "pch.h"

#pragma once
//#include <vector>
#include <EASTL/vector.h>
#include <Windows.h>

#include <d2d1.h>
#include "TehmarniEAStlHeader.h"
#include "UIComponent.h"

class UIComponent;

class UIManager
{
public:
	UIManager(HWND hWnd, ID2D1HwndRenderTarget* pRenderTarget);
	~UIManager();

	void DrawUIComponents() const;
	void AddUI(UIComponent* pComponent);
	void Release();


private:
	HWND m_hWnd;
	eastl::vector<UIComponent*> m_VUIComponents;
	ID2D1HwndRenderTarget* m_pRenderTarget;

};