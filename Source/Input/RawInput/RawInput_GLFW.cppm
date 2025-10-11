// Copyright Jupiter Technologies]= Inc. All Rights Reserved.

export module jpt.RawInput_GLFW;

import jpt.RawInput;
import jpt.HashMap;
//import jpt.StaticHashMap;

export namespace jpt::Input
{
    class RawInput_GLFW : public RawInput
    {
        using Super = RawInput;

    private:
        HashMap<Key, uint32> m_toGLFWKeys;
        HashMap<uint32, Key> m_fromGLFWKeys;

        HashMap<Modifier, uint32> m_toGLFWModifiers;
        HashMap<uint32, Modifier> m_fromGLFWModifiers;

        HashMap<MouseButton, uint32> m_toGLFWMouseButtons;
        HashMap<uint32, MouseButton> m_fromGLFWMouseButtons;

    public:
        virtual bool PreInit() override;

    public:
        virtual uint32 FromKey(Key key) const override;
        virtual Key ToKey(uint32 key) const override;

        virtual Modifier ParseModifiers(uint32 glfwMods) const override;
        virtual KeyState ParseKeyState(uint32 state) const override;

        virtual uint32 FromMouseButton(MouseButton mouseButton) const override;
        virtual MouseButton ToMouseButton(uint32 mouseButton) const override;
    };
}