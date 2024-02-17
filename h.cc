#include <fmt/compile.h>

#include "hobject_fmt.h"


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
#elif 0

// testing to_str()

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


#else
// a class that uses the old (existing) formatter

struct hobj2 : public hobject_t {

  template <typename... ARGS>
  hobj2(ARGS&&... args) : hobject_t{std::forward<ARGS>(args)...}
  {}

  hobj2(const hobj2& rhs) = default;
  hobj2(hobj2&& rhs) = default;
  hobj2& operator=(const hobj2& rhs) = default;
  hobj2& operator=(hobj2&& rhs) = default;
  hobj2(hobject_t_max &&singleton) : hobj2() {
    max = true;
  }
  hobj2 &operator=(hobject_t_max &&singleton) {
    *this = hobject_t();
    max = true;
    return *this;
  }
  bool is_max() const {
    //ceph_assert(!max || (*this == hobject_t(hobject_t::get_max())));
    return max;
  }
  bool is_min() const {
    // this needs to match how it's constructed
    return snap == 0 &&
	   hash == 0 &&
	   !max &&
	   pool == INT64_MIN;
  }

  static std::list<hobj2*> generate_test_instances();
};



namespace {
// \todo reimplement
static inline void app_hobj2(const std::string& in, std::string* out)
{
  for (auto i = in.cbegin(); i != in.cend(); ++i) {
    if (*i == '%' || *i == ':' || *i == '/' || *i < 32 || *i >= 127) {
      char buf[4];
      snprintf(buf, sizeof(buf), "%%%02x", (int)(unsigned char)*i);
      out->append(buf);
    } else {
      out->push_back(*i);
    }
  }
}
}  // namespace

namespace fmt {
template <> struct formatter<hobj2> {

  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext> auto format(const hobj2& ho, FormatContext& ctx)
  {
    if (static_cast<hobject_t>(ho) == hobject_t{}) {
      return fmt::format_to(ctx.out(), "MIN");
    }

    if (static_cast<hobject_t>(ho).is_max()) {
      return fmt::format_to(ctx.out(), "MAX");
    }

    std::string v;
    app_hobj2(ho.nspace, &v);
    v.push_back(':');
    app_hobj2(ho.get_key(), &v);
    v.push_back(':');
    app_hobj2(ho.oid.name, &v);

    return fmt::format_to(ctx.out(), FMT_COMPILE("{}:{:08x}:{}:{}"), static_cast<uint64_t>(ho.pool),
			  ho.get_bitwise_key_u32(), v, ho.snap);
  }
};
}  // namespace fmt



list<hobj2*> hobj2::generate_test_instances()
{
  list<hobj2*> o;
  o.push_back(new hobj2);
  o.push_back(new hobj2);
  o.back()->max = true;
  o.push_back(new hobj2(hobject_t(object_t("oname"), string(), 1, 234, -1, "")));

  o.push_back(
    new hobj2(hobject_t(object_t("o%:/name2"), string("okey"), CEPH_NOSNAP, 67, 0, "n1")));
  o.push_back(
    new hobj2(hobject_t(object_t("on%%//ame2"), string("okey"), CEPH_NOSNAP, 67, 0, "n1")));
  o.push_back(
    new hobj2(hobject_t(object_t("ona\030me2"), string("okey"), CEPH_NOSNAP, 67, 0, "n1")));
  o.push_back(new hobj2(
    hobject_t(object_t("oname3"), string("oname3"), CEPH_SNAPDIR, 910, 1, "n2")));
  o.push_back(new hobj2(
    hobject_t(object_t("onasss\002sssssssssssssssssssssssssssssssssssssssssssssssssme3"), string("oname3"), CEPH_SNAPDIR, 910, 1, "n2")));
  o.push_back(new hobj2(
    hobject_t(object_t("oname3"), string("oname3"), CEPH_SNAPDIR, 910, 1, "n2")));
  o.push_back(new hobj2(
    hobject_t(object_t("o//////name3"), string("oname3"), CEPH_SNAPDIR, 910, 1, "n2")));
  o.push_back(new hobj2(
    hobject_t(object_t("oname3"), string("on::::::::::::::::ame3"), CEPH_SNAPDIR, 910, 1, "n2")));

  return o;
}

#if 1
int main()
{

  auto o1 = hobj2::generate_test_instances();
  for (auto g1 : o1) {

    hobject_t as_hobj = *g1;
    std::cout << fmt::format("old:\t{}\nnew:\t{}\n\n", *g1, as_hobj) << std::endl;

    //std::cout << fmt::format("{} \tto_str: {}", *g1, g1->to_str()) << std::endl;
    //std::cout << *g1 << "\n";
  }

  // std::string test_1 = fmt::format("{}", o1.front()->hobj.snap);

  //   for (auto g1 : o1) {
  //     auto st1 = g1->hobj.to_str();
  //     auto st2 = g1->hobj.to_str3();
  //     //std::cout << fmt::format(FMT_COMPILE("== {} <> {}\n"), st1, st2);
  //     std::cout << fmt::format("== {} <> {}\n", st1, st2);
  //     assert(false || st1 == st2);
  //   }

  return 0;
}


#else

// testing hobject formatter

static void hobj_orig(benchmark::State& state)
{
  auto o1 = hobj2::generate_test_instances();
  for (auto _ : state) {
    for (auto g1 : o1) {
      std::string r = fmt::format("{}", *g1);
      benchmark::DoNotOptimize(r);
    }
  }
}
// Register the function as a benchmark
BENCHMARK(hobj_orig);


static void hobj_new1(benchmark::State& state)
{
  auto o1 = hobj2::generate_test_instances();
  for (auto _ : state) {
    for (auto g1 : o1) {
      std::string r = fmt::format("{}", *static_cast<hobject_t*>(g1));
      benchmark::DoNotOptimize(r);
    }
  }
}
// Register the function as a benchmark
BENCHMARK(hobj_new1);

BENCHMARK_MAIN();

#endif
#endif
