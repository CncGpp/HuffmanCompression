/*
 * iBitStream.h
 *
 *  Created on: 12 feb 2016
 *  Author: CncGpp
 */

#ifndef IBITSTREAM_H_
#define IBITSTREAM_H_
#include "BitStream.h"

/** Classe che simula un input bit-stream, eredita dalla classe BitStream la struttura interna del bit-stream
 *  e fornisce essenzialmente solo le funzioni base per la lettura.
 *  */
class iBitStream : public BitStream {
public:
	iBitStream(string path, int bufferSize, _Ios_Openmode openMode = ios::in);	//Costruttore di classe con parametri.
	virtual ~iBitStream();														//Distruttore di classe.

	ifstream& getFile() {return file;}											//Metodo getter, ritorna uno stream al file che si sta leggendo
	int read();																	//Legge memorizzando nel buffer il file, ritorna il numero di byte letti.
	inline void flush();														//Svuota il buffer e resetta a zero le posizioni dei seek.
	bool operator>> (bool& b);													//Overload dell'operatore >>, legge un bit dal buffer e lo memorizza in b
	bool operator>> (char& c);													//Overload dell'operatore >>, legge un byte dal buffer e lo memorizza in c

private:
	ifstream file;																//Input stream al file che si sta leggendo con il buffer
	int goodByte;																//Numero di byte effettivamente letti e usabili nel buffer
	void endOfBufferCheck();													//Controlla se il buffer e' terminato e se necessario lo rilegge.

};

#endif /* IBITSTREAM_H_ */
