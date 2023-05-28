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

	jpt::list<int32> list{ 0,1,2,3,4 };
	jpt::vector<jpt::string> vector{ "0","1","2","3","4" };
	jpt::pair<jpt::list<int32>, jpt::vector<jpt::string>> pair3 = jpt::make_pair<jpt::list<int32>, jpt::vector<jpt::string>>(list, vector);
	for (int32 num : pair3.first)
	{
		JPT_RUN_UNIT_TEST(jpt::to_string(num), pair3.second[num]);
	}

	return true;
}

bool UnitTest_Vector()
{
	// Primitive Types
	int32 i = 0;

	jpt::vector<int32> jptVectorInt{ 0,1,2 };
	JPT_RUN_UNIT_TEST(jptVectorInt.size(), 3);
	for (jpt::vector<int32>::iterator itr = jptVectorInt.begin(); itr != jptVectorInt.end(); ++itr)
	{
		JPT_RUN_UNIT_TEST(*itr, i);
		++i;
	}

	i = 0;
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

	copiedIntVector.clear();

	copiedIntVector.resize(5);
	for (int32 num: copiedIntVector)
	{
		JPT_RUN_UNIT_TEST(num, 0);
	}

	// Non-Trivial Objects
	jpt::vector<jpt::string> jptVectorStr{ "0", "1", "2" };
	JPT_RUN_UNIT_TEST(jptVectorStr.size(), 3);
	i = 0;
	for (jpt::vector<jpt::string>::iterator itr = jptVectorStr.begin(); itr != jptVectorStr.end(); ++itr)
	{
		JPT_RUN_UNIT_TEST(*itr, jpt::to_string(i));
		++i;
	}

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

	copiedStrVector.clear();

	copiedStrVector.resize(5, "resized content");
	for (const jpt::string& str : copiedStrVector)
	{
		JPT_RUN_UNIT_TEST(str, "resized content");
	}

	return true;
}

bool UnitTest_List()
{
	int32 i = 0;

	// Primitive types
	jpt::list<int32> intList{ 3,5,7 };
	intList.insert(1, 4);
	intList.insert(3, 6);
	intList.emplace_front(2);
	intList.push_front(1);
	intList.push_front(i);	// 0
	intList.emplace_back(8);
	intList.push_back(9);

	// 0,1,2,3,4,5,6,7,8,9
	for (jpt::list<int32>::iterator itr = intList.begin(); itr != intList.end(); ++itr)
	{
		JPT_RUN_UNIT_TEST(*itr, i);
		++i;
	}

	intList.pop_front();
	intList.pop_front();
	intList.pop_back();
	intList.pop_back();
	// 2,3,4,5,6,7

	intList.erase(1);
	intList.erase(2);
	intList.erase(3);
	// 2,4,6

	i = 2;
	for (jpt::list<int32>::iterator itr = intList.begin(); itr != intList.end(); ++itr)
	{
		JPT_RUN_UNIT_TEST(*itr, i);
		i += 2;
	}

	jpt::list<int32> anotherCopiedIntList(intList);

	i = 2;
	for (jpt::list<int32>::iterator itr = anotherCopiedIntList.begin(); itr != anotherCopiedIntList.end(); ++itr)
	{
		JPT_RUN_UNIT_TEST(*itr, i);
		i += 2;
	}

	jpt::list<int32> anotherMovedIntList;
	anotherMovedIntList = jpt::move(intList);

	i = 2;
	for (jpt::list<int32>::iterator itr = anotherMovedIntList.begin(); itr != anotherMovedIntList.end(); ++itr)
	{
		JPT_RUN_UNIT_TEST(*itr, i);
		i += 2;
	}

	// Non-trivial types
	i = 0;
	jpt::list<jpt::string> strList{ "3","5","7" };

	strList.insert(1, "4");
	strList.insert(3, "6");
	strList.emplace_front("2");
	strList.push_front("1");
	strList.push_front(jpt::to_string(i));	// 0
	strList.emplace_back(jpt::to_string(8));
	strList.push_back("9");

	// 0,1,2,3,4,5,6,7,8,9
	for (jpt::list<jpt::string>::iterator itr = strList.begin(); itr != strList.end(); ++itr)
	{
		JPT_RUN_UNIT_TEST(*itr, jpt::to_string(i));
		++i;
	}

	strList.pop_front();
	strList.pop_front();
	strList.pop_back();
	strList.pop_back();
	// 2,3,4,5,6,7

	strList.erase(1);
	strList.erase(2);
	strList.erase(3);
	// 2,4,6

	i = 2;
	for (jpt::list<jpt::string>::iterator itr = strList.begin(); itr != strList.end(); ++itr)
	{
		JPT_RUN_UNIT_TEST(*itr, jpt::to_string(i));
		i += 2;
	}

	jpt::list<jpt::string> anotherCopiedStrList(strList);
	i = 2;
	for (jpt::list<jpt::string>::iterator itr = anotherCopiedStrList.begin(); itr != anotherCopiedStrList.end(); ++itr)
	{
		JPT_RUN_UNIT_TEST(*itr, jpt::to_string(i));
		i += 2;
	}

	jpt::list<jpt::string> anotherMovedStrList;
	anotherMovedStrList = jpt::move(strList);

	i = 2;
	for (jpt::list<jpt::string>::iterator itr = anotherMovedStrList.begin(); itr != anotherMovedStrList.end(); ++itr)
	{
		JPT_RUN_UNIT_TEST(*itr, jpt::to_string(i));
		i += 2;
	}

	return true;
}

bool UnitTest_unordered_map()
{
	jpt::unordered_map<int32, jpt::string> jptIntCharUnorderedMap;
	int32 key = 0;

	jptIntCharUnorderedMap.insert({ 0, "A" });
	JPT_CHECK_RESULT(jptIntCharUnorderedMap[0] == "A");
	jptIntCharUnorderedMap[1] = "C";
	JPT_CHECK_RESULT(jptIntCharUnorderedMap[1] == "C");

	jptIntCharUnorderedMap[key] = "B";
	jptIntCharUnorderedMap[1] = "D";
	JPT_CHECK_RESULT(jptIntCharUnorderedMap[0] == "B");
	JPT_CHECK_RESULT(jptIntCharUnorderedMap[1] == "D");

	jptIntCharUnorderedMap[key + 5] = "E";
	JPT_CHECK_RESULT(jptIntCharUnorderedMap[key + 5] == "E");

	JPT_RUN_UNIT_TEST(jptIntCharUnorderedMap.contains(0), true);
	JPT_RUN_UNIT_TEST(jptIntCharUnorderedMap.contains(4), false);

	jptIntCharUnorderedMap[6] = "1";
	JPT_RUN_UNIT_TEST(jptIntCharUnorderedMap.find(6)->second, "1");
	//jptIntCharUnorderedMap.erase(6);
	//JPT_RUN_UNIT_TEST(jptIntCharUnorderedMap.contains(6), false);

	for (auto& [k, v] : jptIntCharUnorderedMap)
	{
		JPT_LOG("%d: %s", k, v.c_str());
		v = "Test Ref in reference";
	}

	for (auto& [k, v] : jptIntCharUnorderedMap)
	{
		JPT_LOG("%d: %s", k, v.c_str());
	}

	return true;
}

void RunDataStructuresUnitTests()
{
	JPT_RUN_UNIT_TESTS(UnitTest_Pair);
	JPT_RUN_UNIT_TESTS(UnitTest_Vector);
	JPT_RUN_UNIT_TESTS(UnitTest_List);
	JPT_RUN_UNIT_TESTS(UnitTest_unordered_map);
}