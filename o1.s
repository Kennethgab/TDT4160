.thumb
.syntax unified

.include "gpio_constants.s"     // Register-adresser og konstanter for GPIO

.text
	.global Start
	
	// PE_dout = GPIO_BASE + 9w*port_E +  GPIO_PORT_DOUTSET
	// PE_DIN = GPIO_BASE + 9W*PORT_B + GPIO_PORT_DIN
Start:
	LDR R0, =GPIO_BASE
	LDR R1, =GPIO_PORT_DIN
	LDR R2, =PORT_SIZE				 // 36B, 9w
	LDR R3, =PORT_B
	MUL R4, R3, R2 				// 9w*PORT_B
	ADD R5, R4, R0 				// 9w*PORT_B + GPIO_BASE
	ADD R5, R5, R1   //  GPIO_BASE + 9W*PORT_B + GPIO_PORT_DIN,

	LDR R3, =PORT_E
	LDR R1, =GPIO_PORT_DOUTSET
	MUL R4, R3, R2
	ADD R6, R4, R0
	ADD R7, R6, R1
	LDR R1, =GPIO_PORT_DOUTCLR
	ADD R8, R6, R1

	// R7 = PORT_E_DOUTSET ADDRESS
	// R5 = PORT_B_DIN ADDRESS
	// R8 = PORT_E_DOUTCLR
	MOV R9, 0b100
	B CHECK


PRESSED:
STR R9, [R7]




CHECK:
LDR R1, [R5]
AND R2, R1, 0b1000000000
CMP R2, 0

BEQ PRESSED

STR R9, [R8]
B CHECK








    // Skriv din kode her...


NOP // Behold denne på bunnen av fila

