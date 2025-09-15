cmake_minimum_required(VERSION 3.13)
project(lpc1114_blinky_11cxx C)
set(LPC_CHIP_DIR "${CMAKE_SOURCE_DIR}/lpc_chip_11cxx_lib" CACHE PATH "Path to lpc_chip_11cxx_lib")
if(NOT EXISTS "${LPC_CHIP_DIR}")
  message(FATAL_ERROR "lpc_chip_11cxx_lib not found at: ${LPC_CHIP_DIR}")
endif()
macro(_pick VAR CANDIDATES)
  set(_found "")
  foreach(f ${CANDIDATES})
    if(EXISTS "${LPC_CHIP_DIR}/src/${f}")
      list(APPEND _found "${LPC_CHIP_DIR}/src/${f}")
    endif()
  endforeach()
  if(NOT _found)
    message(FATAL_ERROR "None of these sources were found in ${LPC_CHIP_DIR}/src: ${CANDIDATES}")
  endif()
  # ここを修正: PARENT_SCOPE を削除
  set(${VAR} ${_found})
endmacro()
_pick(CHIP_CORE     "chip_11xx.c")
_pick(CLOCK_SRC     "clock_11xx.c")
_pick(GPIO_SRC      "gpio_11xx_1.c;gpio_11xx_2.c")
_pick(SYSINIT_SRC   "sysinit_11xx.c")
_pick(SYSTEM_SRC    "sysctl_11xx.c")
set(LPC_CHIP_SOURCES ${CHIP_CORE} ${CLOCK_SRC} ${GPIO_SRC} ${SYSINIT_SRC} ${SYSTEM_SRC})
add_executable(${PROJECT_NAME}
  src/startup.c
  ${LPC_CHIP_SOURCES}
  src/main.c
)
target_include_directories(${PROJECT_NAME} PRIVATE ${LPC_CHIP_DIR}/inc src)
target_compile_definitions(${PROJECT_NAME} PRIVATE CORE_M0)
add_compile_options(-UCHIP_LPC1125)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME} ${PROJECT_NAME}.bin
  COMMENT "Generating ${PROJECT_NAME}.bin")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}
  COMMENT "Size:")
