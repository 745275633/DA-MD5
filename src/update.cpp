#include "DA/md5.hpp"
#include <algorithm>
#include <iterator>
#include <sstream>

namespace DA
{
namespace MD5
{
void MD5::up(const std::vector<byte>& input)
{
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
}

void MD5::update(const std::string& str)
{
  update(std::vector<byte>{str.begin(), str.end()});
}

void MD5::update(std::istream& in)
{
  if (!in) {
    return;
  }

  std::streamsize length;
  char buffer[BUFFER_SIZE];
  std::stringstream ss;
  while (!in.eof()) {
    in.read(buffer, BUFFER_SIZE);
    length = in.gcount();
    if (length > 0) ss << std::string(buffer, length);
  }
  update(ss.str());
}

void MD5::update(const std::vector<byte>& input)
{
  reset();
  up(input);
  end();
}

} // namespace MD5
} // namespace DA
