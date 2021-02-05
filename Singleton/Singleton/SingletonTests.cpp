#include "Singleton.hpp"
#include <gtest/gtest.h>

//TEST(DatabaseTests, IsSingletonTest)
//{
//  auto& db = SingletonDatabase::get();
//  auto& db2 = SingletonDatabase::get();
//  ASSERT_EQ(1, db.instance_count);
//  ASSERT_EQ(1, db2.instance_count);
//}


/**
 * 실제 데이터를 이용해서 테스트해야 한다.
 * 데이터는 언제든 바뀔 수 있다. 그때마다 테스트 코드를 수정하는 것은 소모적
 * 현재 구조에서는 더미 컴포넌트를 사용하는 것이 불가능하다.
 */
TEST(RecordFinderTests, SingletonTotalPopulationTest)
{
  SingletonRecordFinder rf;
  std::vector<std::string> names{ "Seoul", "Mexico City" };
  int tp = rf.total_population(names);
  EXPECT_EQ(17500000 + 17400000, tp);
}

TEST(RecordFinderTests, DependantTotalPopulationTest)
{
  DummyDatabase db{};
  ConfigurableRecordFinder rf{ db };
  EXPECT_EQ(4, rf.total_population(
    std::vector<std::string>{"alpha", "gamma"}));
}

int dsfmain(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av); 
  return RUN_ALL_TESTS();
}
