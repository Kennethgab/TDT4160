.thumb
.syntax unified

.include "gpio_constants.s"     // Register-adresser og konstanter for GPIO
.include "sys-tick_constants.s" // Register-adresser og konstanter for SysTick

.text
	.global Start
	
Start:

    // Skriv din kode her...

	LDR R0, =GPIO_BASE
	LDR R2, =PORT_SIZE
	LDR R3, =PORT_E
	LDR R1, =GPIO_PORT_DOUTSET
	MUL R4, R3, R2
	ADD R6, R4, R0
	ADD R0, R6, R1        // R0 = DOUTSET Register
	LDR R1, =GPIO_PORT_DOUTCLR
	ADD R1, R6, R1		  // R1 = DOUTCLR Register



    LDR R2, =SYSTICK_BASE // R2 = Control and Status register
    ADD R3, R2, 4 		  // R3 = Reload Value Register
    ADD R4, R2, 8		 // R4 = Current Value Register
	LDR R10, =1400000
	STR R10, [R3]   // R3 Not used anymore
	STR R10, [R4]   // R4 not used anymore


	LDR R5, =tenths //  R5 = tenths register
	LDR R6, =seconds // R6 = seconds register
	LDR R7, = minutes // R7 = minutes register

	MOV R3, R0
	SUB R3, R3, 4 // R3 = DOUT REGISTER

	MOV R9, 0b1111
	LSL R9, R9, 4
	MVN R9, R9

	LDR R10, =GPIO_BASE
	ADD R10, R10, 260  // EXTIPSELH ADDRESS

	LDR R12, [R10]
	AND R12, R12, R9
	MOV R9, 0b0001
	LSL R9, R9, 4
	ORR R12, R12, R9
	STR R12, [R10]  // PORT_B ON pin 9 now port B now set on interrupt

	LDR R10, =GPIO_BASE
	ADD R10, R10, 268  // EXTIFALL ADDRESS

	LDR R12, [R10]
	MOV R9, 0b1000000000
	ORR R12, R12, R9
	STR R12, [R10] // falling edge now set on pin 9

	LDR R10, =GPIO_BASE
	ADD R10, R10, 272  //  ADDRESS

	LDR R12, [R10]
	MOV R9, 0b1000000000
	ORR R12, R12, R9
	STR R12, [R10] // enabled interrupt on pin 9

	LDR R4, =GPIO_BASE
	ADD R4, R4, 284   // Address for interrupt flag clear SET



	MOV R9, 0
	MOV R12, 0
	MOV R10, 0b100
	MOV R8, 0


    LOOP:

    B LOOP

    .global GPIO_ODD_IRQHandler
    .thumb_func
    GPIO_ODD_IRQHandler:

    LDR R12, [R2]
    CMP R12, 0b000
    BEQ TURNON


	MOV R12, 0b000
    STR R12, [R2]
    B IFCLEAR

    TURNON:
    MOV R12, 0b111
    STR R12, [R2]
    B IFCLEAR

	IFCLEAR:
	MOV R12, 0b1000000000
	STR R12, [R4]

    BX LR


    .global SysTick_Handler
    .thumb_func
    SysTick_Handler:

    ADD R9, R9, 1

    CMP R9, 10
    BEQ SECONDS


    STR R9, [R5]


    BX LR


    SECONDS:

	LDR R11, [R3]
	CMP R11, 0b000
	BEQ LEDON

	MOV R10, 0b100
	STR R10, [R1]


	B OVER
	LEDON:

	MOV R10, 0b100
	STR R10, [R0]

	B OVER

	OVER:
    MOV R9, 0
    STR R9, [R5]




	LDR R12, [R6]
   	ADD R12, R12, 1
   	CMP R12, 60
   	BEQ MINUTES

    STR R12, [R6]

    BX LR

    MINUTES:
    MOV R12, 0
    STR R12, [R6]

	LDR R8, [R7]
    ADD R8, R8, 1
    STR R8, [R7]
    BX LR





NOP // Behold denne på bunnen av fila

