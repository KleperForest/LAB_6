//*********************************************************************
// Universidad del Valle de Guatemala
// IE2023: Programación de Microcontroladores
// Autor: Alan Gomez
// Proyecto: LAB_6.c
// Descripción: Sexto Laboratorio de Programación de Microcontroladores con lenguaje C.
// Hardware: ATmega328p
// Created: 4/18/2024 12:27:37 AM
//*********************************************************************

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void initUART9600(void);
void writeUART(char Caracter);
void writeTextUART(char* Texto);
void setup(void); // Modulo de pines
void decimalToBinary(uint8_t decimal, uint8_t binary[]);

volatile uint8_t bufferRX = 0;

int main(void)
{
	initUART9600();
	sei();
	setup();
	while (1)
	{
		if (bufferRX == 10) // Cuando se recibe un 10
		{
			uint8_t binary[8] = {0}; // Array para almacenar el número binario
			decimalToBinary(bufferRX, binary); // Convertir el valor decimal a binario
			
			// Asignar bits del PB0 al PB5
			PORTB = (binary[0] << PB0) | (binary[1] << PB1) | (binary[2] << PB2) | (binary[3] << PB3) | (binary[4] << PB4) | (binary[5] << PB5);
			// Asignar bits del PD4 al PD7
			PORTD = (binary[6] << PD4) | (binary[7] << PD5) | (binary[8] << PD6) | (binary[9] << PD7);
		}
	}
}

void setup(void)
{
	// Configurar pines PB0 a PB5 como salidas para LEDs.
	DDRB |= 0b00111111;
	// Configurar pines PD4 a PD7 como salidas para LEDs.
	DDRD |= 0b11110000;
}

void initUART9600()
{
	// Paso 1: RX como entrada y TX como salida
	DDRD &= ~(1 << DDD0);
	DDRD |= (1 << DDD1);

	// Paso 2: Configurar UCSR0A
	// Registro A
	UCSR0A = 0;

	// Paso 3: Configurar UCSR0B
	UCSR0B = 0;
	// Habilitar recepción y transmisión
	UCSR0B |= (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);

	// Paso 4: Configurar UCSR0C> ASINCRONO, PARIDAD NO. Bit de stop. 8 caracteres.
	UCSR0C = 0;

	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

	// Paso 5: Configurar Velocidad de Baudrate: 9600
	UBRR0 = 103;
}

// Interrupcion
ISR(USART_RX_vect)
{ // Recepción.

	bufferRX = UDR0;

	while (!(UCSR0A & (1 << UDRE0)))
	; // Enviar de regreso
	UDR0 = bufferRX;
}

void decimalToBinary(uint8_t decimal, uint8_t binary[])
{
	for (int i = 0; i < 8; i++)
	{
		binary[i] = (decimal >> i) & 0x01;
	}
}
