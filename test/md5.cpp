#include <DA/md5.hpp>
#include <cassert>

int main(int argc, const char *argv[])
{
  using DA::MD5::uint32;
  DA::MD5::MD5 md5;
  md5("test");
  auto tmp = md5.digest();
  std::array<uint32, 16> u32a = {0};
  for (int i = 0; i < 16; ++i) {
    switch (i % 4) {
      case 0:
        u32a[i] = tmp[i] << 24;
        break;
      case 1:
        u32a[i] = tmp[i] << 16;
        break;
      case 2:
        u32a[i] = tmp[i] << 8;
        break;
      default:
        u32a[i] = tmp[i];
        break;
    }
  }
  DA::MD5::data d;
  d.state = {
    u32a[0] + u32a[1] + u32a[2] + u32a[3],
    u32a[4] + u32a[5] + u32a[6] + u32a[7],
    u32a[8] + u32a[9] + u32a[10] + u32a[11],
    u32a[12] + u32a[13] + u32a[14] + u32a[15],
  };
  DA::MD5::MD5 test(d);
  assert(test("test") == "29ebc768f00673e3a3be0d4652ad353f");
  assert(test("TEST") == "4fbfc1f48386a440e9f7bf8c1717c21a");
}
