/*
 * INode.h
 *
 *  Created on: 08 dic 2015
 *      Author: Giuseppe
 */

#ifndef INODE_H_
#define INODE_H_

#include "HNode.h"

/* Classe 'INode' -> Internal Node, derivata dalla classe HNode, definisce appunto un nodo interno dell'albero di huffman
 * dotato di puntatori ai nodi figli e di un campo frequenza il cui valore e' pari alla somma delle frequenze nei nodi figli*/
class INode: public HNode {
public:
	/**COSTRUTTORE*/ //Creo un nodo che ha come figli a e b e come frequenza la somma delle frequenze.
	INode(HNode* a, HNode* b) : HNode(a->getFrequency()+b->getFrequency()), left(a), right(b) {};	//Costruttore con parametri.
	virtual ~INode();																				//Distruttore.

	/**METODI GETTER & SETTER*/
	HNode* getLeft() {return left;}																	//Metodo getter, restituisce un puntatore al figlio sinistro.
	HNode* getRight() {return right;}																//Metodo getter, restituisce un puntatore al figlio destro.

private:
	HNode* left;																					//Puntatore al figlio sinistro.
	HNode* right;																					//Puntatore al figlio destro.
};

#endif /* INODE_H_ */
