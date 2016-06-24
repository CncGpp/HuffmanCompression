
#ifndef OBITSTREAM_H_
#define OBITSTREAM_H_
#include "BitStream.h"

/** Classe che simula un output bit-stream, eredita dalla classe BitStream la struttura interna del bit-stream
 *  e fornisce essenzialmente solo le funzioni base per la scrittura.
 *  */
class oBitStream : public BitStream {
public:
	oBitStream(string path, int bufferSize, _Ios_Openmode openMode = ios::out);		//Costruttore di classe con parametri.
	virtual ~oBitStream();															//Distruttore di classe.

	ofstream& getFile() {return file;}												//Metodo getter, ritorna uno stream al file che si sta scrivendo
	unsigned char write();															//Scrive il contenuto del buffer nel file, ritorna il numero di trash bit.
	inline void flush();															//Svuota il buffer e resetta a zero le posizioni dei seek.
	oBitStream& operator<< (bool b);												//Overload dell'operatore <<, scrive il bit in input nel buffer
	oBitStream& operator<< (char c);												//Overload dell'operatore <<, scrive il char in input nel buffer

private:
	ofstream file;																	//Output stream al file che si sta scrivendo con il buffer
	void endOfBufferCheck();														//Controlla se il buffer e' terminato e se necessario lo rilegge.

};
#endif /* OBITSTREAM_H_ */
