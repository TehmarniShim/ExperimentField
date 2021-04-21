#include "pch.h"
#include "UIManager.h"

UIManager::UIManager(HWND hWnd, ID2D1HwndRenderTarget* pRenderTarget)
	: m_hWnd(hWnd)
	, m_pRenderTarget(pRenderTarget)
	, m_VUIComponents()
{

}

UIManager::~UIManager()
{
	for (auto& comp : m_VUIComponents)
	{
		delete comp;
	}
}

void UIManager::DrawUIComponents() const
{
	for (auto& comp : m_VUIComponents)
	{
		comp->Draw();
	}
}

void UIManager::AddUI(UIComponent* pComponent)
{
	pComponent->TakeRenderTarget(m_pRenderTarget);
	m_VUIComponents.push_back(pComponent);
}

void UIManager::Release()
{
	return;
}
