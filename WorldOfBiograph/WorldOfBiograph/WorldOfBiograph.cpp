#pragma once
#include "WorldOfBiograph.h"
#pragma comment(lib, "d2d1")
WorldOfBiograph::WorldOfBiograph()
    : m_hwnd(NULL)
    , m_pDirect2dFactory(NULL)
    , m_pCornflowerBlueBrush(NULL)
    , m_pLightSlateGrayBrush(NULL)
    , m_pRenderTarget(NULL)
    , mpBioGraph()
{
    for (auto& pBio : mpBioGraph)
    {
        pBio = nullptr;
    }
    
}

WorldOfBiograph::~WorldOfBiograph()
{
    SafeRelease(&m_pCornflowerBlueBrush);
    SafeRelease(&m_pDirect2dFactory);
    SafeRelease(&m_pLightSlateGrayBrush);
    SafeRelease(&m_pRenderTarget);
    for (auto& i : mpBioGraph)
    {
        if (nullptr != i)
        {
        delete i;
        }
    }
    
}

void WorldOfBiograph::RunMessageLoop()
{
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

HRESULT WorldOfBiograph::Initialize()
{
    HRESULT hr;

    // Demo device-indpendent resources, such
    // as the Direct2D factory.
    hr = createDeviceIndependentResources();

    if (SUCCEEDED(hr))
    {
        // Register the window class.
        WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WorldOfBiograph::WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = sizeof(LONG_PTR);
        wcex.hInstance = HINST_THISCOMPONENT;
        wcex.hbrBackground = NULL;
        wcex.lpszMenuName = NULL;
        wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
        wcex.lpszClassName = L"WorldOFBiograph";

        RegisterClassEx(&wcex);


        // Because the CreateWindow function takes its size in pixels,
        // obtain the system DPI and use it to scale the window size.
        //FLOAT dpiX, dpiY;
        //m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);
        HDC screen = GetDC(0);
        int dpiX = GetDeviceCaps(screen, LOGPIXELSX);
        int dpiY = GetDeviceCaps(screen, LOGPIXELSY);
        // The factory returns the current system DPI. This is also the value it will use
        // to create its own windows.

        // Create the window.
        m_hwnd = CreateWindow(
            L"WorldOFBiograph",
            L"WOB",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            static_cast<UINT>(ceil(640.f * dpiX / 96.f)),
            static_cast<UINT>(ceil(480.f * dpiY / 96.f)),
            NULL,
            NULL,
            HINST_THISCOMPONENT,
            this
        );
        hr = m_hwnd ? S_OK : E_FAIL;
        if (SUCCEEDED(hr))
        {
            ShowWindow(m_hwnd, SW_SHOWNORMAL);
            UpdateWindow(m_hwnd);
        }
    }

    return hr;
}

HRESULT WorldOfBiograph::createDeviceIndependentResources()
{
    HRESULT hr = S_OK;

    // Create a Direct2D factory.
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

    return hr;
}

//HRESULT Demo::createDeviceDependentResources()
//{
//    RECT rc;
//    GetClientRect(m_hwnd, &rc);
//
//    D2D1_SIZE_U size = D2D1::SizeU(
//        rc.right - rc.left,
//        rc.bottom - rc.top
//    );
//
//    // Create a Direct2D render target.
//    hr = m_pDirect2dFactory->CreateHwndRenderTarget(
//        D2D1::RenderTargetProperties(),
//        D2D1::HwndRenderTargetProperties(m_hwnd, size),
//        &m_pRenderTarget
//    );
//}
HRESULT WorldOfBiograph::createDeviceResources()
{
    HRESULT hr = S_OK;

    if (!m_pRenderTarget)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(
            rc.right - rc.left,
            rc.bottom - rc.top
        );

        // Create a Direct2D render target.
        hr = m_pDirect2dFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &m_pRenderTarget
        );


        if (SUCCEEDED(hr))
        {
            // Create a gray brush.
            hr = m_pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::LightSlateGray),
                &m_pLightSlateGrayBrush
            );
        }
        if (SUCCEEDED(hr))
        {
            // Create a blue brush.
            hr = m_pRenderTarget->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
                &m_pCornflowerBlueBrush
            );
        }
        mpBioGraph[0] = new BioGraph(static_cast<float>(size.width / 2.0f), static_cast<float>(size.height / 2.0f + 150.0f), 80.0f);
        mpBioGraph[0]->MakeGraph();
    }

    return hr;
}

void WorldOfBiograph::discardDeviceResources()
{
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&m_pLightSlateGrayBrush);
    SafeRelease(&m_pCornflowerBlueBrush);
}

HRESULT WorldOfBiograph::OnRender()
{
    using namespace DirectX;
    HRESULT hr = S_OK;
    hr = createDeviceResources();
    if (SUCCEEDED(hr))
    {
        m_pRenderTarget->BeginDraw();

        m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

        m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

        D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

        // Draw a grid background.
        const int width = static_cast<int>(rtSize.width-80.0f);
        const int height = static_cast<int>(rtSize.height);
        //const float fStrokSize = 10.0f;


        for (size_t i = 0; i < GENES_COUNT; i++)
        {
            assert(nullptr != mpBioGraph[i]);
            VecBio VBioLines = mpBioGraph[i]->GetBioGraph();
            size_t nGraphSize = VBioLines.size();
            for (auto& bioLine : VBioLines)
            {
                Vector2D& VStartPoint = bioLine->V2Position;
                Vector2D VAddingDirection = bioLine->V2Direction;
                VAddingDirection = VAddingDirection * bioLine->fLength;
                Vector2D VEndPoint = VStartPoint + VAddingDirection;
               
                const float fX1 = VStartPoint.fX;
                const float fY1 = VStartPoint.fY;
                const float fX2 = VEndPoint.fX;
                const float fY2 = VEndPoint.fY;

                
                m_pRenderTarget->DrawLine(
                    D2D1::Point2F(fX1, fY1),
                    D2D1::Point2F(fX2, fY2),
                    m_pLightSlateGrayBrush,
                    3.5f
                );

            }
        }
        hr = m_pRenderTarget->EndDraw();

        

        //for (int x = 0; x < width; x += 3)
        //{
        //    m_pRenderTarget->DrawLine(
        //        D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
        //        D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
        //        m_pLightSlateGrayBrush,
        //        0.5f
        //    );

        //}

        //for (int y = 0; y < height; y += 3)
        //{
        //    m_pRenderTarget->DrawLine(
        //        D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
        //        D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
        //        m_pLightSlateGrayBrush,
        //        fStrokSize
        //    );
        //}

        //// Draw two rectangles.
        //D2D1_RECT_F rectangle1 = D2D1::RectF(
        //    rtSize.width / 2 - 50.0f,
        //    rtSize.height / 2 - 50.0f,
        //    rtSize.width / 2 + 50.0f,
        //    rtSize.height / 2 + 50.0f
        //);

        //D2D1_RECT_F rectangle2 = D2D1::RectF(
        //    rtSize.width / 2 - 100.0f,
        //    rtSize.height / 2 - 100.0f,
        //    rtSize.width / 2 + 100.0f,
        //    rtSize.height / 2 + 100.0f
        //);

        // Draw a filled rectangle.
        //m_pRenderTarget->FillRectangle(&rectangle1, m_pLightSlateGrayBrush);

        // Draw the outline of a rectangle.
        //m_pRenderTarget->DrawRectangle(&rectangle2, m_pCornflowerBlueBrush);

        //hr = m_pRenderTarget->EndDraw();

    }

    if (hr == D2DERR_RECREATE_TARGET)
    {
        hr = S_OK;
        discardDeviceResources();
    }
    return hr;
}
LRESULT CALLBACK WorldOfBiograph::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    

    if (message == WM_CREATE)
    {
        LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
        WorldOfBiograph* pDemoApp = (WorldOfBiograph*)pcs->lpCreateParams;

        ::SetWindowLongPtrW(
            hwnd,
            GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(pDemoApp)
        );

        result = 1;
    }
    else
    {
        WorldOfBiograph* pDemoApp = reinterpret_cast<WorldOfBiograph*>(static_cast<LONG_PTR>(
            ::GetWindowLongPtrW(
                hwnd,
                GWLP_USERDATA
            )));

        bool wasHandled = false;

        if (pDemoApp)
        {
            switch (message)
            {
            case WM_SIZE:
            {
                UINT width = LOWORD(lParam);
                UINT height = HIWORD(lParam);
                pDemoApp->OnResize(width, height);
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_DISPLAYCHANGE:
            {
                InvalidateRect(hwnd, NULL, FALSE);
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_PAINT:
            {
                pDemoApp->OnRender();
                ValidateRect(hwnd, NULL);
            }
            result = 0;
            wasHandled = true;
            break;

            case WM_DESTROY:
            {
                PostQuitMessage(0);
            }
            result = 1;
            wasHandled = true;
            break;

            case WM_KEYDOWN:
            {
                eGenes eMutation;
                switch (wParam)
                {
                case 0x41: //A
                    eMutation = eGenes::ADD_ANGLE_EVEN_BRANCH;
                    break;
                    
                case 0x53: //S
                    eMutation = eGenes::SUBTRACT_ANGLE_EVEN_BRANCH;
                    break;

                case 0x44:  //D
                    eMutation = eGenes::ADD_ANGLE_ODD_BRANCH;
                    break;

                case 0x46: //F
                    eMutation = eGenes::SUBTRACT_ANGLE_ODD_BRANCH;
                    break;

                case 0x5A: //Z
                    eMutation = eGenes::ADD_LENGTH_EVEN_BIO;
                    break;

                case 0x58: //X
                    eMutation = eGenes::SUBTRACT_LENGTH_EVEN_BIO;
                    break;

                case 0x43: //C
                    eMutation = eGenes::ADD_LENGTH_ODD_BIO;
                    break;

                case 0x56: //V
					eMutation = eGenes::SUBTRACT_LENGTH_ODD_BIO;
					break;

				default:
					eMutation = eGenes::DEFAULT;
					break;
				}
				pDemoApp->mpBioGraph[0]->MakeChange(eMutation);
				pDemoApp->OnRender();
				wasHandled = true;
				result = 0;
			}

            }
        }

        if (!wasHandled)
        {
            result = DefWindowProc(hwnd, message, wParam, lParam);
        }
    }

    return result;
}
void WorldOfBiograph::OnResize(UINT width, UINT height)
{
    if (m_pRenderTarget)
    {
        // Note: This method can fail, but it's okay to ignore the
        // error here, because the error will be returned again
        // the next time EndDraw is called.
        m_pRenderTarget->Resize(D2D1::SizeU(width, height));
        mpBioGraph[0]->RelocateGraph(width/2 , height / 2 + 150);
    }

}