@echo off
setlocal ENABLEDELAYEDEXPANSION

rem DS203 Win32 GCC support by valky.eu
rem USER DEFINED VALUES
rem ===================================================
set CBASE=C:\arm_toolchain\arm-2011.03\bin\
set TARGET=F:\
set TFILE=APP_G251
set APP=1
rem ===================================================
 
Echo DS203 Build tool by valky.eu
Echo Target slot: !APP!
Echo DFU Drive: !TARGET!

call :CheckSpaces "%CD%" %CD%

set CROSS=!CBASE!arm-none-eabi-
set CC=!CROSS!gcc
set OBJCOPY=!CROSS!objcopy
set LD=!CROSS!ld
set AS=!CROSS!as
set STRIP=!CROSS!strip

if not exist !CC!.* (
  echo Compiler not found !
  goto :eof
)

set INTERMEDIATE=Bin

set STM_INC=..\FWLib\inc
set STM_SRC=..\FWLib\src
set STM_ASM=..\FWLib\asm

set SRC_SRC=..\App\src
set SRC_INC=..\App\inc
set SRC_LDS=..\App\lds
set SRC_ASM=..\App\asm

set CFLAGS=-Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fomit-frame-pointer -MD -I !STM_INC! -I !SRC_INC! -I !SRC_SRC!
set AFLAGS=-mcpu=cortex-m3 -mthumb
set LDFLAGS=-nostartfiles -mcpu=cortex-m3 -mthumb -march=armv7 -mfix-cortex-m3-ldrd -msoft-float

set OBJS=Calibrat.o Draw.o Files.o Function.o ^
         Interrupt.o Main.o Menu.o Process.o startup.o ^
         stm32f10x_nvic.o cortexm3_macro.o

set SRCS= !SRC_SRC!\Calibrat.c !SRC_SRC!\Draw.c !SRC_SRC!\Files.c !SRC_SRC!\Function.c !SRC_SRC!\Interrupt.c ^
          !SRC_SRC!\Main.c !SRC_SRC!\Menu.c !SRC_SRC!\Process.c !SRC_SRC!\startup.c ^
          !STM_SRC!\stm32f10x_nvic.c 

if exist *.elf del *.elf
if exist *.hex del *.hex
if exist *.bin del *.bin

echo Compiling...
if not exist !INTERMEDIATE! mkdir !INTERMEDIATE!
cd !INTERMEDIATE!
!CC! !AFLAGS! -c !STM_ASM!\cortexm3_macro.s -o cortexm3_macro.o
!CC! !AFLAGS! -c !SRC_ASM!\bios.S -o bios.o
!CC! !CFLAGS! -c !SRCS!

echo Linking...
!CC! -o !TFILE!_!APP!.elf !LDFLAGS! -T !SRC_LDS!\app!APP!.lds !OBJS!
!OBJCOPY! -O binary !TFILE!_!APP!.elf !TFILE!.bin
!OBJCOPY! -O ihex !TFILE!_!APP!.elf !TFILE!.hex

rem del *.d *.o

if not exist !TFILE!.hex (
  echo Build failed
  goto :eof
) else (
  echo Build succeed !
)

if not exist !TARGET! (
  echo Target device not ready
  goto :eof
)

if exist !TARGET!\*.WPT (
  echo Please start the device in DFU mode
  echo ^(Hold the first button while powering on^)
  goto :eof
)

Echo Downloading...
copy !TFILE!.hex !TARGET!!TFILE!.hex

rem del *.elf *.hex *.bin
Echo Waiting for the device...
:loop
if exist !TARGET!!TFILE!.not (
  echo Failed to download
  goto :eof
)
if exist !TARGET!!TFILE!.rdy (
  echo Download ok
  goto :eof
)
goto loop

:CheckSpaces
if not %1=="%2" (
  echo.
  echo Your current path contains spaces, it can cause some problems...
  pause
)
