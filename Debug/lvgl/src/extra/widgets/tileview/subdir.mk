################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/src/extra/widgets/tileview/lv_tileview.c 

OBJS += \
./lvgl/src/extra/widgets/tileview/lv_tileview.o 

C_DEPS += \
./lvgl/src/extra/widgets/tileview/lv_tileview.d 


# Each subdirectory must supply rules for building sources it contributes
lvgl/src/extra/widgets/tileview/%.o lvgl/src/extra/widgets/tileview/%.su: ../lvgl/src/extra/widgets/tileview/%.c lvgl/src/extra/widgets/tileview/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DLV_LVGL_H_INCLUDE_SIMPLE -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../lvgl -I"/home/juraszekl/STMicroelectronics/STM32_Workspace/08_OLED_SH1106/lvgl/src/core" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lvgl-2f-src-2f-extra-2f-widgets-2f-tileview

clean-lvgl-2f-src-2f-extra-2f-widgets-2f-tileview:
	-$(RM) ./lvgl/src/extra/widgets/tileview/lv_tileview.d ./lvgl/src/extra/widgets/tileview/lv_tileview.o ./lvgl/src/extra/widgets/tileview/lv_tileview.su

.PHONY: clean-lvgl-2f-src-2f-extra-2f-widgets-2f-tileview

