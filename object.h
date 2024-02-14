#pragma once


#include <benchmark/benchmark.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <vector>
#include <array>
#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/ostream.h>
#include <cassert>
#include <list>


using namespace std;

namespace {
// reverse_bits:

uint32_t reverse_bits(uint32_t v) {
  if (v == 0)
    return v;

  /* reverse bits
   * swap odd and even bits
   */
  v = ((v >> 1) & 0x55555555) | ((v & 0x55555555) << 1);
  /* swap consecutive pairs */
  v = ((v >> 2) & 0x33333333) | ((v & 0x33333333) << 2);
  /* swap nibbles ... */
  v = ((v >> 4) & 0x0F0F0F0F) | ((v & 0x0F0F0F0F) << 4);
  /* swap bytes */
  v = ((v >> 8) & 0x00FF00FF) | ((v & 0x00FF00FF) << 8);
  /* swap 2-byte long pairs */
  v = ( v >> 16             ) | ( v               << 16);
  return v;
}

uint32_t reverse_nibbles(uint32_t retval) {
  /* reverse nibbles */
  retval = ((retval & 0x0f0f0f0f) << 4) | ((retval & 0xf0f0f0f0) >> 4);
  retval = ((retval & 0x00ff00ff) << 8) | ((retval & 0xff00ff00) >> 8);
  retval = ((retval & 0x0000ffff) << 16) | ((retval & 0xffff0000) >> 16);
  return retval;
}

} // namespace

// hash  (1)


// Robert Jenkins' function for mixing 32-bit values
// http://burtleburtle.net/bob/hash/evahash.html
// a, b = random bits, c = input and output

#define hashmix(a,b,c) \
	a=a-b;  a=a-c;  a=a^(c>>13); \
	b=b-c;  b=b-a;  b=b^(a<<8);  \
	c=c-a;  c=c-b;  c=c^(b>>13); \
	a=a-b;  a=a-c;  a=a^(c>>12); \
	b=b-c;  b=b-a;  b=b^(a<<16); \
	c=c-a;  c=c-b;  c=c^(b>>5);  \
	a=a-b;  a=a-c;  a=a^(c>>3); \
	b=b-c;  b=b-a;  b=b^(a<<10); \
	c=c-a;  c=c-b;  c=c^(b>>15);


//namespace ceph {

template <class _Key> struct rjhash { };

inline uint64_t rjhash64(uint64_t key) {
  key = (~key) + (key << 21); // key = (key << 21) - key - 1;
  key = key ^ (key >> 24);
  key = (key + (key << 3)) + (key << 8); // key * 265
  key = key ^ (key >> 14);
  key = (key + (key << 2)) + (key << 4); // key * 21
  key = key ^ (key >> 28);
  key = key + (key << 31);
  return key;
}

inline uint32_t rjhash32(uint32_t a) {
  a = (a+0x7ed55d16) + (a<<12);
  a = (a^0xc761c23c) ^ (a>>19);
  a = (a+0x165667b1) + (a<<5);
  a = (a+0xd3a2646c) ^ (a<<9);
  a = (a+0xfd7046c5) + (a<<3);
  a = (a^0xb55a4f09) ^ (a>>16);
  return a;
}


template<> struct rjhash<uint32_t> {
  inline size_t operator()(const uint32_t x) const {
    return rjhash32(x);
  }
};

template<> struct rjhash<uint64_t> {
  inline size_t operator()(const uint64_t x) const {
    return rjhash64(x);
  }
};






#define CEPH_STR_HASH_LINUX      0x1  /* linux dcache hash */
#define CEPH_STR_HASH_RJENKINS   0x2  /* robert jenkins' */

extern unsigned ceph_str_hash_linux(const char *s, unsigned len);
extern unsigned ceph_str_hash_rjenkins(const char *s, unsigned len);

extern unsigned ceph_str_hash(int type, const char *s, unsigned len);
extern const char *ceph_str_hash_name(int type);
extern bool ceph_str_hash_valid(int type);




struct bl_t {

  std::string x_;
};

#define CEPH_SNAPDIR ((uint64_t)(-1))  /* reserved for hidden .snap dir */
#define CEPH_NOSNAP  ((uint64_t)(-2))  /* "head", "live" revision */
#define CEPH_MAXSNAP ((uint64_t)(-3))  /* largest valid snapid */


struct snapid_t {
  uint64_t val;
  // cppcheck-suppress noExplicitConstructor
  constexpr snapid_t(uint64_t v=0) : val(v) {}
  snapid_t operator+=(snapid_t o) { val += o.val; return *this; }
  snapid_t operator++() { ++val; return *this; }
  constexpr operator uint64_t() const { return val; }
  //auto operator<=>(const snapid_t&) const noexcept = default;
  //auto operator==(const snapid_t&) const noexcept = default;
};



struct object_t {
  std::string name;

  object_t() {}
  // cppcheck-suppress noExplicitConstructor
  object_t(const char *s) : name(s) {}
  // cppcheck-suppress noExplicitConstructor
  object_t(const std::string& s) : name(s) {}
  object_t(std::string&& s) : name(std::move(s)) {}
  object_t(std::string_view s) : name(s) {}

  auto operator<=>(const object_t&) const noexcept = default;

  void swap(object_t& o) {
    name.swap(o.name);
  }
  void clear() {
    name.clear();
  }

  //void encode(bl_t& bl) const {
  //  using ceph::encode;
  //  encode(name, bl);
  //}
  //void decode(ceph::buffer::list::const_iterator &bl) {
  //  using ceph::decode;
  //  decode(name, bl);
  //}

 // void dump(ceph::Formatter *f) const {
 //   f->dump_string("name", name);
 // }

  static void generate_test_instances(std::list<object_t*>& o) {
    o.push_back(new object_t);
    o.push_back(new object_t("myobject"));
  }
};

namespace std {
template<> struct hash<object_t> {
  size_t operator()(const object_t& r) const {
    //static hash<string> H;
    //return H(r.name);
    return ceph_str_hash_linux(r.name.c_str(), r.name.length());
  }
};
} // namespace std


struct sobject_t {
  object_t oid;
  snapid_t snap;

  sobject_t() : snap(0) {}
  sobject_t(object_t o, snapid_t s) : oid(o), snap(s) {}

  constexpr auto operator<=>(const sobject_t&) const noexcept = default;

  void swap(sobject_t& o) {
    oid.swap(o.oid);
    snapid_t t = snap;
    snap = o.snap;
    o.snap = t;
  }
#if 0
  void encode(ceph::buffer::list& bl) const {
    using ceph::encode;
    encode(oid, bl);
    encode(snap, bl);
  }
  void decode(ceph::buffer::list::const_iterator& bl) {
    using ceph::decode;
    decode(oid, bl);
    decode(snap, bl);
  }
  void dump(ceph::Formatter *f) const {
    f->dump_stream("oid") << oid;
    f->dump_stream("snap") << snap;
  }
#endif
  static void generate_test_instances(std::list<sobject_t*>& o) {
    o.push_back(new sobject_t);
    o.push_back(new sobject_t(object_t("myobject"), 123));
  }
};


//inline std::ostream& operator<<(std::ostream& out, const sobject_t &o) {
//  return out << o.oid << "/" << o.snap;
//}

namespace std {
template<> struct hash<sobject_t> {
  size_t operator()(const sobject_t &r) const {
    static hash<object_t> H;
    static rjhash<uint64_t> I;
    return H(r.oid) ^ I(r.snap);
  }
};
} // namespace std


