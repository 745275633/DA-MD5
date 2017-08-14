set_xmakever("2.1.3")
set_languages("gnu++14")

target("DA-MD5")
  set_kind("shared")
  add_files("src/*.cpp") 
  add_headers("src/(DA/*.hpp)")
  before_build(function (target)
    if not os.exists("/usr/include/boost") then
      if io.stderr == nil then
        io.stderr = io.open("/dev/stderr", "a")
      end
      io.stderr:print("Need boost");
      io.stderr:print("Install it with package manager")
      io.stderr:print("Or http://www.boost.org/")
      os.exit(1);
    end
  end)
