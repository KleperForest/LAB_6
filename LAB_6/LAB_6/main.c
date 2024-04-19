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
#include <avr/interrupt.h>

// Declaración de funciones
void initUART9600(void);
void setup(void);

// Variable global para almacenar el último byte recibido
volatile uint8_t bufferRX = 0;

int main(void)
{
	// Inicialización de la UART y configuración de pines
	initUART9600();
	setup();

	// Habilitar interrupciones globales
	sei();

	while (1)
	{
		// Si se recibe un byte
		if (bufferRX != 0)
		{
			// Mostrar el carácter recibido en los LEDs
			PORTB = bufferRX;
			
			// Limpiar el buffer después de procesar el byte recibido
			bufferRX = 0;
		}
	}
	return 0;
}

// Configuración de pines
void setup(void)
{
	// Configurar puerto B como salida para LEDs
	DDRB = 0xFF;
}

// Inicialización de UART a 9600 baudios
void initUART9600()
{
	// Configuración de pines RX y TX
	DDRD &= ~(1 << DDD0); // RX como entrada
	DDRD |= (1 << DDD1);   // TX como salida

	// Habilitar recepción y transmisión
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// Configurar velocidad de baudios a 9600
	UBRR0 = 103;

	// Configurar tamaño de datos a 8 bits, sin paridad, 1 bit de parada
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// ISR de recepción de UART
ISR(USART_RX_vect)
{
	// Almacenar el byte recibido en el buffer
	bufferRX = UDR0;
}
