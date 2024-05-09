# Install script for directory: /home/berlad/.platformio/packages/framework-zephyr

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/home/berlad/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/zephyr/arch/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/zephyr/lib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/zephyr/soc/arm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/zephyr/boards/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/zephyr/subsys/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/zephyr/drivers/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/_bare_module/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/CANopenNode/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/civetweb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/cmsis/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/fatfs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/hal_nordic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/st/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/libmetal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/littlefs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/loramac-node/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/lvgl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/lz4/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/mbedtls/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/mcuboot/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/mcumgr/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/mipi-sys-t/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/nanopb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/open-amp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/openthread/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/segger/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/sof/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/tflite-micro/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/tinycbor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/tinycrypt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/modules/TraceRecorder/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/zephyr/kernel/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/zephyr/cmake/flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/zephyr/cmake/usage/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/berlad/Uni/6.Semester/IoT/IoT/Lab_1/Single_Hop/SingleHopper/.pio/build/nrf52840_dk/zephyr/cmake/reports/cmake_install.cmake")
endif()

