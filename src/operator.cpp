#include "DA/md5.hpp"
#include <iostream>

namespace DA
{
namespace MD5
{

std::string MD5::operator()()
{
  return toString();
}
std::string MD5::operator()(const std::string& str)
{
  update(str);
  return toString();
}
std::string MD5::operator()(std::istream& in)
{
  update(in);
  return toString();
}
std::string MD5::operator()(const std::vector<byte>& input)
{
  update(input);
  return toString();
}

std::ostream& operator<<(std::ostream& out, const MD5& md5)
{
  out << md5.toString();
  return out;
}
std::istream& operator>>(std::istream& in, MD5& md5)
{
  md5.update(in);
  return in;
}

} // namespace MD5
} // namespace DA
