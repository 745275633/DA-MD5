#ifndef DA_MD5_MD5_HPP
#define DA_MD5_MD5_HPP

#include <array>
#include <iostream>
#include <string>
#include <vector>

namespace DA
{
namespace MD5
{
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

#endif // DA_MD5_MD5_HPP
