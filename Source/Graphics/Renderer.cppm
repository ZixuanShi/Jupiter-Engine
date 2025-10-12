// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/Utilities.h"

export module jpt.Renderer;

import jpt.Graphics_Settings;

import jpt.TextureSampler;
import jpt.Camera;

import jpt.Window;
import jpt.TimeTypeDefs;

import jpt.EventManager;
import jpt.Event_Window_Resize;
import jpt.Event_Window_Close;

export namespace jpt
{
    class Renderer
    {
    protected:
        Graphics_Settings m_settings;
        TextureSampler* m_pTextureSampler = nullptr;
        Camera m_camera;
        GraphicsAPI m_api = GraphicsAPI::Unknown;

    public:
        virtual ~Renderer() = default;

        virtual bool PreInit();
        virtual bool Init();

        virtual void Update(TimePrecision deltaSeconds);
        virtual void Render() { }
        virtual void Terminate() {}

        virtual void DrawFrame() { }

        virtual void RegisterWindow([[maybe_unused]] Window* pWindow) {}
        virtual void OnWindowResize([[maybe_unused]] const Event_Window_Resize& eventWindowResize) {}
        virtual void OnWindowClose([[maybe_unused]] const Event_Window_Close& eventWindowClose) {}

    public:
        /** Sets shouldRecreateSwapChain for all window resources */
        virtual void SetShouldRecreateSwapChains() {}

    public:
        Graphics_Settings& GetSettings();
        TextureSampler* GetTextureSampler() const { return m_pTextureSampler; }
        Camera& GetCamera() { return m_camera; }
        GraphicsAPI GetAPI() const { return m_api; }
        
        const Graphics_Settings& GetSettings() const;
        const Camera& GetCamera() const { return m_camera; }
    };
}