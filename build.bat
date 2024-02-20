platformio run -e STM32F446ZE_btt
cd C:\work\debug\ARM_tools
copy C:\Users\bobku\Documents\GitHub\Marlin-Bob-2\.pio\build\STM32F446ZE_btt\firmware.elf
.\objdump.exe -d -S -l -C -t firmware.elf >C:\work\debug\ARM_tools\ARM_disassemble_with_line_numbers.txt
cd C:\Users\bobku\Documents\GitHub\Marlin-Bob-2