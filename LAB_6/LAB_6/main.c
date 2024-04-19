//*********************************************************************
// Universidad del Valle de Guatemala
// IE2023: Programaci�n de Microcontroladores
// Autor: Alan Gomez
// Proyecto: LAB_6.c
// Descripci�n: Sexto Laboratorio de Programaci�n de Microcontroladores con lenguaje C.
// Hardware: ATmega328p
// Created: 4/18/2024 12:27:37 AM
//*********************************************************************

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL

volatile char receivedChar = 0;

void setup() {
	// Configurar el puerto B como salida
	DDRB = 0xFF; // Todos los pines del puerto B como salidas
	// Inicializar la UART a 9600 baudios
	initUART9600(); // Inicializar UART a 9600 baudios
}

void loop() {
	if (receivedChar != 0) {
		// Muestra el car�cter recibido en el puerto B
		PORTB = receivedChar;
		receivedChar = 0; // Reiniciar la variable para recibir un nuevo car�cter
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

// ISR de recepci�n de UART
ISR(USART_RX_vect) {
	receivedChar = UDR0; // Almacena el car�cter recibido
	// Puedes agregar procesamiento adicional aqu� antes de enviar de vuelta
	// Env�o de vuelta solo despu�s de procesamiento si es necesario
	while(!(UCSR0A &(1<<UDRE0)));//Enviar de regreso
	UDR0 = receivedChar;
}

int main(void) {
	setup(); // Configurar el microcontrolador

	// Habilitar interrupciones globales
	sei();

	writeTextUART("Hola Mundo");
	while (1) {
		loop(); // Loop principal
	}

	return 0;
}
