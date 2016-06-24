#include "HNode.h"

void HNode::freeTreeMemory(HNode* root){
	if(root==nullptr)
		return;
	freeTreeMemory(root->getLeft());
	freeTreeMemory(root->getRight());

	delete root;
}
