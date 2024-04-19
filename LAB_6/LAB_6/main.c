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
			for (int i = 0; i < 6; i++) // Iterar sobre los bits binarios
			{
				if (binary[i])
				PORTB |= (1 << i); // Establecer el bit correspondiente en el puerto B
				else
				PORTB &= ~(1 << i); // Borrar el bit correspondiente en el puerto B
			}
		}
	}
}

void setup(void)
{
	// Configurar pines PB0 a PB5 como salidas para LEDs.
	DDRB |= 0b00111111;
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
	int i = 0;
	while (decimal > 0)
	{
		binary[i] = decimal % 2;
		decimal = decimal / 2;
		i++;
	}
}
