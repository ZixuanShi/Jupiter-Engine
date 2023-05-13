#include "Core/Building/JupiterPCH.h"
#include "DataStructuresUnitTests.h"

bool UnitTest_Pair()
{
	jpt::pair<int32, float> pair1 = jpt::make_pair<int32, float>(1, 1.5f);
	JPT_RUN_UNIT_TEST(pair1.first, 1);
	JPT_RUN_UNIT_TEST(pair1.second, 1.5f);

	pair1 = jpt::make_pair<int32, float>(2, 3.5f);
	JPT_RUN_UNIT_TEST(pair1.first, 2);
	JPT_RUN_UNIT_TEST(pair1.second, 3.5f);

	jpt::pair<jpt::string, jpt::vector<jpt::string>> pair2 = jpt::make_pair<jpt::string, jpt::vector<jpt::string>>("TestStr", { "TestVecElement1","TestVecElement2" });
	JPT_RUN_UNIT_TEST(pair2.first, "TestStr");
	JPT_RUN_UNIT_TEST(pair2.second[0], "TestVecElement1");
	JPT_RUN_UNIT_TEST(pair2.second[1], "TestVecElement2");

	pair2 = jpt::make_pair<jpt::string, jpt::vector<jpt::string>>("NewTestStr", { "NewTestVecElement1","NewTestVecElement2", "NewTestVecElement3"});
	JPT_RUN_UNIT_TEST(pair2.first, "NewTestStr");
	JPT_RUN_UNIT_TEST(pair2.second[0], "NewTestVecElement1");
	JPT_RUN_UNIT_TEST(pair2.second[1], "NewTestVecElement2");
	JPT_RUN_UNIT_TEST(pair2.second[2], "NewTestVecElement3");

	return true;
}

bool UnitTest_Vector()
{
	// Primitive Types
	jpt::vector<int32> jptVectorInt{ 0,1,2 };
	JPT_RUN_UNIT_TEST(jptVectorInt.size(), 3);

	int32 i = 0;
	for (int32 num : jptVectorInt)
	{
		JPT_RUN_UNIT_TEST(num, i);
		++i;
	}

	jptVectorInt.reserve(5);
	JPT_RUN_UNIT_TEST(jptVectorInt.capacity(), 5);

	jptVectorInt.push_back(44);
	JPT_RUN_UNIT_TEST(jptVectorInt[3], 44);

	jptVectorInt.emplace_back(55);
	JPT_RUN_UNIT_TEST(jptVectorInt.back(), 55);

	jptVectorInt.pop_back();
	JPT_RUN_UNIT_TEST(jptVectorInt.size(), 4);
	JPT_RUN_UNIT_TEST(jptVectorInt.back(), 44);

	jpt::vector<int32> anotherIntVector;
	anotherIntVector = jpt::move(jptVectorInt);
	JPT_RUN_UNIT_TEST(anotherIntVector.size(), 4);
	JPT_RUN_UNIT_TEST(jptVectorInt.size(), 0);
	JPT_RUN_UNIT_TEST(jptVectorInt.data(), nullptr);

	anotherIntVector.pop_back();
	i = 0;
	for (int32 num : anotherIntVector)
	{
		JPT_RUN_UNIT_TEST(num, i);
		++i;
	}

	jpt::swap(anotherIntVector[0], anotherIntVector[2]);
	JPT_RUN_UNIT_TEST(anotherIntVector[0], 2);
	JPT_RUN_UNIT_TEST(anotherIntVector[2], 0);

	int32& tempNum = anotherIntVector[1];
	JPT_RUN_UNIT_TEST(tempNum, 1);

	anotherIntVector.erase(1);
	JPT_RUN_UNIT_TEST(anotherIntVector.size(), 2);
	JPT_RUN_UNIT_TEST(anotherIntVector[1], 0);

	jpt::vector<int32> copiedIntVector(anotherIntVector);
	JPT_RUN_UNIT_TEST(copiedIntVector.size(), 2);
	JPT_RUN_UNIT_TEST(anotherIntVector.size(), 2);

	// Non-Trivial Objects
	jpt::vector<jpt::string> jptVectorStr{ "0", "1", "2" };
	JPT_RUN_UNIT_TEST(jptVectorStr.size(), 3);

	i = 0;
	for (const jpt::string& str : jptVectorStr)
	{
		JPT_RUN_UNIT_TEST(str, jpt::to_string(i));
		++i;
	}

	jptVectorStr.reserve(5);
	JPT_RUN_UNIT_TEST(jptVectorStr.capacity(), 5);

	jptVectorStr.push_back("Test");
	JPT_RUN_UNIT_TEST(jptVectorStr[3], "Test");

	jptVectorStr.emplace_back("Jupiter");
	JPT_RUN_UNIT_TEST(jptVectorStr[4], "Jupiter");

	jptVectorStr.pop_back();
	JPT_RUN_UNIT_TEST(jptVectorStr.size(), 4);
	JPT_RUN_UNIT_TEST(jptVectorStr.back(), "Test");

	jpt::vector<jpt::string> anotherStrVector;
	anotherStrVector = jpt::move(jptVectorStr);
	JPT_RUN_UNIT_TEST(anotherStrVector.size(), 4);
	JPT_RUN_UNIT_TEST(jptVectorStr.size(), 0);
	JPT_RUN_UNIT_TEST(jptVectorStr.data(), nullptr);

	anotherStrVector.pop_back();
	i = 0;
	for (const jpt::string& str : anotherStrVector)
	{
		JPT_RUN_UNIT_TEST(str, jpt::to_string(i));
		++i;
	}

	jpt::swap(anotherStrVector[0], anotherStrVector[2]);
	JPT_RUN_UNIT_TEST(anotherStrVector[0], "2");
	JPT_RUN_UNIT_TEST(anotherStrVector[2], "0");

	const jpt::string& temp = anotherStrVector[1];
	JPT_RUN_UNIT_TEST(temp, "1");

	anotherStrVector.erase(1);
	JPT_RUN_UNIT_TEST(anotherStrVector.size(), 2);
	JPT_RUN_UNIT_TEST(anotherStrVector[1], "0");

	jpt::vector<jpt::string> copiedStrVector{ "123","456" };
	copiedStrVector = (anotherStrVector);
	JPT_RUN_UNIT_TEST(copiedStrVector.size(), 2);
	JPT_RUN_UNIT_TEST(anotherStrVector.size(), 2);

	copiedStrVector.insert(1, "1");
	copiedStrVector.insert(0, "3");
	JPT_RUN_UNIT_TEST(copiedStrVector.size(), 4);

	return true;
}

void RunDataStructuresUnitTests()
{
	JPT_RUN_UNIT_TESTS(UnitTest_Pair);
	JPT_RUN_UNIT_TESTS(UnitTest_Vector);
}