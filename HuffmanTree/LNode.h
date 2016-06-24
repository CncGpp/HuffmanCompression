#ifndef LNODE_H_
#define LNODE_H_

#include "HNode.h"

/* Classe 'LNode' -> Leaf Node, definisce un nodo foglia dell'albero di huffman, dotato cioe' oltre che di frequenza anche
 * del campo contenente il simbolo.*/
class LNode: public HNode {
public:
	/**COSTRUTTORI*/
	LNode(unsigned long int f, unsigned char c) : HNode(f), c(c){}		//Costruttore con parametri.
	/**DISTRUTTORE*/
	virtual ~LNode();													//Distruttore.
	/**METODI GETTER & SETTER*/
	unsigned char getC(){return c;}										//Metodo getter, restituisce un il simbolo memorizzato nel nodo.
	HNode* getLeft() {return nullptr;}									//Metodo getter, restituisce un puntatore al figlio sinistro.
	HNode* getRight() {return nullptr;}									//Metodo getter, restituisce un puntatore al figlio destro.
																		//Poiche' LNode e' un nodo fogli i due metodi restituiscono sempre nullptr.

private:
	unsigned char c;													//Simbolo memorizzato nel nodo foglia.
};

#endif /* LNODE_H_ */
