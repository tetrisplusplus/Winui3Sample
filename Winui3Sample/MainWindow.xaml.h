// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "MainWindow.g.h"

#include <Common/interface/RefCntAutoPtr.hpp>
#include <Graphics/GraphicsEngineD3D12/interface/EngineFactoryD3D12.h>
#include <Graphics/GraphicsEngine/interface/SwapChain.h>

namespace winrt::Winui3Sample::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void SwapChainPanel_Loading(winrt::Microsoft::UI::Xaml::FrameworkElement const& sender, winrt::Windows::Foundation::IInspectable const& args);

        void SwapChainPanel_SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::SizeChangedEventArgs const& e);

        void SwapChainPanel_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);

        void StartRenderLoop();

    private:
        Diligent::RefCntAutoPtr<Diligent::IRenderDevice>  device;
        Diligent::RefCntAutoPtr<Diligent::IDeviceContext> context;
        Diligent::RefCntAutoPtr<Diligent::ISwapChain>     swapchain;
        Diligent::RefCntAutoPtr<Diligent::IPipelineState> pso;

        winrt::Windows::Foundation::IAsyncAction renderLoopWorker;
    };
}

namespace winrt::Winui3Sample::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
