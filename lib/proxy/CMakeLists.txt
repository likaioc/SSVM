# SPDX-License-Identifier: Apache-2.0
add_library(ssvmProxy
  cmdparser.cpp
  proxy.cpp
)

target_link_libraries(ssvmProxy
  PRIVATE
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ssvmExpVM
)
