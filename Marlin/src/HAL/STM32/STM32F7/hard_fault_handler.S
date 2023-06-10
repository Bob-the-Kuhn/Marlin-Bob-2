/* .section  .text.Reset_Handler */
.section  .text.HardFault_Handler
.global  HardFault_Handler
.type  HardFault_Handler, %function
HardFault_Handler:
  movs r0,#4
  movs r1, lr
  tst r0, r1
  beq _MSP
  mrs r0, psp
  b _HALT
_MSP:
  mrs r0, msp
_HALT:
  ldr r1,[r0,#20]
  b hard_fault_handler_c
  bkpt #0

.size  HardFault_Handler, .-HardFault_Handler