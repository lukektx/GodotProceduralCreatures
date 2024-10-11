#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/"], SCONS_CXX_STANDARD="c++11")

sources = (Glob(f"src/*.cpp"))
sources.extend(Glob(f"src/creatures/*.cpp"))
sources.extend(Glob(f"src/creatures/body/*.cpp"))

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demo/bin/libcreatures.{}.{}.framework/libcreatures.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "project/bin/libcreatures{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
