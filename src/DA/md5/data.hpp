#ifndef DA_MD5_DATA_HPP
#define DA_MD5_DATA_HPP

#include <array>
#include <functional>

namespace DA
{
namespace MD5
{
struct data
{
  std::array<byte, 64> padding{{0x80}};
  std::array<char, 16> hex_number
  {{
     '0', '1', '2', '3',
     '4', '5', '6', '7',
     '8', '9', 'a', 'b',
     'c', 'd', 'e', 'f'
   }};
  std::array<uint32, 4> state
  {{0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476}};

  std::function<uint32(uint32, uint32, uint32)> f =
    [] (uint32 x, uint32 y, uint32 z) -> uint32
    {
      return (x & y) | (~x & z);
    };
  std::function<uint32(uint32, uint32, uint32)> g =
    [] (uint32 x, uint32 y, uint32 z) -> uint32
    {
      return (x & z) | (y & ~z);
    };
  std::function<uint32(uint32, uint32, uint32)> h =
    [] (uint32 x, uint32 y, uint32 z) -> uint32
    {
      return x ^ y ^ z;
    };
  std::function<uint32(uint32, uint32, uint32)> i =
    [] (uint32 x, uint32 y, uint32 z) -> uint32
    {
      return y ^ (x | ~z);
    };

  std::function<uint32(uint32, uint32)> rotate_left =
    [] (uint32 x, uint32 n) -> uint32
    {
      return (x << n) | (x >> (32 - n));
    };
  // std::array<std::array<uint32, 4>, 4> s = {{
    // {{ 7, 12, 17, 22}},
    // {{ 5,  9, 14, 20}},
    // {{ 4, 11, 16, 23}},
    // {{ 6, 10, 15, 21}}
  // }};
  // std::array<std::array<uint32, 4>, 4> s = {{
    // {{ 7,  5,  4,  6}},
    // {{12,  9, 11, 10}},
    // {{17, 14, 16, 15}},
    // {{22, 20, 23, 21}}
  // }};
};

} // namespace MD5
} // namespace DA

#endif // DA_MD5_DATA_HPP
