// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#if IS_PLATFORM_MACOS || IS_PLATFORM_IOS

// Metal4Renderer.cpp owns the *_PRIVATE_IMPLEMENTATION defines; a second TU only includes.
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

#include "GpuCapture.h"

import jpt.Logger;
import jpt.PlatformPaths;
import std;

namespace jpt
{
    namespace local
    {
        [[nodiscard]] std::string Timestamp()
        {
            const std::time_t now = std::time(nullptr);
            char text[32] = {};
            std::strftime(text, sizeof(text), "%Y%m%d-%H%M%S", std::localtime(&now));
            return text;
        }
    }

    void GpuCapture::RequestCapture()
    {
        if (!m_path.empty())
        {
            return;
        }

        if (!MTL::CaptureManager::sharedCaptureManager()->supportsDestination(MTL::CaptureDestinationGPUTraceDocument))
        {
            Debug::Error("GPU capture unavailable: the bundle has no MTLCaptureEnabled.");
            return;
        }

        m_path = GetSavedDir() / "Traces" / (local::Timestamp() + ".gputrace");

        std::error_code error;
        std::filesystem::create_directories(m_path.parent_path(), error);
        std::filesystem::remove_all(m_path, error);   // startCapture will not overwrite one.

        m_framesUntil = 1;
    }

    void GpuCapture::DeleteAll()
    {
        if (!m_path.empty())
        {
            return;   // A capture is scheduled to write there this frame.
        }

        std::error_code error;
        uint32 deleted = 0;
        for (const std::filesystem::directory_entry& entry :
             std::filesystem::directory_iterator(GetSavedDir() / "Traces", error))
        {
            const bool isCapture = entry.path().extension() == ".gputrace";   // .DS_Store goes uncounted
            deleted += (std::filesystem::remove_all(entry.path(), error) > 0) && isCapture;
        }

        Debug::Info("Deleted {} capture(s).", deleted);
    }

    void GpuCapture::BeginFrame(MTL::Device* pDevice)
    {
        if (m_path.empty() || --m_framesUntil != 0)
        {
            return;
        }

        // The device, not a scope from the Metal 4 queue: the capture layer sends -traceStream to
        // whatever it is handed and such a scope does not answer it. Hence start/stop, not scopes.
        MTL::CaptureDescriptor* pDesc = MTL::CaptureDescriptor::alloc()->init()->autorelease();
        pDesc->setCaptureObject(pDevice);
        pDesc->setDestination(MTL::CaptureDestinationGPUTraceDocument);
        pDesc->setOutputURL(NS::URL::fileURLWithPath(NS::String::string(m_path.c_str(), NS::UTF8StringEncoding)));

        NS::Error* pError = nullptr;
        if (!MTL::CaptureManager::sharedCaptureManager()->startCapture(pDesc, &pError))
        {
            Debug::Error("GPU capture failed: {}", pError->localizedDescription()->utf8String());
            m_path.clear();
        }
    }

    void GpuCapture::EndFrame()
    {
        if (m_path.empty() || m_framesUntil != 0)
        {
            return;
        }

        MTL::CaptureManager::sharedCaptureManager()->stopCapture();

        Debug::Info("GPU capture written to {}", m_path.string());   // On iOS, Scripts/Debug/pull_captures.py fetches it
        m_path.clear();
    }
}

#endif
