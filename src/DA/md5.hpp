#ifndef DA_MD5_HPP
#define DA_MD5_HPP

#include <array>
#include <boost/cstdint.hpp>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace DA
{
namespace MD5
{
using byte = boost::uint8_t;
using uint32 = boost::uint32_t;

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
class MD5
{
private:
  data _data;
  std::array<uint32, 4> _state;
  std::array<uint32, 2> _count;
  std::array<byte, 64> _buffer;
  std::array<byte, 16> _digest;

  const static int BUFFER_SIZE = 1024;
  void end();
  void transform(const std::array<byte, 64>& block);

  void up(const std::vector<byte>& input);

  static std::vector<byte> encode(const std::vector<uint32>& input);
  static std::vector<uint32> decode(const std::vector<byte>& input);

public:
  MD5();
  explicit MD5(data a);
  ~MD5() = default;
  MD5(const MD5&) = default;
  MD5& operator=(const MD5&) = default;
  MD5(MD5&&) noexcept(false) /*For clang-tidy*/ = default;
  MD5& operator=(MD5&&) noexcept(false) = default;
  void reset();

  void update(const std::string& str);
  void update(std::istream& in);
  void update(const std::vector<byte>& input);

  std::array<byte, 16> digest() const;
  std::string toString() const;

  std::string operator()();
  std::string operator()(const std::string& str);
  std::string operator()(std::istream& in);
  std::string operator()(const std::vector<byte>& input);
};

std::ostream& operator<<(std::ostream& out, const MD5& md5);
std::istream& operator>>(std::istream& in, MD5& md5);

} // namespace MD5
} // namespace DA

#endif /* DA_MD5_HPP */
