#THIS FILE IS AUTO GENERATED FROM THE TEMPLATE! DO NOT CHANGE!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.19)

# specify cross compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(HelloWord-Dynamic-fw C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# for use printf & scanf with float
set(COMMON_FLAGS "-specs=nosys.specs -specs=nano.specs -u _printf_float -u _scanf_float")

#Uncomment for hardware floating point
add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

#Uncomment for software floating point
#add_compile_options(-mfloat-abi=soft)

add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
    add_definitions(-D_D_DEBUG_)
endif ()

# 设置编译时间戳
string(TIMESTAMP COMPILE_TIME %Y%m%d)
set(build_time    ${COMPILE_TIME})
message(STATUS "build time: ${build_time}")
# 设置编译版本
string(TIMESTAMP COMPILE_TIME %Y.%m.%d)
set(build_version    "v${COMPILE_TIME}")
message(STATUS "build time: ${build_version}")
set(build_version_parse_tag    "build_version_parse_tag:{{${build_version}}}:build_version_parse_tag")

find_package(Git REQUIRED)
# 设置GIT版本信息
execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
        OUTPUT_VARIABLE GIT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
# 设置GIT分支信息
execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
# 设置GIT HASH
execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        OUTPUT_VARIABLE GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
configure_file("${CMAKE_SOURCE_DIR}/UserApp/build_info.h.in" "${CMAKE_SOURCE_DIR}/UserApp/build_info.h")

include_directories(
        ./
        Core/Inc
        Drivers/STM32F4xx_HAL_Driver/Inc
        Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
        Drivers/CMSIS/Device/ST/STM32F4xx/Include
        Drivers/CMSIS/Include
        Middlewares/Third_Party/FreeRTOS/Source/include
        Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
        Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
        Middlewares/ST/STM32_USB_Device_Library/Core/Inc
        #Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
        Middlewares/ST/STM32_USB_Device_Library/Class/CustomHID/Inc
        USB_DEVICE/App
        USB_DEVICE/Target
        BSP/fibre/cpp/include
        BSP/u8g2
        BSP/u8g2/cpp
        BSP/uvector
        BSP/eink
        Platform/Utils
        UserApp
        UserApp/protocols
)

add_definitions(-DUSE_HAL_DRIVER -DSTM32F4 -DSTM32F4xx -DSTM32F405xx -DconfigAPPLICATION_ALLOCATED_HEAP)

file(GLOB_RECURSE SOURCES
        "startup/*.*"
        "Drivers/*.*"
        "Core/*.*"
        "BSP/*.*"
        "Middlewares/*.*"
        "USB_DEVICE/*.*"
        "Ctrl/*.*"
        "Platform/*.*"
        "UserApp/*.*"
        )

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F405RGTx_FLASH.ld)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

# 原装屏
option(Eink290BW "Enable feature Eink290BW" OFF)

if(Eink290BW)
    add_definitions(-D_Eink290BW_)
endif()

# 咸鱼屏
option(WF0290CZ0LP "Enable feature WF0290CZ0LP" OFF)

if(WF0290CZ0LP)
    add_definitions(-D_WF0290CZ0LP_)
endif()

link_directories("Drivers/CMSIS/Lib")
link_libraries("arm_cortexM4lf_math.a")

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

target_compile_options(${PROJECT_NAME}.elf PRIVATE -O0)

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

set(SUFFIX "")
if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    set(SUFFIX ".exe")
endif ()

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMAND g++ -O3 -std=c++14 -o ${PROJECT_BINARY_DIR}/uf2conv${SUFFIX} ${CMAKE_HOME_DIRECTORY}/scripts/uf2conv.cpp
        COMMAND ${PROJECT_BINARY_DIR}/uf2conv${SUFFIX} ${BIN_FILE} ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.uf2
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")

