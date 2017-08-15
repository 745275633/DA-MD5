#include "DA/md5.hpp"
namespace DA
{
namespace MD5
{
std::array<byte, 16> MD5::digest() const
{
  return _digest;
}

std::string MD5::toString() const
{
  auto input = digest();
  std::string str;
  str.reserve(input.size() << 1);
  for (const auto & t : input) {
    int a = t / 16;
    int b = t % 16;
    str.append(1, _data.hex_number.at(a));
    str.append(1, _data.hex_number.at(b));
  }
  return str;
}

void MD5::reset()
{
  _state = _data.state;
  _count = {0};
  _digest = {0};
  _buffer = {0};
}

} // namespace MD5
} // namespace DA
