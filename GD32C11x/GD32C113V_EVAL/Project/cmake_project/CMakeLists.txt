# =============================================================================
# CMakeLists.txt — 由 mcudeva 自动生成
#
#   [mcudeva:auto] 区：自动生成，重新生成 / 同步 .project.json 时会被整体覆盖，
#                      请勿直接修改（改动会丢失）。
#   [mcudeva:user] 区：用户自定义区，重新生成时原样保留，
#                      请把所有手写的 CMake 语句写在这些区域内。
#
# =============================================================================

# ==== [mcudeva:auto] BEGIN project (hash:444d3903) ====
cmake_minimum_required(VERSION 3.20)

project(Project C ASM)

set(LINKER_SCRIPT gd32c11x_flash.ld)
# ==== [mcudeva:auto] END project ====

# ---- [mcudeva:user] BEGIN prologue — 用户自定义区，重新生成时保留，可自由修改 ----
# 可在此设置前置变量 / option()，作用于后续所有自动区
# ---- [mcudeva:user] END prologue ----

# ==== [mcudeva:auto] BEGIN flags (hash:924ea126) ====
# 编译参数
set(MCU_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")

set(CMAKE_C_FLAGS
    "${MCU_FLAGS} -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -std=gnu11"
    CACHE STRING "" FORCE)

set(CMAKE_ASM_FLAGS "${MCU_FLAGS} -x assembler-with-cpp" CACHE STRING "" FORCE)

# 各配置编译标志（按 CMAKE_BUILD_TYPE 自动叠加）
set(CMAKE_C_FLAGS_DEBUG "-O0 -g3" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELEASE "-O0 -g" CACHE STRING "" FORCE)

set(CMAKE_EXE_LINKER_FLAGS
    "${MCU_FLAGS} \
     -T${LINKER_SCRIPT} \
     -Wl,--print-memory-usage \
     -Xlinker --gc-sections \
     -Wl,-Map=${PROJECT_NAME}.map \
     --specs=nano.specs \
     --specs=nosys.specs \
     -L${CMAKE_SOURCE_DIR}/ldscripts \
     -Wl,--no-warn-rwx-segments"
    CACHE STRING "" FORCE)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# ==== [mcudeva:auto] END flags ====

# ---- [mcudeva:user] BEGIN flags — 用户自定义区，重新生成时保留，可自由修改 ----
# 可在此追加编译/链接选项，例如 set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
# ---- [mcudeva:user] END flags ----

# ==== [mcudeva:auto] BEGIN definitions (hash:53719dad) ====
# 预处理器宏定义
add_compile_definitions(
    GD32C11X
    GD_ECLIPSE_GCC
    USE_STDPERIPH_DRIVER
)
# ==== [mcudeva:auto] END definitions ====

# ---- [mcudeva:user] BEGIN definitions — 用户自定义区，重新生成时保留，可自由修改 ----
# 可在此追加宏定义，例如 add_compile_definitions(MY_FLAG=1)
# ---- [mcudeva:user] END definitions ----

# ==== [mcudeva:auto] BEGIN includes (hash:4f718a79) ====
# 头文件路径
set(INC_DIRS
    ${CMAKE_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/CMSIS
    ${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/CMSIS/GD/GD32C11x/Include
    ${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Include
    ${CMAKE_SOURCE_DIR}/../../Utilities
    ${CMAKE_SOURCE_DIR}/../../Utilities/LCD_Commom
    ${CMAKE_SOURCE_DIR}/../../../../Middleware/Third_Party/FreeRTOS-Kernel-11.2.0/include
    ${CMAKE_SOURCE_DIR}/../../../../Middleware/Third_Party/FreeRTOS-Kernel-11.2.0/portable/GCC/ARM_CM4F
)
# ==== [mcudeva:auto] END includes ====

# ---- [mcudeva:user] BEGIN includes — 用户自定义区，重新生成时保留，可自由修改 ----
# 可在此追加头文件路径，例如 list(APPEND INC_DIRS ${CMAKE_SOURCE_DIR}/my_inc)
# ---- [mcudeva:user] END includes ----

# ==== [mcudeva:auto] BEGIN sources (hash:4ef6ce73) ====
# 源文件列表
set(SOURCES)
list(APPEND SOURCES
    "${CMAKE_SOURCE_DIR}/../main.c"
    "${CMAKE_SOURCE_DIR}/../gd32c11x_it.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/CMSIS/GD/GD32C11x/Source/system_gd32c11x.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_adc.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_bkp.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_can.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_crc.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_ctc.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_dac.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_dbg.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_dma.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_exmc.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_exti.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_fmc.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_fwdgt.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_gpio.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_i2c.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_misc.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_pmu.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_rcu.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_rtc.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_spi.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_timer.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_usart.c"
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/GD32C11x_standard_peripheral/Source/gd32c11x_wwdgt.c"
    "${CMAKE_SOURCE_DIR}/../../Utilities/gd32c113v_eval.c"
    "${CMAKE_SOURCE_DIR}/../../Utilities/gd32c113v_lcd_eval.c"
    "${CMAKE_SOURCE_DIR}/../../Utilities/LCD_Commom/lcd_font.c"
    "${CMAKE_SOURCE_DIR}/../../../../Middleware/Third_Party/FreeRTOS-Kernel-11.2.0/croutine.c"
    "${CMAKE_SOURCE_DIR}/../../../../Middleware/Third_Party/FreeRTOS-Kernel-11.2.0/event_groups.c"
    "${CMAKE_SOURCE_DIR}/../../../../Middleware/Third_Party/FreeRTOS-Kernel-11.2.0/list.c"
    "${CMAKE_SOURCE_DIR}/../../../../Middleware/Third_Party/FreeRTOS-Kernel-11.2.0/queue.c"
    "${CMAKE_SOURCE_DIR}/../../../../Middleware/Third_Party/FreeRTOS-Kernel-11.2.0/stream_buffer.c"
    "${CMAKE_SOURCE_DIR}/../../../../Middleware/Third_Party/FreeRTOS-Kernel-11.2.0/tasks.c"
    "${CMAKE_SOURCE_DIR}/../../../../Middleware/Third_Party/FreeRTOS-Kernel-11.2.0/timers.c"
    "${CMAKE_SOURCE_DIR}/../../../../Middleware/Third_Party/FreeRTOS-Kernel-11.2.0/portable/MemMang/heap_4.c"
    "${CMAKE_SOURCE_DIR}/../../../../Middleware/Third_Party/FreeRTOS-Kernel-11.2.0/portable/GCC/ARM_CM4F/port.c"
    "${CMAKE_SOURCE_DIR}/gcc_startup/startup_gd32c11x.S"
)

# ==== [mcudeva:auto] END sources ====

# ---- [mcudeva:user] BEGIN sources — 用户自定义区，重新生成时保留，可自由修改 ----
# 可在此增删源文件，例如 list(APPEND SOURCES ${CMAKE_SOURCE_DIR}/my/extra.c)
list(APPEND SOURCES
    "${CMAKE_SOURCE_DIR}/../../../GD32C11x_Firmware_Library/CMSIS/GD/GD32C11x/Source/GCC/newlib/syscalls.c"
)
# ---- [mcudeva:user] END sources ----

# ==== [mcudeva:auto] BEGIN target (hash:57a9b457) ====
set(TARGET_ELF ${PROJECT_NAME}.elf)

add_executable(${TARGET_ELF}
    ${SOURCES}
)

target_include_directories(${TARGET_ELF} PRIVATE ${INC_DIRS})

# ==== [mcudeva:auto] END target ====

# ---- [mcudeva:user] BEGIN target — 用户自定义区，重新生成时保留，可自由修改 ----
# 可在此写 target_* 命令，例如 target_link_libraries(${TARGET_ELF} PRIVATE m)
# ---- [mcudeva:user] END target ----

# ==== [mcudeva:auto] BEGIN post-build (hash:7d6fc976) ====
# 构建后处理
set(TARGET_HEX ${PROJECT_NAME}.hex)
set(TARGET_BIN ${PROJECT_NAME}.bin)
set(TARGET_LST ${PROJECT_NAME}.lst)

add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:${TARGET_ELF}> ${TARGET_HEX}
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET_ELF}> ${TARGET_BIN}
    COMMAND ${CMAKE_OBJDUMP} --source --all-headers --demangle --line-numbers --wide
            $<TARGET_FILE:${TARGET_ELF}> > ${TARGET_LST}
    COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${TARGET_ELF}>
    COMMENT "Generating ${TARGET_HEX}, ${TARGET_BIN}, ${TARGET_LST}"
    BYPRODUCTS ${TARGET_HEX} ${TARGET_BIN} ${TARGET_LST}
)
# ==== [mcudeva:auto] END post-build ====

# ---- [mcudeva:user] BEGIN epilogue — 用户自定义区，重新生成时保留，可自由修改 ----
# 可在此追加自定义 target / include(user.cmake) 等收尾语句
# ---- [mcudeva:user] END epilogue ----
