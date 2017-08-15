#include "DA/md5.hpp"

namespace DA
{
namespace MD5
{
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

} // namespace MD5
} // namespace DA
