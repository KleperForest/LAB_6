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
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile char receivedChar = 0;

void setup() {
	// Configurar el puerto B como salida
	DDRB = 0xFF; // Todos los pines del puerto B como salidas
	// Inicializar la UART a 9600 baudios
	initUART9600(); // Inicializar UART a 9600 baudios
}

void loop() {
	if (receivedChar != 0) {
		// Muestra el carácter recibido en el puerto B
		PORTB = receivedChar;
		receivedChar = 0; // Reiniciar la variable para recibir un nuevo carácter
	}
}

// Configuración de UART a 9600 baudios
void initUART9600() {
	// Configuración de pines RX y TX
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // Habilitar recepción, transmisión y la interrupción de recepción
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Configurar tamaño de datos a 8 bits

	// Configurar velocidad de baudios a 9600
	UBRR0 = 103; // Para F_CPU 16MHz y baudrate de 9600
}

// ISR de recepción de UART
ISR(USART_RX_vect) {
	receivedChar = UDR0; // Almacena el carácter recibido
}

int main(void) {
	setup(); // Configurar el microcontrolador

	// Habilitar interrupciones globales
	sei();

	while (1) {
		loop(); // Loop principal
	}

	return 0;
}
