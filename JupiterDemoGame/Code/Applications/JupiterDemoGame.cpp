#include "Core/Building/JupiterPCH.h"
#include "Applications/EntryPoint.h"

#include "DemoGameApplication.h"

class Foo
{
public:
	int* m_pData;

	Foo()
		: m_pData(nullptr)
	{
	}

	Foo(int data) 
		: m_pData(new int(data))
	{
	}

	Foo(const Foo& other)
		: m_pData(new int(*other.m_pData))
	{
	}

	Foo& operator=(const Foo& other)
	{
		m_pData = new int(*other.m_pData);
		return *this;
	}

	~Foo()
	{
		Clean();
	}

	void Clean()
	{
		delete m_pData;
	}
};

jpt::Application* jpt::CreateApplication()
{
	JPT_START_TIMING_PROFILER("Create Application");

	int32* pInt = JPT_NEW(int32);
	JPT_WARNING("%i", *pInt);
	JPT_DELETE(pInt);

	float* pAnother = JPT_NEW_WITH_VALUE(float, 42.056f);
	JPT_ERROR("%.3f", *pAnother);
	JPT_DELETE(pAnother);

	//std::vector<Foo> foos{ 1,2,3,4 };
	//for (Foo& foo : foos)
	//{
	//	std::cout << *foo.m_pData << ", ";
	//}

	//jpt::string test = "Jupiter Engine";
	//JPT_LOG("%s", test.c_str());
	std::vector<jpt::string> strings{ "Jupiter Engine", "Zixuan Shi", "Jupiter Technologies" };
	JPT_LOG("%s", strings[0].c_str());


	// TODO:
	// - String add support operator= to initializerlist item
	// - Vector try change memcpy
	//for (size_t i = 0; i < strings.size(); ++i)
	//{
	//	JPT_LOG("%s", strings[i].c_str());
	//}
	for (const jpt::string& str : strings)
	{
		JPT_LOG("%s", str.c_str());
	}

	//jpt::vector<int32> vec{1,2,3,4,5};
	//for (int32& i : vec)
	//{
	//	i *= 2;
	//	JPT_LOG("%d", i);
	//}
	//for (auto itr = vec.begin(); itr != vec.end(); ++itr)
	//{
	//	*itr *= 2;
	//	JPT_LOG("%d", *itr);
	//}
	//for (size_t i = 0; i < vec.size(); ++i)
	//{
	//	JPT_LOG("%d", vec[i]);
	//}

	return JPT_NEW(DemoGameApplication);
}