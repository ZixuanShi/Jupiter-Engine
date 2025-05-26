// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include "Core/Minimal/CoreMacros.h"

export module jpt.Renderer;

import jpt.Graphics.Settings;

import jpt.Texture.Sampler;
import jpt.Camera;

import jpt.Window;
import jpt.Time.TypeDefs;

import jpt.Event.Manager;
import jpt.Event.Window.Resize;
import jpt.Event.Window.Close;

export namespace jpt
{
    class Renderer
    {
    protected:
        GraphicsSettings m_settings;
        TextureSampler* m_pTextureSampler = nullptr;
        Camera m_camera;

    public:
        virtual ~Renderer() = default;

        virtual bool PreInit();
        virtual bool Init();

        virtual void Update(TimePrecision deltaSeconds);
        virtual void Render() { }
        virtual void Shutdown() {}

        virtual void DrawFrame() { }

        virtual void RegisterWindow([[maybe_unused]] Window* pWindow) {}
        virtual void OnWindowResize([[maybe_unused]] const Event_Window_Resize& eventWindowResize) {}
        virtual void OnWindowClose([[maybe_unused]] const Event_Window_Close& eventWindowClose) {}

    public:
        /** Sets shouldRecreateSwapChain for all window resources */
        virtual void SetShouldRecreateSwapChains() {}

    public:
        GraphicsSettings& GetSettings();
        const GraphicsSettings& GetSettings() const;

        TextureSampler* GetTextureSampler() const { return m_pTextureSampler; }
        
        Camera& GetCamera() { return m_camera; }
        const Camera& GetCamera() const { return m_camera; }
    };

    GraphicsSettings& GetGraphicsSettings();
}