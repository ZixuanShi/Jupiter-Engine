// Copyright Jupiter Technologies, Inc. All Rights Reserved.

#pragma once

namespace jpt
{
	struct EnumStringInfo;

	struct JPT_API EnumSearchInfo
	{
		// The whole string for all enum values passed along
		const char* m_wholeString;

		// String info for each enum value
		const EnumStringInfo* m_nameParts;

		int32 m_startValue;
		size_t m_count;
	};

	/* Used to store the sections of the string that belong to each enum value */
	struct JPT_API EnumStringInfo
	{
		// Index start and end for an enum value
		size_t m_start = kInvalidValue<uint16>;
		size_t m_end = kInvalidValue<uint16>;

		size_t Len() const { return m_end - m_start; }

		/* Builds Enum search info when creating it
			@param string:	The string of values got passed in when creating an enum 
			@param infos:	A static array of enum string infos, with the size of count of this enum's values 
			@param start:	Starting value. Typically 0
			@param count:	How many values this enum category has 
			@return			The search info for this enum category */
		static EnumSearchInfo BuildEnumStringInfos(const char* string, EnumStringInfo* infos, int32 start, size_t count);

		/* Converts an enum value to a jpt::string literal
			@param value: Enum's value in integer
			@param searchInfo: Will be generated as static member when creating the enum and been passed in
			@return		A jpt::string with the enum's value string literal */
		static jpt::string ToString(int32 value, const EnumSearchInfo& searchInfo);

		/* Helper for finding the value by it's string literal
			@param string:		The enum's literal to find 
			@param searchInfo:	Auto generated static search info for this enum category
			@param foundValue:	A reference to retrieve the value of string.
			@return		true if found the string in this enum. false if not */
		static bool FindValue(const char* string, const EnumSearchInfo& searchInfo, int32& foundValue);
	};

	/* Typed enum wrapper. Use this struct as the actual enum type. */
	template<typename EnumInfo>
	class Enum : public EnumInfo
	{
	public:
		using EnumType = typename EnumInfo::EnumType;
		static const EnumSearchInfo s_searchInfo;

	private:
		EnumType m_value;

	public:
		Enum();
		Enum(EnumType value);
		explicit Enum(int32 value);
		Enum(const Enum& other);

		operator EnumType() const { return m_value; }
		EnumType operator++();
		EnumType operator++(int32);
		EnumType operator--();
		EnumType operator--(int32);

	public:
		jpt::string ToString() const { return EnumStringInfo::ToString(m_value, s_searchInfo); }
		static jpt::string ToString(EnumType value) { return EnumStringInfo::ToString(value, s_searchInfo); }
		static jpt::string ToString(int32 value) { return EnumStringInfo::ToString(value, s_searchInfo); }

		bool FromString(const char* string);
		static EnumType EnumFromString(const char* string);
	};

	template<typename EnumInfo>
	const EnumSearchInfo Enum<EnumInfo>::s_searchInfo = EnumInfo::GetEnumSearchInfo();

	template<typename EnumInfo>
	inline Enum<EnumInfo>::Enum()
		: m_value(EnumInfo::start)
	{
	}

	template<typename EnumInfo>
	inline Enum<EnumInfo>::Enum(EnumType value)
		: m_value(value)
	{
	}

	template<typename EnumInfo>
	inline Enum<EnumInfo>::Enum(int32 value)
		: m_value(static_cast<EnumType>(value))
	{
	}

	template<typename EnumInfo>
	inline Enum<EnumInfo>::Enum(const Enum& other)
		: m_value(other.m_value)
	{
	}

	template<typename EnumInfo>
	inline Enum<EnumInfo>::EnumType Enum<EnumInfo>::operator++()
	{
		m_value = static_cast<EnumType>(m_value + 1);
		return m_value;
	}

	template<typename EnumInfo>
	inline Enum<EnumInfo>::EnumType Enum<EnumInfo>::operator++(int32)
	{
		EnumType temp = m_value;
		++m_value;
		return temp;
	}

	template<typename EnumInfo>
	inline Enum<EnumInfo>::EnumType Enum<EnumInfo>::operator--()
	{
		m_value = static_cast<EnumType>(m_value - 1);
		return m_value;
	}

	template<typename EnumInfo>
	inline Enum<EnumInfo>::EnumType Enum<EnumInfo>::operator--(int32)
	{
		EnumType temp = m_value;
		--m_value;
		return temp;
	}

	template<typename EnumInfo>
	inline bool Enum<EnumInfo>::FromString(const char* string)
	{
		static_assert(sizeof(m_value) == sizeof(int32));
		return EnumStringInfo::FindValue(string, s_searchInfo, static_cast<int32&>(m_value));
	}

	template<typename EnumInfo>
	inline Enum<EnumInfo>::EnumType Enum<EnumInfo>::EnumFromString(const char* string)
	{
		const int32 value = EnumInfo::start;
		EnumStringInfo::FindValue(string, s_searchInfo, value);
		return EnumType(value);
	}
}

/* EEnum e = EEnum::Value;
   jpt::string eString = e.ToString(); 
   
   The base class defined 3 default elements:
   Enum::Start: The starting value you specified in the macro
   Enum::End:   start + count
   Enum::Count: The number of elements in the enum (not 1 + the last value) */
	#define JPT_ENUM_WITH_START(type, start, first, ...)\
		struct type##Info\
		{\
			enum EnumType { first = start, Start = start, __VA_ARGS__, End, Count = End - Start }; \
			\
		protected:\
			static inline jpt::EnumSearchInfo GetEnumSearchInfo()\
			{ \
				static jpt::EnumStringInfo s_enumStringInfos[Count]; \
				return jpt::EnumStringInfo::BuildEnumStringInfos(#first", "#__VA_ARGS__, s_enumStringInfos, Start, Count); \
			}\
		}; \
		typedef jpt::Enum<type##Info> type;

/* Declares an enum class with additional features. Like item to jpt::string literal, indexed with start/end, for loop through. etc
	@param enumName: Required as the name of this enum. Example: EFruit 
	@param first:	 Required as the first value in the enum. Example: Apple
	@param ...:      Optional. The rest of values to be added to the enum. Examples: Banana, Orange. */
	#define JPT_ENUM(type, first, ...) JPT_ENUM_WITH_START(type, 0, first, __VA_ARGS__)
