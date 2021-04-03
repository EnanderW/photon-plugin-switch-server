cmake_minimum_required(VERSION 3.17)
project(Photon_ServerSwitchPlugin C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(<path_to>/Photon/src)

add_library(Photon_ServerSwitchPlugin SHARED serverswitch.c)
if (WIN32)
    target_link_libraries(Photon_ServerSwitchPlugin <path_to>/PhotonAPI.dll uv)
else()
    target_link_libraries(Photon_ServerSwitchPlugin <path_to>/PhotonAPI.so uv)
endif()

set_target_properties(Photon_ServerSwitchPlugin PROPERTIES PREFIX "")
set_target_properties(Photon_ServerSwitchPlugin PROPERTIES OUTPUT_NAME "serverswitch")