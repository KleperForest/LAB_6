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
void decimalToBinary(uint8_t decimal, uint8_t binary[]);

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
			// Convertir el byte recibido a su representación binaria y encender LEDs
			uint8_t binary[8] = {0};
			decimalToBinary(bufferRX, binary);
			for (int i = 0; i < 6; i++)
			{
				if (binary[i])
				PORTB |= (1 << i); // Establecer el bit correspondiente en el puerto B
				else
				PORTB &= ~(1 << i); // Borrar el bit correspondiente en el puerto B
			}
			// Limpiar el buffer después de procesar el byte recibido
			bufferRX = 0;
		}
	}
	return 0;
}

// Configuración de pines
void setup(void)
{
	// Configurar pines PB0 a PB5 como salidas para LEDs
	DDRB |= 0b00111111;
}

// Inicialización de UART a 9600 baudios
void initUART9600()
{
	// Configuración de pines RX y TX
	DDRD &= ~(1 << DDD0); // RX como entrada
	DDRD |= (1 << DDD1);   // TX como salida

	// Habilitar recepción y transmisión
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);

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

	// Enviar el byte de vuelta
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = bufferRX;
}

// Función para convertir decimal a binario
void decimalToBinary(uint8_t decimal, uint8_t binary[])
{
	int i = 0;
	while (decimal > 0)
	{
		binary[i] = decimal % 2;
		decimal = decimal / 2;
		i++;
	}
}
