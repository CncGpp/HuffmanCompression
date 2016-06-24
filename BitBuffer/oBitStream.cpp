#include "oBitStream.h"

/** Descrizione:
 *  Costruttore della classe, inizializza l'output bit-stream. Viene chiamato il costruttore della classe base.
 *  Parametri:
 *  path			- Stringa contenente il percorso al file che si vuole scrivere con lo stream.
 *  bufferSize		- Dimensione del buffer da utilizzare per la scrittura.
 *  openMode		- Spacifica in che modo aprire il file per la scrittura				[Default = ios::out]
 * */
oBitStream::oBitStream(string path, int bufferSize, _Ios_Openmode openMode) : BitStream(bufferSize){
	if(path == "") {
		cerr << "ERRORE: IMPOSSIBILE APRIRE IL FILE IN SCRITTURA PER IL BITSTREA, PATH NON VALIDA.\n";
		return;
	}

	file.open(path.c_str(), openMode);

	if(!file.is_open()) {
		cerr << "ERRORE: IMPOSSIBILE APRIRE IL FILE IN SCRITTURA PER IL BITSTREAM.\n";
		return;
	}
}
/** Descrizione:
 *  Distruttore della classe, scrive cio' che resta sul buffer nel file ed infine lo chiude.
 *  NB: la memoria viene liberata dal distruttore della classe base.
 * */
oBitStream::~oBitStream() {
	write();
	file.close();
}

/** Descrizione:
 *  Funzione che svuota il buffer setta a zero tutti i suoi byte e resetta a zero il byte_seek ed il buffer_seek.
 * */
inline void oBitStream::flush(){
	memset(buffer, 0 , buffer_size);						//Uso memset perche molto piu' veloce di altri metodi.
	byte_seek = 0;
	buffer_seek = 0;
}

/** Descrizione:
 *  Scrive il buffer sul file.
 *  Ritorno:
 *  Ritorna il numero di trash bit, cioe' i bit che non facevano parte del buffer ma necessari per scrivere il byte.
 * */
unsigned char oBitStream::write(){
	if(!byte_seek and !buffer_seek)							//Se il buffer e' vuoto, non ho nulla da scrivere.
	return 0;

	if(byte_seek == 0)										//Se il byte_seek e' zero, scrivo fino al buffer_seek...
	file.write(buffer,buffer_seek);
	else													//Altrimenti devo scrivere fino buffer_seek poiche' ci sono
	file.write(buffer,buffer_seek+1);						//bit nel buffer che non hanno completato un byte.

	return 8-byte_seek;
}

/** Descrizione:
 *  Una volta effettuate operazioni di scrittura, si controlla se il buffer e' pieno, in tal caso avviene una sua scrittura
 *  sul file e pertanto sono reinizializzati byte_seek e buffer_seek.
 * */
void oBitStream::endOfBufferCheck(){
	if(byte_seek >= 8){										//Se dopo aver scritto un bit, il byte corrente e' terminato...
		byte_seek = 0;										//Resetto byte_seek ed incremento buffer_seek per scrivere il prossimo
		buffer_seek++;
	}

	if(buffer_seek >= buffer_size){							//Se ho riempito tutto il buffer...
		file.write(buffer,buffer_size);						//Scrivo il buffer sul file e ricomincio da capo.
		flush();											//Pulisco il buffer per prepararlo ad una nuova lettura
	}
}
/** Descrizione:
 *  Overload dell'operatore <<, la funzione scrive un singolo bit in input nel buffer. Dopo aver scritto il bit
 *  si aggiorna il byte_seek e si chiama la funzione endOfBufferCheck(), per verificare se il buffer/byte e' terminato.
 *  Parametri:
 *  b				- booleano da scrivere nel buffer.
 *  Ritorno:
 *  la funzione ritorna un reference all'oggetto stesso in modo da effettuare chiamate sequenziali.
 *  */
oBitStream& oBitStream::operator<< (bool b){
	if(b){													//Se il bit da inserire e' 1, lo insersco alla sua posizione
		unsigned char mask = 1 << (7-byte_seek);			//tramite una maschera, altrimenti e' necessario solo aumentare
		buffer[buffer_seek] |= mask;						//il byte_seek.
	}
		byte_seek++;

		endOfBufferCheck();									//Effettuo i controlli per verificare se il byte/buffer e' terminato.

	return *this;
}
/** Descrizione:
 *  Overload dell'operatore <<, la funzione scrive un byte in input nel buffer. Dopo aver scritto il byte
 *  si aggiornano i seek e si chiama la funzione endOfBufferCheck(), per verificare se il buffer/byte e' terminato.
 *  Parametri:
 *  c				- char da scrivere nel buffer.
 *  Ritorno:
 *  la funzione ritorna un reference all'oggetto stesso in modo da effettuare chiamate sequenziali.
 *  */
oBitStream& oBitStream::operator<< (char c){

	if(byte_seek == 0){										//Se il byte_seek e' 0, mi trovo all'inizio del byte e quindi posso
		buffer[buffer_seek++] = c;							//scrivere c direttamente nel buffer.
		endOfBufferCheck();
		return *this;
	}

	for(int i = 0; i < CHAR_BIT; i++){						//In caso contrario effettuo CHAR_BIT immissioni di bit dal buffer
		*this << (c<0);										//in modo tale che possa scrivere il char.
		c <<=1;
	}

	return *this;
}
