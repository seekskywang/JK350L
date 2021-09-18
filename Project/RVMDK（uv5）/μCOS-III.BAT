SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\Program Files\Common Files\Oracle\Java\javapath;C:\Program Files\dotnet\;C:\Program Files\IVI Foundation\VISA\WinNT\Bin\;C:\Program Files\IVI Foundation\VISA\WinNT\Bin;C:\Program Files\IDM Computer Solutions\UltraEdit\
SET CPU_TYPE=STM32F767IGTx
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=¦ÌCOS-III
SET CPU_CLOCK=0x017D7840
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\..\output\system_stm32f7xx.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via "..\..\output\startup_stm32f767xx._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\..\output\stm32f7xx_hal.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\..\output\stm32f7xx_hal_cortex.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\..\output\stm32f7xx_hal_gpio.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via "..\..\Output\¦ÌCOS-III.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" "..\..\Output\¦ÌCOS-III.axf" --i32combined --output "..\..\Output\¦ÌCOS-III.hex"
C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe  --bin -o  ..\..\Output\updata.bin ..\..\Output\updata.axf
