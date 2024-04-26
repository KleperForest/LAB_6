#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#define F_CPU 16000000

volatile char receivedChar = 0;

void setup() {
	// Configurar los pines del puerto B como salida (PB0 a PB3)
	DDRB |= 0x0F;

	// Configurar los pines del puerto D como salida (PD4 a PD7)
	DDRD |= 0xF0;

	// Inicializar la UART a 9600 baudios
	initUART9600();
}

void loop() {
	while (1) {
		// Mostrar men�
		writeTextUART("\n\rMenu:\n\r");
		writeTextUART("1. Leer Potenciometro\n\r");
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
	// Configurar ADC para leer el pin PC5
	ADMUX = (1 << REFS0) | (1 << MUX0) | (1 << MUX2); // AVCC como referencia, PC5 como entrada
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Habilitar ADC y configurar preescalador a 128

	ADCSRA |= (1 << ADSC); // Iniciar conversi�n
	while (ADCSRA & (1 << ADSC)); // Esperar a que la conversi�n termine

	// Leer valor convertido
	uint16_t value = ADC;

	// Mostrar valor en la terminal
	char buffer[20];
	sprintf(buffer, "\n\rValor del potenci�metro: %d\n\r", value);
	writeTextUART(buffer);
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

	// Mostrar el c�digo ASCII en los pines de salida
	PORTB = asciiCode & 0x0F; // Los 4 bits menos significativos en PB0 a PB3
	PORTD = (asciiCode & 0xF0) >> 4; // Los 4 bits m�s significativos en PD4 a PD7
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

	loop(); // Loop principal

	return 0;
}
