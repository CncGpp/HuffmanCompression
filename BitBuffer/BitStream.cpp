/*
 * BitStream.cpp
 *
 *  Created on: 12 feb 2016
 *  Author: CncGpp
 */

#include "BitStream.h"

/**
 * Descrizione:
 * Costruttore della classe, verifica la bonta' dell'input ed inizializza i vari attributi della classe
 * Parametri:
 * bufferSize		- Dimensione del buffer da utilizzare per la lettura/scrittura
 * */
BitStream::BitStream(int bufferSize) : buffer_size(bufferSize) {
	if(bufferSize <= 0){
		cerr << "ERRORE: IL BUFFER NON PUO' AVERE SIZE NEGATIVO O NULLO." << endl;
		return;
	}

	buffer = new char[bufferSize]();
	buffer_seek = 0;
	byte_seek = 0;
}
/**
 * Descrizione:
 * Distruttore della classe, libera la memoria allocata per il buffer.
 * */
BitStream::~BitStream() {
	delete[] buffer;
}

/**
 * Descrizione:
 * Stampa il contenuto del buffer, byte a byte, in formato binario, utilizza print_bit().
 * */
void BitStream::printBin(){
	for(int i = 0; i<buffer_size; i++){
		print_bit(buffer[i]);
		std::cout << " ";
	}
}
/**
 * Descrizione:
 * Stampa il contenuto del buffer, byte a byte, in formato esadecimale.
 * */
void BitStream::printHex(){
	for(int i = 0; i<buffer_size; i++){
		std::cout << std::hex << (int)buffer[i] << " ";
	}
}
/**
 * Descrizione:
 * Stampa il char fornito in input in formato binario.
 * Parametri:
 * c			- char da stampare in binario.
 * */
void BitStream::print_bit(char c){
	for(int i = 0; i<CHAR_BIT; i++){
		std::cout << (c<0);						//Stampo il bit più significativo
		c = c << 1;								//e shift a destra di un bit.
	}

}













