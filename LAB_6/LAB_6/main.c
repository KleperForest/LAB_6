//*********************************************************************
// Universidad del Valle de Guatemala
// IE2023: Programación de Microcontroladores
// Autor: Alan Gomez
// Proyecto: LAB_6.c
// Descripción: Sexto Laboratorio de Programación de Microcontroladores con lenguaje C.
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
		// Mostrar menú
		writeTextUART("\n\rMenu:\n\r");
		writeTextUART("1. Leer Potenciómetro\n\r");
		writeTextUART("2. Enviar ASCII\n\r");
		writeTextUART("Seleccione una opción: ");

		while (receivedChar == 0); // Esperar a que se reciba un carácter
		char option = receivedChar;
		receivedChar = 0;

		switch(option) {
			case '1':
			// Leer valor del potenciómetro
			readPotentiometer();
			break;
			case '2':
			// Enviar ASCII
			sendASCII();
			break;
			default:
			writeTextUART("\n\rOpción no válida\n\r");
			break;
		}
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

void writeTextUART(char* Texto) {
	int i;
	for (i = 0; Texto[i] != '\0'; i++) {
		while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = Texto[i];
	}
}

// Leer valor del potenciómetro
void readPotentiometer() {
	// Configurar ADC para leer el pin ADC6 (PC0)
	ADMUX = (1 << REFS0) | (1 << MUX1) | (1 << MUX2); // AVCC como referencia, ADC6 como entrada
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Habilitar ADC, iniciar conversión y configurar preescalador a 128

	// Esperar a que la conversión termine
	while (ADCSRA & (1 << ADSC));

	// Leer valor convertido
	uint8_t value = ADCH;

	// Mostrar valor en los LEDs del puerto B
	PORTB = value;
}

// Enviar códigos ASCII
void sendASCII() {
	writeTextUART("\n\rEnvía el código ASCII (0-255): ");

	while (receivedChar == 0); // Esperar a que se reciba un carácter
	uint8_t asciiCode = receivedChar;
	receivedChar = 0;

	// Enviar código ASCII al puerto serie
	char buffer[20];
	sprintf(buffer, "\n\rEnviando código ASCII: %c\n\r", asciiCode);
	writeTextUART(buffer);

	// Mostrar el código ASCII en los LEDs del puerto B
	PORTB = asciiCode;
}

// ISR de recepción de UART
ISR(USART_RX_vect) {
	receivedChar = UDR0; // Almacena el carácter recibido
	while (!(UCSR0A &(1 << UDRE0))); // Enviar de regreso
	UDR0 = receivedChar;
}

// Inicialización del ADC
void initADC() {
	ADMUX = 0; // Configurar ADMUX inicialmente en 0 para leer desde ADC0 (PC0)
	ADCSRA = 0; // Asegurarse de que ADC esté apagado inicialmente
}

int main(void) {
	setup(); // Configurar el microcontrolador

	// Habilitar interrupciones globales
	sei();

	loop(); // Loop principal

	return 0;
}
