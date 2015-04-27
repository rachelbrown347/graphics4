# - Config file for the Armadillo package
# It defines the following variables
#  ARMADILLO_INCLUDE_DIRS - include directories for Armadillo
#  ARMADILLO_LIBRARY_DIRS - library directories for Armadillo (normally not used!)
#  ARMADILLO_LIBRARIES    - libraries to link against

# Tell the user project where to find our headers and libraries
set(ARMADILLO_INCLUDE_DIRS "/Users/rachel/Desktop/programming/CS284_Computer-Graphics/assignment_4/armadillo-5.000.1;/Users/rachel/Desktop/programming/CS284_Computer-Graphics/assignment_4/armadillo-5.000.1")
set(ARMADILLO_LIBRARY_DIRS "/Users/rachel/Desktop/programming/CS284_Computer-Graphics/assignment_4/armadillo-5.000.1")

# Our library dependencies (contains definitions for IMPORTED targets)
include("/Users/rachel/Desktop/programming/CS284_Computer-Graphics/assignment_4/armadillo-5.000.1/ArmadilloLibraryDepends.cmake")

# These are IMPORTED targets created by ArmadilloLibraryDepends.cmake
set(ARMADILLO_LIBRARIES armadillo)

