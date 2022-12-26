################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/src/extra/libs/ffmpeg/lv_ffmpeg.c 

OBJS += \
./lvgl/src/extra/libs/ffmpeg/lv_ffmpeg.o 

C_DEPS += \
./lvgl/src/extra/libs/ffmpeg/lv_ffmpeg.d 


# Each subdirectory must supply rules for building sources it contributes
lvgl/src/extra/libs/ffmpeg/%.o lvgl/src/extra/libs/ffmpeg/%.su: ../lvgl/src/extra/libs/ffmpeg/%.c lvgl/src/extra/libs/ffmpeg/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DLV_LVGL_H_INCLUDE_SIMPLE -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../lvgl -I"/home/juraszekl/STMicroelectronics/STM32_Workspace/08_OLED_SH1106/lvgl/src/core" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lvgl-2f-src-2f-extra-2f-libs-2f-ffmpeg

clean-lvgl-2f-src-2f-extra-2f-libs-2f-ffmpeg:
	-$(RM) ./lvgl/src/extra/libs/ffmpeg/lv_ffmpeg.d ./lvgl/src/extra/libs/ffmpeg/lv_ffmpeg.o ./lvgl/src/extra/libs/ffmpeg/lv_ffmpeg.su

.PHONY: clean-lvgl-2f-src-2f-extra-2f-libs-2f-ffmpeg

