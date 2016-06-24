/*
 * Huffman.h
 *
 *  Created on: 12 feb 2016
 *  Author: CncGpp
 */

#ifndef HUFFMAN_H_
#define HUFFMAN_H_
#include <iostream>
#include <iomanip> 		 //PER setw()
#include <climits>		 //Per CHAR_BIT, numero di bit in un char
#include <string.h>		 //Per memset
#include <vector>
#include <map>
#include <math.h>
#include "BitBuffer/BitBuffer.h"
#include "HuffmanTree/HuffmanTree.h"
#include "PriorityQueue/PriorityQueue.h"
using namespace std;

class Huffman {

	typedef unsigned char byte;
	typedef vector<bool> code;

public:
	//  COSTRUTTORE
	Huffman(const unsigned int bufferSize = 1024, const string extension = "cnc");
	//  DISTRUTTORE
	virtual ~Huffman();

	//  METODI DELLA CLASSE
	void printHuffmanCodes() const;									//Stampa i codici di Huffman generati per ogni carattere.
	void compress(string path);										//Comprime il file che si trova alla path di input
	void decompress(string path);									//!<Decomprime il file che si trova alla path di input
	long double getEntropy(string path);
	string getCOMPRESS_EXTRENSION() const {return COMPRESS_EXTRENSION;}


private:
	//  ATTRIBUTI PRIVATI
	static const int MAX_SYMBOLS = 1<<CHAR_BIT;						//Numero dei possibili caratteri diversi in un byte - 2^(numero di bit in un char)
	const unsigned int BUFFER_SIZE;									//Dimensione del buffer usato per la lettura e scrittura dei file.
	const string COMPRESS_EXTRENSION;								//Estensione del file compresso.
	unsigned long int* freq_table;									//Tavola delle frequenze relative ad ogni carattere [mediante array dinamico]
	unsigned long int total_symbols;								//Numero totale dei simboli da codificare, equivale alla grandezza del file
	HNode* huffmanTree;												//Puntatore all'albero di Huffman generato per la compressione/decompressione
	map<byte,code> huffmanCodes;									//Mappa di tipo <char, vector<bool>> contenente il carattere con relativo codice di Huffman

	//  METODI PRIVATI
	inline void intd_freq_table();									//Inizializza a tutti 0 le frequenze
	void symbolsFrequency(string path);								//Calcola le frequenze per ogni carattere del file che si trova alla path di input
	void buildHuffmanTree();										//Costruisce l'albero di Huffman basandosi sulle frequenze
	void generateCodes();											//Genera i codici di Huffman relativi ad i singoli caratteri e li inserisce nella map
	void _generateCodes(HNode* root, code c);
	const string boolVector2String(const code&) const;				//Funzione per la stampa, converte i vettori di bool in una stringa

	void writeHeader(string& path);									//Scrive l'header sul file che si trova alla path di input.
	string readHeader(iBitStream* reader);							//Legge l'header sul file che si trova alla path di input.
	void writeHuffmanTree(HNode* root, oBitStream* writer) const;	//Funzione che linearizza l'albero di huffman e lo scrive su un file in input
	HNode* readHuffmanTree(iBitStream* reader);						//Funzione legge l'albero di huffman linearizzato da un file di input

	const string removeExtension(const string& path) const;			//Rimuove, se c'e', l'estensione da una stringa contenente il path di un file.
	const string extractExtension(const string& path) const;		//Ritorna, se c'e',l'estensione di una stringa contenente il path di un file.
	void printStats(string path1, string path2) const;				//Stampa le statistiche dopo la compressione o la decompressione
};

#endif /* HUFFMAN_H_ */
