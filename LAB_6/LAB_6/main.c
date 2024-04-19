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

volatile uint8_t bufferTX;

int main(void)
{
    initUART9600();
	sei();
	writeUART('H');
	writeUART('o');
	writeUART('l');
	writeUART('a');
	writeUART(10);
	writeUART(13);
	
	writeTextUART("Hola Mundo");
	
    while (1) 
    {
    }
}

void initUART9600(){
	//Paso 1:  RX como entrada y TX como saldia
	DDRD &= ~(1<<DDD0);
	DDRD |=(1<<DDD1);
	
	//Paso 2: Configurar UCSR0A
	//Registro A
	UCSR0A = 0;
	
	//Paso 3: Configurar UCSR0B
	UCSR0B = 0;
	//Habilitar recepcion y transmision
	UCSR0B |= (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	
	//Paso 4: Configurar UCSR0C> ASYNCRONO, PARIDAD NO. bit de stop. 8 caracteres.
	UCSR0C = 0;
	
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	
	//Paso 5: Configurar Velocidad de Boudare: 9600
	UBRR0 = 103;
	
}

//Interrupcion
void writeUART(char Caracter){
	while(!(UCSR0A &(1<<UDRE0)));// Enviar a compu
	UDR0 = Caracter;
}
void writeTextUART(char* Texto){
	int i;
	for(i=0; Texto[i]!='\0'; i++){
		while (!(UCSR0A &(1<<UDRE0)));
		UDR0 = Texto[i];
	}
}

ISR(USART_RX_vect){ // Resepcion.
	
	bufferTX = UDR0;
	
	while(!(UCSR0A &(1<<UDRE0)));//Enviar de regreso
	UDR0 = bufferTX;
	
	
}