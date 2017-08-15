#include "DA/md5.hpp"
#include <algorithm>
#include <iterator>
#include <sstream>

namespace DA
{
namespace MD5
{
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

#define F _data.f
#define G _data.g
#define H _data.h
#define I _data.i

#define ROTATE_LEFT _data.rotate_left

#define FF(a, b, c, d, x, s, ac) { \
  (a) += F ((b), (c), (d)) + (x) + (ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
  (a) += G ((b), (c), (d)) + (x) + (ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
  (a) += H ((b), (c), (d)) + (x) + (ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
  (a) += I ((b), (c), (d)) + (x) + (ac); \
  (a) = ROTATE_LEFT ((a), (s)); \
  (a) += (b); \
}

MD5::MD5():
  _count{0},
  _state(_data.state),
  _buffer{0},
  _digest{0}
{}

MD5::MD5(data a):
  _data(std::move(a)),
  _count{0},
  _state(a.state),
  _buffer{0},
  _digest{0}
{}

std::array<byte, 16> MD5::digest() const
{
  return _digest;
}

/* Reset the calculate state */
void MD5::reset()
{
  _state = _data.state;
  _count = {0};
  _digest = {0};
  _buffer = {0};
}

void MD5::update(const std::string& str)
{
  update(std::vector<byte>{str.begin(), str.end()});
}

/* Updating the context with a file. */
void MD5::update(std::istream& in)
{
  if (!in) {
    return;
  }

  std::streamsize length;
  char buffer[BUFFER_SIZE];
  while (!in.eof()) {
    in.read(buffer, BUFFER_SIZE);
    length = in.gcount();
    if (length > 0) update(std::string(buffer, length));
  }
}

/* MD5 block update operation. Continues an MD5 message-digest
operation, processing another message block, and updating the
context.
*/
void MD5::update(const std::vector<byte>& input)
{
  reset();

  uint32 index = (_count[0] >> 3) & 0x3f;

  if ((_count[0] += (input.size() << 3)) < (input.size() << 3)) ++_count[1];
  _count[1] += input.size() >> 29;

  uint32 i = 64 - index;

  if (input.size() >= i) {
    std::copy(input.begin(), input.end(),
        std::next(_buffer.begin(), index));
    transform(_buffer);

    // TODO(clang-tidy): warning: [misc-misplaced-widening-cast]
    for (; (i + 63) < input.size(); i += 64) {
      std::array<byte, 64> tmp = {0};
      std::copy_n(std::next(input.begin(), i), 64,
          tmp.begin());
      transform(tmp);
    }
    index = 0;
  } else {
    i = 0;
  }
  std::copy_n(std::next(input.begin(), i), input.size() - i,
      std::next(_buffer.begin(), index));

  end();
}

void MD5::end()
{
  std::array<uint32, 4> oldState(_state);
  std::array<uint32, 2> oldCount(_count);
  uint32 index, padLen;

  auto bits = encode({_count.begin(), _count.end()});

  index = uint32((_count[0] >> 3) & 0x3f);
  padLen = (index < 56) ? (56 - index) : (120 - index);
  update(std::vector<byte>{_data.padding.begin(), std::next(_data.padding.begin(), padLen)});

  update(bits);

  std::copy_n(encode({_state.begin(), _state.end()}).begin(), _digest.size(),
      _digest.begin());

  _state = oldState;
  _count = oldCount;
}

/* MD5 basic transformation. Transforms _state based on block. */
void MD5::transform(const std::array<byte, 64>& block)
{
  uint32 a = _state[0], b = _state[1], c = _state[2], d = _state[3];

  auto x = decode({block.begin(), block.end()});

  /* Round 1 */
  FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
  FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
  FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
  FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
  FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
  FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
  FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
  FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
  FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
  FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
  FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
  FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
  FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
  FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
  FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
  FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

  /* Round 2 */
  GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
  GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
  GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
  GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
  GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
  GG (d, a, b, c, x[10], S22, 0x02441453); /* 22 */
  GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
  GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
  GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
  GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
  GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
  GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
  GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
  GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
  GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
  GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

  /* Round 3 */
  HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
  HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
  HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
  HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
  HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
  HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
  HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
  HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
  HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
  HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
  HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
  HH (b, c, d, a, x[ 6], S34, 0x04881d05); /* 44 */
  HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
  HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
  HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
  HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

  /* Round 4 */
  II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
  II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
  II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
  II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
  II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
  II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
  II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
  II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
  II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
  II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
  II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
  II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
  II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
  II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
  II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
  II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

  _state[0] += a;
  _state[1] += b;
  _state[2] += c;
  _state[3] += d;
}

std::vector<byte> MD5::encode(const std::vector<uint32>& input)
{
  std::vector<byte> output;
  for (const auto & i : input) {
    output.push_back(i & 0xff);
    output.push_back((i >> 8) & 0xff);
    output.push_back((i >> 16) & 0xff);
    output.push_back((i >> 24) & 0xff);
  }
  return output;
}

std::vector<uint32> MD5::decode(const std::vector<byte>& input)
{
  std::vector<uint32> output;
  for (decltype(input.size()) j = 0; j < input.size(); j += 4) {
    output.push_back(uint32(input[j]) | (uint32(input[j + 1]) << 8) |
        (uint32(input[j + 2]) << 16) | (uint32(input[j + 3]) << 24));
  }
  return output;
}

/* Convert digest to string value */
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

} // namespace MD5
} // namespace DA
