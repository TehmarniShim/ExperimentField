
#include "WorldOfBiograph.h"

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    // Use HeapSetInformation to specify that the process should
    // terminate if the heap manager detects an error in any heap used
    // by the process.
    // The return value is ignored, because we want to continue running in the
    // unlikely event that HeapSetInformation fails.
    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
    {
        {
            WorldOfBiograph wob{};

            if (SUCCEEDED(wob.Initialize()))
            {
                wob.RunMessageLoop();
            }
        }
        CoUninitialize();
    }

    return 0;
}