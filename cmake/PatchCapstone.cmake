# Patches capstone's CMakeLists.txt for compatibility with CMake >= 4.0:
#   - Bumps cmake_minimum_required to 3.10 (CMake >= 4.0 dropped < 3.5 support; < 3.10 warns)
#   - Removes cmake_policy(SET CMP0048 OLD) (rejected by CMake >= 3.27)
file(READ "${DIR}/CMakeLists.txt" _content)
string(REGEX REPLACE
    "cmake_minimum_required[ \t]*\\([ \t]*VERSION[ \t]+[0-9]+\\.[0-9]+(\\.[0-9]+)?[ \t]*\\)"
    "cmake_minimum_required(VERSION 3.10)"
    _content "${_content}")
string(REGEX REPLACE
    "cmake_policy[ \t]*\\([ \t]*SET[ \t]+CMP0048[ \t]+OLD[ \t]*\\)"
    "# cmake_policy(SET CMP0048 OLD)"
    _content "${_content}")
file(WRITE "${DIR}/CMakeLists.txt" "${_content}")
