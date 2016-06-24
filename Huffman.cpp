/*
 * Huffman.cpp
 *
 *  Created on: 12 feb 2016
 *  Author: CncGpp
 */

#include "Huffman.h"

/**	Funzione: Costruttore
 *	Descrizione:
 *	Inizializza a nullptr, l'albero di huffman, l'array contenente le frequenze dei simboli, la dimensione del buffer
 *	e l'estensione del file una volta compresso.
 * 	Parametri:
 * 	bufferSize	- Dimensione, in byte, del buffer usato per la lattura e scrittura		[default 1024 byte]
 * 	extension	- Estensione che identifica il file una volta compresso					[default "cnc"]
 * */
Huffman::Huffman(const unsigned int bufferSize, const string extension) : BUFFER_SIZE(bufferSize), COMPRESS_EXTRENSION(extension){
	huffmanTree = nullptr;													//Inizialmente l'albero di huffman e' vuoto
	freq_table = nullptr;
	total_symbols = 0;
}
/**	Funzione: Distruttore
 *	Descrizione:
 *	libera la memoria allocata per l'array delle frequenze e per l'albero di huffman
 * */
Huffman::~Huffman() {
	delete[] freq_table;													//Libero la memoria destinata alla tabella delle frequenze
	huffmanTree->freeTreeMemory(huffmanTree);

}

/**	Funzione: intd_freq_table
 * 	Descrizione:
 * 	Inizializza freq_table e total_symbols, utilizza memset in quanto molto piu' veloce rispetto altri metodi
 * */
inline void Huffman::intd_freq_table(){
	if(freq_table!=nullptr)													//Mi assicuro che la memoria per freq_table sia stata allocata
	memset(freq_table,0,sizeof(unsigned long int)*MAX_SYMBOLS);
	total_symbols = 0;
}
/**
 * 	Descrizione:
 * 	Legge tramite un buffer un file e ne calcola il numero di occorrenze di ogni simbolo memorizzandolo in freq_table e calcola il
 * 	numero totale dei simboli, cioe' le dimensioni in byte del file, salvandolo in total_symbols
 * 	Parametri:
 * 	path			        - Stringa contenente il percorso del file
 * 	Ritorno:
 * 	total_symbols			- Il numero totale dei simboli presenti nel file
 * */
void Huffman::symbolsFrequency(string path){
	freq_table = new unsigned long int[MAX_SYMBOLS];  						//Alloco la memoria per contenere le frequenze
	intd_freq_table();														//Inizializzo la tavola delle frequenze
	iBitStream buff(path, BUFFER_SIZE, ios::binary);						//Creo un buffer per il file

	char c;
	while(buff >> c)  ++freq_table[ (byte) c ];								//Per ogni char vado ad aumentare il valore corrispondente nell'array.

	buff.getFile().clear();
	total_symbols = buff.getFile().tellg();
}
/**Funzione: buildHuffmanTree
 * 	Descrizione:
 * 	Costruisce un albero di huffman partendo delle frequenze dei simboli calcolata su un file.
 *	NB: Richiede che le frequenze siano state calcolate dalla funzione symbolsFrequency
 * 	Ritorno:
 * 	total_symbols	- Il numero totale dei simboli presenti nel file
 * */
void Huffman::buildHuffmanTree(){
	/*Funzione che una volta calcolate le frequenze costruisce l'albero di Huffman sfruttando una coda di priorità
	 * 1)Si crea per ogni carattere con frequenza>0, un nodo foglia e lo si inserisce in una min-coda di priorità ordinata in base alle frequenze
	 * 2)Si estraggono i due nodi minimi e si crea un albero binario in cui quest'ultimi sono figli e la cui radice è un nodo contenente la somma
	 *   delle frequenze dei figli
	 * 3)Si aggiunge l'albero appena creato nella min-coda
	 * 4)Si ripetono i passaggi 2-3 fin che non rimane un unico nodo(albero) nella coda (cioè l'albero di huffman)*/

	PriorityQueue<HNode*> queue( [](HNode* a, HNode* b){return a->getFrequency() < b->getFrequency();} );

	//Scorro i caratteri e per tutti quelli con frequenze positive, alloco e poi inserisco nella coda un nodo foglia 'LNode'
	for(int i = 0; i<MAX_SYMBOLS; i++)
		if(freq_table[i]!=0)
			queue.insert(new LNode(freq_table[i], (byte)i ));

	while(queue.size()>1){							//Fin quando nella coda non resta un solo elemento...
		HNode* a = queue.remove();				  	//Estraggo i due puntatori ai nodi con frequenza minima
		HNode* b = queue.remove();
		INode* c = new INode(a,b);					//Creo un nuovo nodo 'INode' interno avente come figli i nodi puntati da a e b
		queue.insert(c);							//Inserisco tale nodo nella coda di priorità
	}


	huffmanTree = queue.remove();          		//Salvo l'albero risultante dall'estrazione dell'ultimo elemento
}

/** Funzione: generateCodes
 * 	Descrizione:
 * 	Funzione ricorsiva che una volta creato l'albero di Huffman, genera i codici relativi ad ogni carattere e li memorizza nella map
 * 	huffmanCodes semplicemente percorrendo il cammino radice-foglia effettuato per raggiungere il carattere.
 * 	- Il codice e' una sequenza binaria di 0 e 1
 * 	- lungo tanto quanto il percorso radice-foglia
 * 	- Composto da 1 se si va a sinistra e 0 se si va a destra lungo il percorso per raggiungere il carattere
 *
 * 	NB: Richiede che sia stato costruito l'albero di huffman tramite la funzione buildHuffmanTree
 * */
void Huffman::generateCodes(){
	//CASO  #1 L'albero di huffman e' vuoto, non devo eseguire alcuna operazione
	if(huffmanTree == 0) return;

	//CASO  #2 L'albero di huffman ha un solo nodo (la radice)
	if(huffmanTree->getLeft() == 0 and huffmanTree->getRight()==0){
		code c(1,true);	//Vettore di bool con un solo elemento '1'
		huffmanCodes.insert(make_pair(huffmanTree->getC(), c));				//Lo inserisco nella map
	}

	//CASO  #3 L'albero ha piu' di un nodo, chiamo la procedura ricorsiva.
	else {
		code c; //vettore di bool vuoto.
		_generateCodes(huffmanTree,c);
	}
}
void Huffman::_generateCodes(HNode* root, code c){
	// CASO BASE Mi trovo ad una foglia?
	if(root->getLeft() == 0 and root->getRight()==0){
		huffmanCodes.insert(make_pair(root->getC(), c));
		return;
	}
	//Altrimenti se non mi trovo alla foglia chiamo ricorsivamente a destra e a sinistra
	code c_sx = c;
	c_sx.push_back(true);						//vettore di bool con aggiunto alla fine '1'
	_generateCodes(root->getLeft(), c_sx);		//Chiamo ricorsivamente nel sottoalbero sinistro
	code c_dx = c;
	c_dx.push_back(false);						//vettore di bool con aggiunto alla fine '0'
	_generateCodes(root->getRight(), c_dx);		//Chiamo ricorsivamente nel sottoalbero destro
}
/**
 * 	Descrizione:
 *	Converte un vettore di bool in una stringa in modo da poterla stampare con facilita'.
 *	Parametri:
 *	v		- vettore di bool da convertire in stringa
 * */
const string Huffman::boolVector2String(const code& v) const {
	string s = "";
	for(unsigned int i=0; i<v.size(); i++){
		if(v[i])
			s+="1";
		else
			s+="0";
	}
	return s;
}
/**
 * 	Descrizione:
 *	Stampa i codici di huffman relativi ad ogni simbolo memorizzati nella map huffmanCodes.
 * */
void Huffman::printHuffmanCodes() const{
	if(huffmanCodes.empty()) return;  							//Se la map e' vuota non stampo nulla

	map<byte,code>::const_iterator it = huffmanCodes.begin();	//Iteratore alla map in modo da poterla scorrere con facilita'

	// CALCOLO IL CODICE PIU' LUNGO
	unsigned int max_len=0;
	for(;it!=huffmanCodes.end();it++)							//Calcolo la lunghezza massima fra tutti i codici, in modo da
		if(it->second.size()>max_len)							//adattare la stampa ai codici
			max_len = it->second.size();

	// GENERO IL SEPARATORE
	string separator = "+---+";									//Genero il separatore utilizzato durante la stampa
	for(unsigned int i=0; i<max_len; i++)						//La sua lunghezza varia in funzione di  max_len
		separator+="-";
	separator+="+";

	// STAMPO TUTTI I CODICI									//Stampo tutti i codici.
	cout<<separator<<endl;

	for(it=huffmanCodes.begin();it!=huffmanCodes.end();it++){
		cout<<"|'"<<it->first<<"'|" <<setw(max_len)<<boolVector2String(it->second) << "|" <<endl;
		cout<<separator<<endl;
	}
}

/**
 * 	Descrizione:
 *	Scrive l'albero di huffman su un file, tramite un buffer oBitStream fornito in input, la scrittura avviene tramite
 *	una visita pre-order dell'albero.
 *	Parametri:
 *	root		- puntatore alla radice dell'albero di huffman da scrivere
 *	writer		- puntatore al bit-stream che scrive sul file
 * */
void Huffman::writeHuffmanTree(HNode* root, oBitStream* writer) const {
    if (root->getLeft() == nullptr and root->getRight() == nullptr){	//CASO BASE: root e' una foglia?
        *writer << true;												//Scrivi 1 + la rappresentazione binaria del simbolo
        char c = root->getC();
        *writer << c;
    }else {																//ALTRIMENTI:
        *writer<<false;													//Scrivi 0 ed effettua le chiamate ricorsive ai figli
        writeHuffmanTree(root->getLeft(), writer);
        writeHuffmanTree(root->getRight(), writer);
    }
}
/**
 * 	Descrizione:
 *	Legge l'albero di huffman da un file, tramite un buffer iBitStream fornito in input, la lettura avviene tramite
 *	una visita pre-order dell'albero.
 *	Parametri:
 *	reader		- puntatore al bit-stream che legge dal file
 *	Ritorno:
 *	Puntatore all'albro di huffman ricostruito leggendo il file
 * */
HNode* Huffman::readHuffmanTree(iBitStream* reader){
    bool b;
    *reader >> b;														//Leggo un bit dallo stream
    if (b==1) {															//Se questo bit e' 1, vuol dire che mi trovo ad una foglia e che
        char c;															//dopo ho memorizzato un simbolo
        *reader >> c;													//Pertanto lo leggo, creo un nuovo nodo foglia 'LNode' e lo ritorno.
        return new LNode(0, c);
    }else {																//Altrimenti effettuo le chiamate ricorsive ai figli
        HNode* leftChild = readHuffmanTree(reader);
        HNode* rightChild = readHuffmanTree(reader);
        return new INode(leftChild, rightChild);						//Creando un nuovo nodo Interno 'INode'
    }
}
/**
 * 	Descrizione:
 * 	Scrive sul file fornito da una path l'header necessario alla decodifica del file. L'header si presenta nella seguente forma:
 * 	|  	   PARTIZIONE A      |    PARTIZIONE B	 |		     PARTIZIONE C			 |
 * 	+----+----+----+----+----+----+----+----+----+-----------------------------+----+
 * 	|char|char|....|char|'\0'| unsigned long int |     Albero di huffman ..... |'\0'|
 * 	+----+----+----+----+----+----+----+----+----+----------------------------+-----+
 * 	A:= Stringa contenente la vecchia estensione del file, puo' essere lunga a piacere.
 * 	B:= Valore contenente il numero totale di simboli codificati, cioe' la dimensione in byte del file prima della compressione.
 *	C:= linearizzazione dell'abero di huffman scritto mediante l'apposita funzione
 *	Parametri:
 *	path			- Stringa contenente il percorso del file
 * */
void Huffman::writeHeader(string& path){

	string new_path = removeExtension(path) + '.' + COMPRESS_EXTRENSION;
	oBitStream writer(new_path,BUFFER_SIZE, ios::binary);							//Creo un bit-stream di output sul nuovo file

	for(auto c : extractExtension(path))											//Scrivo l'estenzione corrente nel file
		writer << c;
	writer << '\0';

	for(unsigned int i = 0; i < sizeof(unsigned long int); i++)						//Scrivo total_symbols nel file, diviendolo byte a byte
		writer << ( (char*)&total_symbols )[i];

	writeHuffmanTree(huffmanTree,&writer);											//Scrivo l'albero di huffman linearizzato
	writer << '\0';
}
/**
 * 	Descrizione:
 * Legge dal file fornito da un bit-stream di input l'header necessario alla decodifica del file. L'header si presenta nella seguente forma:
 * 	|  	   PARTIZIONE A      |    PARTIZIONE B	 |		     PARTIZIONE C			 |
 * 	+----+----+----+----+----+----+----+----+----+-----------------------------+----+
 * 	|char|char|....|char|'\0'| unsigned long int |     Albero di huffman ..... |'\0'|
 * 	+----+----+----+----+----+----+----+----+----+----------------------------+-----+
 * 	A:= Stringa contenente la vecchia estensione del file, puo' essere lunga a piacere.
 * 	B:= Valore contenente il numero totale di simboli codificati, cioe' la dimensione in byte del file prima della compressione.
 *	C:= linearizzazione dell'abero di huffman scritto mediante l'apposita funzione
 *	Parametri:
 *	reader			- Puntatore al bit-stream che legge dal file
 *	Ritorno:
 *	Ritorna una stringa contenente la vecchia estensione letta dell'header
 * */
string Huffman::readHeader(iBitStream* reader){
	char c;
	string old_extension;

	do{																			//Leggo l'estensione vecchia del file
		*reader >> c;
		old_extension += c;
	}while(c!='\0');

	for(unsigned int i = 0; i < sizeof(unsigned long int); i++){				//Leggo total_symbols
		*reader >> ( (char*)&total_symbols )[i];
	}

	huffmanTree = readHuffmanTree(reader);										//Leggo l'albero di huffman linearizzato.

	if(reader->getSeekByte() !='\0')											//Se dopo la lettura dell'albero, il seek del buffer
		reader->nextByte();														//Non si trova sul terminatore allora passo al prossimo byte

	reader->nextByte();															//Sposto avanti il seek del buffer in modo da spostarlo
	*reader >> c;																//Sul blocco di byte da leggere
	return old_extension;
}
/**
 * 	Descrizione:
 * 	Comprime il file fornito in input tramite path effettuando le seguenti operazioni:
 * 	a) Calcola la frequenza dei simboli con symbolsFrequency().
 * 	b) Costruisco l'albero di huffman e genero i codici con buildHuffmanTree() e generateCodes().
 * 	c) Genero e scrivo l'header sul file con writeHeader().
 * 	d) Per ogni simbolo letto dal file da comprimere, scrivo sul file compresso il corrispettivo codice di huffman
 * 	Parametri:
 *	path			- Stringa contenente il percorso del file
 * */
void Huffman::compress(string path){
	symbolsFrequency(path);														//Calcolo la frequenza dei simboli
	buildHuffmanTree();															//Costruisco l'albero e genero i codici
	generateCodes();
	writeHeader(path);															//Scrivo l'header
	string new_path = removeExtension(path) + '.' + COMPRESS_EXTRENSION;

	{	//Nuovo scope per sfruttare i distruttori degli stream, in modo da stampare le statistiche.
		iBitStream reader(path,BUFFER_SIZE,ios::binary);							//Stream per la lettura
		oBitStream writer(new_path, BUFFER_SIZE, ios::app | ios::binary);			//Stream per la scrittura, app
																					//per scrivere dopo l'header
		char c;
		while(reader >>  c){														//Per ogni simbolo letto dal buffer
			code& codice = huffmanCodes[ (byte)c ];									//Associo il relativo codice di huffman e lo scrivo sul file
			for(bool bit : codice)	writer << bit;
		}
	}

	printStats(path,new_path);
}
/**
 * 	Descrizione:
 * 	Decomprime il file fornito in input tramite path effettuando le seguenti operazioni:
 * 	a) Legge l'header del file ricreando cosi' l'albero di huffman e ottenendo la vecchia estensione.
 * 	b) Legge bit a bit il file tramite i quali percorre l'albero (1 -> figlio SX, 0 -> figlio DX) fino a che non si raggiunge
 * 	   una foglia, viene quindi scritto sul file il suo simbolo contenuto e si riparte poi a percorrere l'albero dalla radice.
 * 	Parametri:
 *	path			- Stringa contenente il percorso del file
 * */
void Huffman::decompress(string path){
	string old_extension;

	{ 	//Nuovo scope per sfruttare i distruttori degli stream, in modo da stampare le statistiche.
		iBitStream reader(path,BUFFER_SIZE, ios::binary);										 //BitStream di input
		old_extension = readHeader(&reader);												 	 //Leggo l'header e salvo la vecchia estensione
		oBitStream writer(removeExtension(path) + '.' + old_extension,BUFFER_SIZE, ios::binary); //BitStream di output

		HNode* pnt = huffmanTree;								//Si parte dalla radice
		bool bit;
		while(reader >> bit){									//Per ogni bit letto dallo stream...

			if(bit)												//Percorro l'albero, se il bit e' 1 -> figlio SX
				pnt = pnt->getLeft();
			else												//altrimenti se e' 1 -> figlio DX.
				pnt = pnt->getRight();

			if((pnt->getLeft() == 0 and pnt->getRight() == 0)){	//Se sono arrivato ad una foglia, scrivo il suo simbolo contenuto
				char c = (pnt->getC());							//E ricomincio l'esplorazione dell'albero dalla sua radice
				if(total_symbols-- > 0)				//Se ho scritto tutti i simboli originari, smetto di scrivere
					writer << c;					//L'ultimo byte infatti potrebbe contenere dei bit sporchi
					pnt = huffmanTree;
			}

		}

	}
	printStats(path,removeExtension(path) + '.' + old_extension);
}
/**
 * 	Descrizione:
 * 	Calcola l'entropia di shannon del file calcolata fra [0,1]
 * 	Parametri:
 *	path			- Stringa contenente il percorso del file
 *	Ritorno:
 *	Un valore compreso tra [0,1] rappresentante l'entropia del file.
 * */
long double Huffman::getEntropy(string path){
	if(freq_table == nullptr)										//Se la tavola delle frequenze è ancora vuota...
		symbolsFrequency(path);

    long double p = 0, h = 0;										//p e' la probabilita' frequentista di avere un certo simbolo i nel file
    																//h e' dove viene memorizzata l'entropia
    for(int i = 0; i< MAX_SYMBOLS; i++){
        p=(long double)freq_table[i]/(long double)total_symbols ;
        if(p != 0)
        h+=p*log2(p);
    }
	return -h/8;													//Dividendo per 8 si ottiene un valore compreso tra [0,1]
}

/**
 * 	Descrizione:
 * 	Rimuove l'estensione da una stringa path fornita in input
 * 	Parametri:
 *	path			- Stringa contenente il percorso del file
 *	Ritorno:
 *	Una stringa contenente il path senza estenzione
 * */
const string Huffman::removeExtension(const string& path) const{
/**Funzione che dato il path di un file, restituisce una stringa con estensione rimossa.*/
	size_t lastdot = path.find_last_of(".");		//Il tipo di ritorno di find_last_of e' size_t
	if (lastdot == string::npos)  return path;		//npos sta ad indicare "nessuna occorrenza" ed e' uguale al
													//massimo valore possibile per size_t (2^32-1) oppure (2^64-1)

	  //Ritorna la sottostringa che va da 0 all'ultimo punto escluso    [0,lastdot[
	  return path.substr(0, lastdot);
}
/**
 * 	Descrizione:
 * 	Estrae l'estensione da una stringa path fornita in input
 * 	Parametri:
 *	path			- Stringa contenente il percorso del file
 *	Ritorno:
 *	Una stringa contenente l'estenzione del path fornito in input
 * */
const string Huffman::extractExtension(const string& path) const{
/**Funzione che dato il path di un file, restituisce l'estensione.*/
	size_t lastdot = path.find_last_of(".");		//Il tipo di ritorno di find_last_of e' size_t
	if (lastdot == string::npos) return "";			//npos sta ad indicare "nessuna occorrenza" ed e' uguale al
													//massimo valore possibile per size_t (2^32-1) oppure (2^64-1)

	//DA DOPO IL PUNTO FINO ALLA FINE
	return path.substr(lastdot+1);
}

void Huffman::printStats(string path1, string path2) const{
	fstream file1(path1.c_str());
	fstream file2(path2.c_str());

	file1.seekg(0,file1.end);
	file2.seekg(0,file2.end);

	unsigned long int size1 =	file1.tellg();
	unsigned long int size2 =	file2.tellg();
	cout << endl;
	cout << std::fixed << "Dimensioni del file prima dell'elaborazione:\t " << size1 << " Byte." << endl;
	cout << std::fixed << "Dimensioni del file dopo dell'elaborazione:\t " << size2 << " Byte." << endl;
	cout << std::fixed << "Rapporto di compressione: " << ((double)size1-(double)size2)/size1*100<< "%"<<endl;
	cout << endl;
}
