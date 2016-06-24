/*
 * iBitStream.cpp
 *
 *  Created on: 12 feb 2016
 *  Author: CncGpp
 */

#include "iBitStream.h"

/** Descrizione:
 *  Costruttore della classe, inizializza l'input bit-stream. Viene chiamato il costruttore della classe base.
 *  Parametri:
 *  path			- Stringa contenente il percorso al file che si vuole leggere con lo stream.
 *  bufferSize		- Dimensione del buffer da utilizzare per la lettura.
 *  openMode		- Spacifica in che modo aprire il file per la lettura				[Default = ios::in]
 * */
iBitStream::iBitStream(string path, int bufferSize, _Ios_Openmode openMode) : BitStream(bufferSize){
	if(path == "") {
		cerr << "ERRORE: IMPOSSIBILE APRIRE IL FILE IN SCRITTURA PER IL BITSTREAM, PATH NON VALIDA.\n";
		return;
	}

	file.open(path.c_str(), openMode);
	if(!file.is_open()) {
		cerr << "ERRORE: IMPOSSIBILE APRIRE IL FILE IN SCRITTURA PER IL BITSTREAM.\n";
		return;
	}
	goodByte = read();
}

/** Descrizione:
 *  Distruttore della classe, chiude il file aperto per lo stream.
 *  NB: la memoria viene liberata dal distruttore della classe base.
 * */
iBitStream::~iBitStream() {
	file.close();
}

/** Descrizione:
 *  Funzione che svuota il buffer setta a zero tutti i suoi byte e resetta a zero il byte_seek, il buffer_seek e goodByte.
 * */
inline void iBitStream::flush(){
	memset(buffer, 0 , buffer_size);								//Uso memset perche molto piu' veloce di altri metodi.
	byte_seek = 0;
	buffer_seek = 0;
	goodByte = 0;
}

/** Descrizione:
 *  Legge una porzione di file memorizzandolo nel buffer.
 *  Ritorno:
 *  Ritorna il numero di byte effettivamente letti dal file.
 * */
int iBitStream::read(){
	 file.read(buffer, buffer_size );
	 return file.gcount();											//Torna il numero di bytes letti
}

/** Descrizione:
 *  Overload dell'operatore >>, la funzione legge un singolo bit dal buffer e lo memorizza in b. Dopo aver letto il bit
 *  si aggiorna il byte_seek e si chiama la funzione endOfBufferCheck(), per verificare se il buffer/byte e' terminato.
 *  Parametri:
 *  b				- booleano dove viene memorizzato il bit letto, pertanto e' passato con un reference.
 *  Ritorno:
 *  la funzione ritorna true in caso di successo nella lettura e false in caso di insuccesso (es. file e buffer terminato).
 *  */
bool iBitStream::operator>> (bool& b){

	if(buffer_seek >= goodByte) return false;			//Se non ho piu' bit da leggere ritorno false.
	b = buffer[buffer_seek] < 0;						//Altrimenti estraggo il bit più significativo dal char puntato da buffer_seek
	buffer[buffer_seek] <<= 1;
	byte_seek++;

	endOfBufferCheck();									//Effettuo i controlli per verificare se il byte/buffer e' terminato.

	return true;
}
/** Descrizione:
 *  Overload dell'operatore >>, la funzione legge un byte dal buffer e lo memorizza in c. Dopo aver letto il byte
 *  si aggiornano i seek e si chiama la funzione endOfBufferCheck(), per verificare se il buffer/byte e' terminato.
 *  Parametri:
 *  c				- char dove viene memorizzato il bit letto, pertanto e' passato con un reference.
 *  Ritorno:
 *  la funzione ritorna true in caso di successo nella lettura e false in caso di insuccesso (es. file e buffer terminato).
 *  */
bool iBitStream::operator>> (char& c){
	if(buffer_seek >= goodByte) return false;			//Se non ho piu' byte da leggere ritorno false.

	if(byte_seek == 0){									//Se il byte_seek e' 0, mi trovo all'inizio del byte e quindi posso
		c = buffer[buffer_seek++];						//leggerlo direttamente dal buffer e memorizzarlo in c.
		endOfBufferCheck();
		return true;
	}

	for(int i = 0; i < CHAR_BIT; i++){					//In caso contrario effettuo CHAR_BIT estrazioni di bit dal buffer
		bool b=0;										//in modo tale che possa formare un char.
		*this >> b;
		c <<= 1;
		c |= b;
	}
	return true;
}

/** Descrizione:
 *  Una volta effettuate operazioni di lettura, si controlla se il buffer e' stato letto tutto, in tal caso avviene una nuova lettura
 *  dal file e pertanto sono reinizializzati byte_seek, buffer_seek e goodByte.
 * */
void iBitStream::endOfBufferCheck(){
	if(byte_seek >= 8){											//Se dopo aver letto un bit, il byte corrente e' terminato...
		byte_seek = 0;											//Resetto byte_seek ed incremento buffer_seek per leggere il prossimo
		buffer_seek++;
	}

	if(buffer_seek >= buffer_size){								//Se ho letto tutto il buffer...
		goodByte=read();										//Leggo una nuova porzione di file e ricomincio da capo.
		buffer_seek= 0;
	}
}

