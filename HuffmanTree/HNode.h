#ifndef HNODE_H_
#define HNODE_H_

/* Classe 'HNode' -> Huffman Node, definisce in modo generico la struttura di un nodo dell'albero di huffman*/
class HNode {
public:
	/**COSTRUTTORI*/
	HNode(unsigned long int frequency=0) : frequency(frequency){}					//Costruttore con parametri.

	/**DISTRUTTORE*/
	virtual ~HNode(){}																//Distruttore.
	void freeTreeMemory(HNode* root);												//Funzione ricorsiva, libera la memoria di un albero di nodi.

	/**METODI GETTER & SETTER*/
	int getFrequency() const {return frequency;}									//Metodo getter, restituisce la frequenza del nodo.
	virtual HNode* getLeft() {return nullptr;}										//Metodo getter, restituisce un puntatore al figlio sinistro.
	virtual HNode* getRight() {return nullptr;}										//Metodo getter, restituisce un puntatore al figlio destro.
	virtual unsigned char getC() {return 0;}										//Metodo getter, restituisce un il simbolo memorizzato nel nodo.

private:
	/**ATTRIBUTI PRIVATI*/
	unsigned long int frequency;													//Frequenza del simbolo

};

#endif /* NODE_H_ */
