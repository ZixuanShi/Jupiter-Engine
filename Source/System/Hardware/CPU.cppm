// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.CPU;

import jpt.TypeDefs;
import jpt.String;

export namespace jpt
{
    class CPU
    {
    private:
        String m_name;
        uint32 m_logicalProcessorsCount = 0;
        uint32 m_coresCount = 0;

    public:
        bool PreInit();

    public:
        const String& GetName() const { return m_name; }
        uint32 GetLogicalProcessorsCount() const { return m_logicalProcessorsCount; }
        uint32 GetCoresCount() const { return m_coresCount; };
    };
}