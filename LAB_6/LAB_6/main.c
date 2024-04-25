#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#define F_CPU 16000000

volatile char receivedChar = 0;

void setup() {
	// Configurar el puerto B como salida
	DDRB = 0xFF; // Todos los pines del puerto B como salidas
	// Inicializar la UART a 9600 baudios
	initUART9600(); // Inicializar UART a 9600 baudios
}

void loop() {
	while (1) {
		// Mostrar menú
		writeTextUART("\n\rMenu:\n\r");
		writeTextUART("1. Leer Potenciometro\n\r");
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
	// Configurar ADC para leer el pin PC5
	ADMUX = (1 << REFS0) | (1 << MUX0) | (1 << MUX2); // AVCC como referencia, PC5 como entrada
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Habilitar ADC y configurar preescalador a 128

	ADCSRA |= (1 << ADSC); // Iniciar conversión
	while (ADCSRA & (1 << ADSC)); // Esperar a que la conversión termine

	// Leer valor convertido
	uint16_t value = ADC;

	// Mostrar valor en la terminal
	char buffer[20];
	sprintf(buffer, "\n\rValor del potenciómetro: %d\n\r", value);
	writeTextUART(buffer);
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
	PORTB = asciiCode; // Mostrar el código ASCII en el puerto B
}

// ISR de recepción de UART
ISR(USART_RX_vect) {
	receivedChar = UDR0; // Almacena el carácter recibido
	// Puedes agregar procesamiento adicional aquí antes de enviar de vuelta
	// Envío de vuelta solo después de procesamiento si es necesario
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
