#pragma once

/**
 * \file fmtlib formatters for some hobject.h classes
 */
#include <fmt/format.h>
#include <fmt/ranges.h>

#include "hobject.h"
#include "object_fmt.h"

namespace {
// \todo reimplement
static inline void append_out_escaped(const std::string& in, std::string* out)
{
  for (auto i = in.cbegin(); i != in.cend(); ++i) {
    if (*i == '%' || *i == ':' || *i == '/' || *i < 32 || *i >= 127) {
      out->append(fmt::format(FMT_COMPILE("%{:02x}"), (int)(unsigned char)*i));
    } else {
      out->push_back(*i);
    }
  }
}
}  // namespace


// namespace fmt {
//
// template <>
// struct formatter<snapid_t> {
//
//   constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
//
//   template <typename FormatContext>
//   auto format(const snapid_t& snp, FormatContext& ctx) const
//   {
//     if (snp == CEPH_NOSNAP) {
//       return format_to(ctx.out(), "head");
//     }
//     if (snp == CEPH_SNAPDIR) {
//       return format_to(ctx.out(), "snapdir");
//     }
//     return format_to(ctx.out(), "{:x}", snp.val);
//   }
// };
//
// }


namespace fmt {
template <> struct formatter<hobject_t> {

  template <typename FormatContext>
  static inline auto append_sanitized(FormatContext& ctx, const std::string& in, int sep=0)
  {
    for (auto i = in.cbegin(); i != in.cend(); ++i) {
      if (*i == '%' || *i == ':' || *i == '/' || *i < 32 || *i >= 127) {
	//fmt::format_to(ctx.out(), FMT_COMPILE("%{:02x}"), (int)(unsigned char)*i);
	fmt::format_to(ctx.out(), FMT_COMPILE("%{:02x}"), *i);
      } else {
	fmt::format_to(ctx.out(), FMT_COMPILE("{}"), *i);
      }
    }
    if (sep) {
      fmt::format_to(ctx.out(), FMT_COMPILE("{}"), char(sep));
    }
    return ctx.out();
  }
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext> auto format(const hobject_t& ho, FormatContext& ctx)
  {
    if (ho == hobject_t{}) {
      return fmt::format_to(ctx.out(), "MIN");
    }

    if (ho.is_max()) {
      return fmt::format_to(ctx.out(), "MAX");
    }

    fmt::format_to(ctx.out(), FMT_COMPILE("{}:{:08x}:"), static_cast<uint64_t>(ho.pool),
		   ho.get_bitwise_key_u32());
    append_sanitized(ctx, ho.nspace, ':');
    append_sanitized(ctx, ho.get_key(), ':');
    append_sanitized(ctx, ho.oid.name);
    return fmt::format_to(ctx.out(), FMT_COMPILE(":{}"), ho.snap);

    //
    //     std::string v;
    //     append_out_escaped(ho.nspace, &v);
    //     v.push_back(':');
    //     append_out_escaped(ho.get_key(), &v);
    //     v.push_back(':');
    //     append_out_escaped(ho.oid.name, &v);

    //return fmt::format_to(ctx.out(), FMT_COMPILE("{}:{:08x}:{}:{}"),
	//		  static_cast<uint64_t>(ho.pool), ho.get_bitwise_key_u32(), v,
	//		  ho.snap);
  }
};
// template <> struct formatter<hobject_t> {
//
//   constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
//
//   template <typename FormatContext> auto format(const hobject_t& ho, FormatContext&
//   ctx)
//   {
//     if (ho == hobject_t{}) {
//       return fmt::format_to(ctx.out(), "MIN");
//     }
//
//     if (ho.is_max()) {
//       return fmt::format_to(ctx.out(), "MAX");
//     }
//
//     std::string v;
//     append_out_escaped(ho.nspace, &v);
//     v.push_back(':');
//     append_out_escaped(ho.get_key(), &v);
//     v.push_back(':');
//     append_out_escaped(ho.oid.name, &v);
//
//     return fmt::format_to(ctx.out(), FMT_COMPILE("{}:{:08x}:{}:{}"),
//     static_cast<uint64_t>(ho.pool),
// 			  ho.get_bitwise_key_u32(), v, ho.snap);
//   }
// };
}  // namespace fmt
