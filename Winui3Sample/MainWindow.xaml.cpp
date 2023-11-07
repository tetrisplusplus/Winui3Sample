// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <winrt/Windows.System.Threading.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Winui3Sample::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();
    }

    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void MainWindow::SwapChainPanel_Loading(winrt::Microsoft::UI::Xaml::FrameworkElement const& sender, winrt::Windows::Foundation::IInspectable const& args)
    {
        Diligent::EngineD3D12CreateInfo createInfo;
        auto* factory = Diligent::GetEngineFactoryD3D12();
        factory->CreateDeviceAndContextsD3D12(createInfo, &device, &context);

        Diligent::SwapChainDesc swapChainDesc{};
        swapChainDesc.Width = 1;
        swapChainDesc.Height = 1;

        Diligent::Win32NativeWindow window{ winrt::get_abi(sender) };
        factory->CreateSwapChainD3D12(device, context, swapChainDesc, Diligent::FullScreenModeDesc{}, window, &swapchain);
    }


    void MainWindow::SwapChainPanel_SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::SizeChangedEventArgs const& e)
    {
        auto size = e.NewSize();
        const UINT width = UINT(size.Width);
        const UINT height = UINT(size.Height);

        this->swapchain->Resize(width, height);
    }

    void MainWindow::SwapChainPanel_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        this->StartRenderLoop();
    }

    void MainWindow::StartRenderLoop()
    {
        auto workItemHandler = winrt::Windows::System::Threading::WorkItemHandler([this](winrt::Windows::Foundation::IAsyncAction const& action)
            {
                while (action.Status() == Windows::Foundation::AsyncStatus::Started)
                {
                    auto* pRtv = this->swapchain->GetCurrentBackBufferRTV();

                    const float ClearColor[]{ 0.2f, 0.9f, 0.2f, 1.0f };
                    this->context->ClearRenderTarget(pRtv, ClearColor, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

                    this->swapchain->Present();
                }
            });

        using namespace Windows::System::Threading;
        this->renderLoopWorker = Windows::System::Threading::ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
    }
}