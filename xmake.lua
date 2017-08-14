set_xmakever("2.1.3")
set_languages("gnu++14")

target("DA-MD5")
  set_kind("shared")
  add_files("src/*.cpp") 
  add_headers("src/(DA/*.hpp)")
  before_build(function (target)
    assert(os.isdir("/usr/include/boost"),
    "Need boost\nInstall it with package manager\nOr visit http://www.boost.org/")
  end)
