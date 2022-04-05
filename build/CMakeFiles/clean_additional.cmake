# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/sway_shortcuts_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/sway_shortcuts_autogen.dir/ParseCache.txt"
  "sway_shortcuts_autogen"
  )
endif()
