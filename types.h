#pragma once

#include <string>
#include <list>
#include <set>
#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>
#include <map>
#include <vector>
#include <optional>
#include <ostream>
#include <iomanip>



struct shard_id_t {

  int8_t id;

  shard_id_t() : id(0) {}
  explicit shard_id_t(int8_t _id) : id(_id) {}

  constexpr operator int8_t() const { return id; }

  const static shard_id_t NO_SHARD;

//   void encode(ceph::buffer::list &bl) const {
//     using ceph::encode;
//     encode(id, bl);
//   }
//   void decode(ceph::buffer::list::const_iterator &bl) {
//     using ceph::decode;
//     decode(id, bl);
//   }
//   void dump(ceph::Formatter *f) const {
//     f->dump_int("id", id);
//   }
  static void generate_test_instances(std::list<shard_id_t*>& ls) {
    ls.push_back(new shard_id_t(1));
    ls.push_back(new shard_id_t(2));
  }
  //bool operator==(const shard_id_t&) const = default;
  //auto operator<=>(const shard_id_t&) const = default;
};

const inline shard_id_t shard_id_t::NO_SHARD = shard_id_t(-1);
