/*
 * BitStream.h
 *
 *  Created on: 12 feb 2016
 *  Author: CncGpp
 */

#ifndef BITSTREAM_H_
#define BITSTREAM_H_
#include <iostream>
#include <string.h>
#include <fstream>
using namespace std;

/**Classe che simula un bit stream per la scrittura/lettura su file, permettendo di scrivere o leggere bit a bit un file
 * la classe per velocizzare le operazioni utilizza un buffer che viene scritto solamente quando e' pieno*/
class BitStream {
public:
	BitStream(int bufferSize);							//Costruttore della classe, ha come parametro la dimensione del buffer.
	virtual ~BitStream();								//Distruttore della classe, dealloca la memoria e scrive i bit restanti nel buffer.

	unsigned char write();								//Scrive il contenuto del buffer sul file, ritorna il numero di trash bit scritti.
	int read();											//Legge memorizzando nel buffer il file, ritorna il numero di byte letti.
	char* getBuffer() {return buffer;}					//Metodo getter, ritorna un puntatore al buffer.
	char getSeekByte(){return buffer[buffer_seek];}		//Metodo getter, ritorna il byte del buffer indicizzato dalla posizione del byte_seek.
	void nextByte(){									//Consente di avanzare di un byte sul buffer.
		buffer_seek++;
		byte_seek = 0;
	}
	virtual inline void flush(){}						//Svuota il buffer e resetta a zero le posizioni dei seek.
	void printBin();									//Stampa su console in formato binario il contenuto del buffer in questo momento.
	void printHex();									//Stampa su console in formato hex il contenuto del buffer in questo momento.


protected:
	char* buffer;										//Buffer, rappresentato come Array di char
	const int buffer_size;								//Dimensione del buffer, fornita in input
	int buffer_seek;									//Seek del buffer, indica la posizione per la lettura/scrittura sul buffer
	unsigned char byte_seek;							//Seek del byte, indica la posizione per la lettura/scrittura di un bit sul byte del buffer_seek

	void print_bit(char c);								//Metodo privato, converte un char in binario per la stampa.


};

#endif /* BITSTREAM_H_ */
