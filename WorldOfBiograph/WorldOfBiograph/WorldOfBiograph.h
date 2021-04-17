#pragma once
#include "framework.h"

class BioGraph;
#define GENES_COUNT 1


class WorldOfBiograph
{
public:
	WorldOfBiograph();
	~WorldOfBiograph();


	HRESULT Initialize();

	void RunMessageLoop();

private:
	HRESULT createDeviceIndependentResources();
	HRESULT createDeviceResources();
	void discardDeviceResources();
	HRESULT OnRender();

	void OnResize(UINT width, UINT height);
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);


private:
	HWND m_hwnd;
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
	ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
	BioGraph* mpBioGraph[GENES_COUNT];

};