#include "hobject_fmt.h"
#include <fmt/compile.h>


#if 0
int main() {

  auto o1 = ghobject_t::generate_test_instances();
  for (auto g1 : o1) {
    std::cout << fmt::format("{} \tto_str: {}", g1->hobj, g1->hobj.to_str()) << std::endl;
    std::cout << g1->hobj << "\n";
  }

  std::string test_1 = fmt::format("{}", o1.front()->hobj.snap);

  for (auto g1 : o1) {
    auto st1 = g1->hobj.to_str();
    auto st2 = g1->hobj.to_str3();
    //std::cout << fmt::format(FMT_COMPILE("== {} <> {}\n"), st1, st2);
    std::cout << fmt::format("== {} <> {}\n", st1, st2);
    assert(false || st1 == st2);
  }

  return 0;
}
#else

static void obj_tostr(benchmark::State& state)
{
  auto o1 = ghobject_t::generate_test_instances();
  for (auto _ : state) {
    for (auto g1 : o1) {
      std::string r = g1->hobj.to_str();
      benchmark::DoNotOptimize(r);
    }
  }
}
// Register the function as a benchmark
BENCHMARK(obj_tostr);

static void obj_tostr_e5(benchmark::State& state)
{
  auto o1 = ghobject_t::generate_test_instances();
  for (auto _ : state) {
    for (auto g1 : o1) {
      std::string r = g1->hobj.to_str_esc5();
      benchmark::DoNotOptimize(r);
    }
  }
}
// Register the function as a benchmark
BENCHMARK(obj_tostr_e5);

static void obj_tostr3(benchmark::State& state)
{
  auto o1 = ghobject_t::generate_test_instances();
  for (auto _ : state) {
    for (auto g1 : o1) {
      std::string r = g1->hobj.to_str3();
      benchmark::DoNotOptimize(r);
    }
  }
}
// Register the function as a benchmark
BENCHMARK(obj_tostr3);

static void obj_tostr4(benchmark::State& state)
{
  auto o1 = ghobject_t::generate_test_instances();
  for (auto _ : state) {
    for (auto g1 : o1) {
      std::string r = g1->hobj.to_str4();
      benchmark::DoNotOptimize(r);
    }
  }
}
// Register the function as a benchmark
BENCHMARK(obj_tostr4);

static void obj_tostr2(benchmark::State& state)
{
  auto o1 = ghobject_t::generate_test_instances();
  for (auto _ : state) {
    for (auto g1 : o1) {
      std::string r = g1->hobj.to_str2();
      benchmark::DoNotOptimize(r);
    }
  }
}
// Register the function as a benchmark
BENCHMARK(obj_tostr2);

BENCHMARK_MAIN();

#endif
