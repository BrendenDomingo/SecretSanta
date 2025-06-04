# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\secret_santa_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\secret_santa_autogen.dir\\ParseCache.txt"
  "secret_santa_autogen"
  )
endif()
