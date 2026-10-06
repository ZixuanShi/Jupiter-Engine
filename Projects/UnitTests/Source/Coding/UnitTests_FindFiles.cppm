// Copyright Jupiter Technologies, Inc. All Rights Reserved.

export module UnitTests.FindFiles;

import UnitTests.TestFramework;
import jpt.TypeDefs;
import jpt.Logger;
import std;


// Include keyword
// Exclude keyword
// File formats
// Min/Max file size
// Date of last modification

class FilterStrategy_Keyword_Include
{
protected:
    std::string m_keyword;
    bool m_isCaseSensitive = true;

public:
    FilterStrategy_Keyword_Include(std::string_view keyword, bool isCaseSensitive = true)
        : m_keyword(keyword)
        , m_isCaseSensitive(isCaseSensitive)
    {
        if (!m_isCaseSensitive)
        {
            for (char& c : m_keyword) 
            {
                c = std::tolower(c);
            }
        }
    }

    [[nodiscard]] bool Verify(const std::filesystem::directory_entry& entry) const
    {
        const std::filesystem::path filename = entry.path().filename();
        std::string filenameStr = filename.string();

        if (!m_isCaseSensitive)
        {
            for (char& c : filenameStr) 
            {
                c = std::tolower(c);
            }
        }

        return filenameStr.contains(m_keyword);
    }

    std::string_view GetKeyword() const { return m_keyword; }
};

class FilterStrategy_Keyword_Exclude : public FilterStrategy_Keyword_Include
{
private:
    bool m_checkFullPath = true;

public:
    FilterStrategy_Keyword_Exclude(std::string_view keyword, bool isCaseSensitive = true, bool checkFullPath = true)
        : FilterStrategy_Keyword_Include(keyword, isCaseSensitive)
        , m_checkFullPath(checkFullPath)
    {
    }

    [[nodiscard]] bool Verify(const std::filesystem::directory_entry& entry) const
    {
        if (m_checkFullPath)
        {
            const std::filesystem::path path = entry.path();
            std::string pathStr = path.string();

            if (!m_isCaseSensitive)
            {
               for (char& c : pathStr) 
               {
                   c = std::tolower(c);
               }
            }

            return !pathStr.contains(m_keyword);
        }

        return !FilterStrategy_Keyword_Include::Verify(entry);
    }
};

class FilterStrategy_Extension
{
public:
    enum Type : std::uint32_t
    {
        kNone   = 0,

        kTxt    = (1 << 0),
        kCppm   = (1 << 1),
        kCpp    = (1 << 2),
        kHeader = (1 << 3),
        kJson   = (1 << 4),
        kHpp    = (1 << 5),
        kObj    = (1 << 6),
        kPcm    = (1 << 7),
    };

private:
    std::uint32_t m_type;

public:
    FilterStrategy_Extension(std::uint32_t type)
        : m_type(type)
    {
    }

    [[nodiscard]] bool Verify(const std::filesystem::directory_entry& entry) const
    {
        const std::filesystem::path extension = entry.path().extension();
        const std::string extensionStr = extension.string();
        const Type type = ToType(extensionStr);
        return (m_type & type) != 0;
    }

private:
    [[nodiscard]] Type ToType(std::string_view extensionStr) const
    {
        if (extensionStr == ".txt")
        {
            return Type::kTxt;
        }
        else if (extensionStr == ".cppm")
        {
            return Type::kCppm;
        }
        else if (extensionStr == ".cpp")
        {
            return Type::kCpp;
        }
        else if (extensionStr == ".h")
        {
            return Type::kHeader;
        }
        else if (extensionStr == ".json")
        {
            return Type::kJson;
        }
        else if (extensionStr == ".hpp")
        {
            return Type::kHpp;
        }
        else if (extensionStr == ".obj")
        {
            return Type::kObj;
        }
        else if (extensionStr == ".pcm")
        {
            return Type::kPcm;
        }
        else
        {
            return Type::kNone;
        }
    }
};

class FilterStrategy_SizeCap
{
private:
    std::uintmax_t m_min = std::numeric_limits<std::uintmax_t>::min();
    std::uintmax_t m_max = std::numeric_limits<std::uintmax_t>::max();

public:
    FilterStrategy_SizeCap(std::uintmax_t min = std::numeric_limits<std::uintmax_t>::min(), std::uintmax_t max = std::numeric_limits<std::uintmax_t>::max())
        : m_min(min)
        , m_max(max)
    {
    }

    [[nodiscard]] bool Verify(const std::filesystem::directory_entry& entry) const
    {
        const std::uintmax_t size = entry.file_size();
        return size >= m_min && size <= m_max;
    }
};

using Filter = std::variant<FilterStrategy_Keyword_Include, 
                            FilterStrategy_Keyword_Exclude,
                            FilterStrategy_Extension, 
                            FilterStrategy_SizeCap
                            >;

/* It should return the files within a folder that match different filters, like size min/max cap, date of last modification, key word in filename, file formats, etc */
std::vector<std::filesystem::directory_entry> FindFiles(const std::filesystem::path& directory, const std::vector<Filter>& filters)
{
    if (!std::filesystem::exists(directory))
    {
        return {};
    }

    std::vector<std::filesystem::directory_entry> result;

    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(directory))
    {
        if (entry.is_directory())
        {
            continue;
        }

        auto matches = [&entry](const Filter& filter)
        {
            auto visitor = [&entry](const auto& strategy) { return strategy.Verify(entry); };
            return std::visit(visitor, filter);
        };

        if (std::ranges::all_of(filters, matches))
        {
            result.emplace_back(entry);
        }
    }

    return result;
}

export void RunUnitTests_FindFiles(jpt::TestCase& )
{
    std::vector<std::filesystem::directory_entry> files = FindFiles("C:\\Users\\szx07\\repos\\Jupiter-Engine", { FilterStrategy_Keyword_Include("Input"),
                                                                                                                 FilterStrategy_Keyword_Exclude("_ProjectFiles"),
                                                                                                                 FilterStrategy_Extension(FilterStrategy_Extension::Type::kObj | FilterStrategy_Extension::Type::kPcm | FilterStrategy_Extension::Type::kCpp | FilterStrategy_Extension::Type::kCppm | FilterStrategy_Extension::Type::kHpp),
                                                                                                                 FilterStrategy_SizeCap(10),
                                                                                                               });

    std::sort(files.begin(), files.end(), [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b) -> bool
    {
        return a.last_write_time() < b.last_write_time();
    });

    for (const std::filesystem::directory_entry& entry : files)
    {
        jpt::Debug::Log("{}", entry.path().string());
    }

    //test.Expect(FindFiles("C:\\Users\\szx07\\repos\\Jupiter-Engine").empty(), "Expected no files to be found");
}
