add_executable(SEETAna SEETAna.C)
add_executable(SEETAnaBkgdOnly SEETAnaBkgdOnly.C)
target_link_libraries(SEETAna ${ROOT_LIBRARIES})
target_link_libraries(SEETAnaBkgdOnly ${ROOT_LIBRARIES})