cmake_minimum_required(VERSION 3.0.2)
project(rc_gazebo)

## Use C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## By adding -Wall and -Werror, the compiler does not ignore warnings anymore,
## enforcing cleaner code.
add_definitions(-Wall -Werror)

## Find catkin macros and libraries
find_package(catkin REQUIRED
        COMPONENTS
        roscpp
        roslint
        rc_common
        gazebo_ros
        gazebo_ros_control
        )

# Depend on system install of Gazebo
find_package(gazebo REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
        INCLUDE_DIRS
        include
        LIBRARIES
        rc_action_hw_sim
        CATKIN_DEPENDS
        roscpp
        rc_common

        gazebo_ros
        gazebo_ros_control
        DEPENDS
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
        include
        ${catkin_INCLUDE_DIRS}
        ${GAZEBO_INCLUDE_DIRS}
)

## Declare a cpp library
add_library(rc_action_hw_sim
        src/rc_action_hw_sim.cpp
        )

add_dependencies(rc_action_hw_sim
        ${catkin_EXPORTED_TARGETS}
        )

target_link_libraries(rc_action_hw_sim
        ${catkin_LIBRARIES}
        ${GAZEBO_LIBRARIES}
        )

#############
## Install ##
#############

## Mark executables and/or libraries for installation
install(TARGETS rc_action_hw_sim
        ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
        )

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
        DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
        FILES_MATCHING PATTERN "*.h"
        )

# Mark other files for installation
install(DIRECTORY config launch worlds
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
        )
install(FILES rc_action_hw_sim_plugins.xml
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
        )
#############
## Testing ##
#############

#if (${CATKIN_ENABLE_TESTING})
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
#  ## Add gtest based cpp test target and link libraries
#  catkin_add_gtest(${PROJECT_NAME}-test
#      test/test_ros_package_template.cpp
#      test/AlgorithmTest.cpp)
#  target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME}_core)
#endif ()
add_executable(acc_test src/acc_test.cpp)

target_link_libraries(acc_test
  ${catkin_LIBRARIES}
  # 其他你需要的系统或第三方库
)

install(TARGETS acc_test
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

add_executable(keyboard_ctrl

  control/keyboard_ctrl.cpp
  
  )
  
add_dependencies(keyboard_ctrl ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
  target_link_libraries(keyboard_ctrl
  ${catkin_LIBRARIES} 
)

add_executable(keyboard_info_plus

  control/keyboard_info_plus.cpp
  
  )
add_dependencies(keyboard_info_plus ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
  target_link_libraries(keyboard_info_plus
  ${catkin_LIBRARIES} 
)

add_executable(sim_action_data

  control/sim_action_data.cpp
  
  )
add_dependencies(sim_action_data ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
  target_link_libraries(sim_action_data
  ${catkin_LIBRARIES} 
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(NCURSES REQUIRED ncurses)

include_directories(${NCURSES_INCLUDE_DIRS})
target_link_libraries(sim_action_data ${NCURSES_LIBRARIES})

find_package(Curses REQUIRED)
target_link_libraries(keyboard_info_plus ${CURSES_LIBRARIES})


##########################
## Static code analysis ##
##########################

roslint_cpp()
