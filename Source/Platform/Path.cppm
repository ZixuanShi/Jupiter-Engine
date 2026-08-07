// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module jpt.Path;

import std;

export namespace jpt
{
    class Path
    {
    private:
        std::filesystem::path m_absolute;

    public:
        Path(const char* pRelativePath);
        Path(std::string_view relativePath);

        [[nodiscard]] const std::filesystem::path& GetAbsolute() const noexcept { return m_absolute; }
        [[nodiscard]] std::string GetFileName() const;
    };
}
