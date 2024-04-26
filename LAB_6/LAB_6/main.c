//*********************************************************************
// Universidad del Valle de Guatemala
// IE2023: Programaci�n de Microcontroladores
// Autor: Alan Gomez
// Proyecto: LAB_6.c
// Descripci�n: Sexto Laboratorio de Programaci�n de Microcontroladores con lenguaje C.
// Hardware: ATmega328p
// Created: 4/18/2024 12:27:37 AM
//*********************************************************************
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

#define F_CPU 16000000

volatile char receivedChar = 0;

void setup() {
	// Configurar los pines del puerto B como salida (PB0 a PB7)
	DDRB = 0xFF;

 
}

void loop() {
	while (1) {
		// Mostrar men�
		writeTextUART("\n\rMenu:\n\r");
		writeTextUART("1. Leer Potenci�metro\n\r");
		writeTextUART("2. Enviar ASCII\n\r");
		writeTextUART("Seleccione una opci�n: ");

		while (receivedChar == 0); // Esperar a que se reciba un car�cter
		char option = receivedChar;
		receivedChar = 0;

		switch(option) {
			case '1':
			// Leer valor del potenci�metro
			readPotentiometer();
			break;
			case '2':
			// Enviar ASCII
			sendASCII();
			break;
			default:
			writeTextUART("\n\rOpci�n no v�lida\n\r");
			break;
		}
	}
}

// Configuraci�n de UART a 9600 baudios
void initUART9600() {
	// Configuraci�n de pines RX y TX
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // Habilitar recepci�n, transmisi�n y la interrupci�n de recepci�n
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Configurar tama�o de datos a 8 bits

	// Configurar velocidad de baudios a 9600
	UBRR0 = 103; // Para F_CPU 16MHz y baudrate de 9600
}

void writeTextUART(char* Texto) {
	int i;
	for (i = 0; Texto[i] != '\0'; i++) {
		while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = Texto[i];
	}
}

// Leer valor del potenci�metro
void readPotentiometer() {
	// Configurar ADC para leer el pin ADC6 (PC0)
	ADMUX = (1 << REFS0) | (1 << MUX1) | (1 << MUX2); // AVCC como referencia, ADC6 como entrada
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Habilitar ADC, iniciar conversi�n y configurar preescalador a 128

	// Esperar a que la conversi�n termine
	while (ADCSRA & (1 << ADSC));

	// Leer valor convertido
	uint8_t value = ADCH;

	// Mostrar valor en los LEDs del puerto B
	PORTB = value;
}

// Enviar c�digos ASCII
void sendASCII() {
	writeTextUART("\n\rEnv�a el c�digo ASCII (0-255): ");

	while (receivedChar == 0); // Esperar a que se reciba un car�cter
	uint8_t asciiCode = receivedChar;
	receivedChar = 0;

	// Enviar c�digo ASCII al puerto serie
	char buffer[20];
	sprintf(buffer, "\n\rEnviando c�digo ASCII: %c\n\r", asciiCode);
	writeTextUART(buffer);

	// Mostrar el c�digo ASCII en los LEDs del puerto B
	PORTB = asciiCode;
}

// ISR de recepci�n de UART
ISR(USART_RX_vect) {
	receivedChar = UDR0; // Almacena el car�cter recibido
	while (!(UCSR0A &(1 << UDRE0))); // Enviar de regreso
	UDR0 = receivedChar;
}

// Inicializaci�n del ADC
void initADC() {
	ADMUX = 0; // Configurar ADMUX inicialmente en 0 para leer desde ADC0 (PC0)
	ADCSRA = 0; // Asegurarse de que ADC est� apagado inicialmente
}

int main(void) {
	setup(); // Configurar el microcontrolador

	// Habilitar interrupciones globales
	sei();

	loop(); // Loop principal

	return 0;
}
