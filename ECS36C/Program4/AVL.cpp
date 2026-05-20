#include "AVL.h"

// AVLNode
AVLNode::AVLNode(int key) :
	key_(key),
	bf_(0),
	height_(0),
	parent_(weak_ptr<AVLNode>()),
	left_(nullptr),
	right_(nullptr) {
}

AVLNode::AVLNode(int key, weak_ptr<AVLNode> pNode) :
	key_(key),
	bf_(0),
	height_(0),
	parent_(pNode),
	left_(nullptr),
	right_(nullptr) {
}

void AVLNode::ReplaceLChild(shared_ptr<AVLNode> cNode, shared_ptr<AVLNode> newNode) {
	cNode->left_ = newNode;
	newNode->parent_ = cNode;

}

void AVLNode::ReplaceRChild(shared_ptr<AVLNode> cNode, shared_ptr<AVLNode> newNode) {
	cNode->right_ = newNode;
	newNode->parent_ = cNode;

}


// AVLTree
AVLTree::AVLTree() : 
	root_(nullptr), 
	size_(0) {
}

// cNode: current node
// pNode: parent node
// gNode: grandparent node
// cNodeL: left child of current node
// cNodeR: right child of current node
void AVLTree::Insert(int key) {
	size_++;

	if (root_ == nullptr) {
		root_ = make_shared<AVLNode> (key);
		return;
	}

	// The location of the current node and its parent
	shared_ptr<AVLNode> currentNode = root_, parentNode = nullptr;

	// Finding the insert location
	while (currentNode != nullptr) {
		parentNode = currentNode;
		if (key < currentNode->key_) {
			currentNode = currentNode->left_;
		}
		else {
			currentNode = currentNode->right_;
		}
	}

	// Construct and insert current node
	currentNode = make_shared<AVLNode>(key, parentNode);
	if (currentNode->key_ < parentNode->key_) {
		parentNode->left_ = currentNode;
	} else {
		parentNode->right_ = currentNode;
	}

	// check if balance
	CheckBanlance(currentNode);
	// update heights
	UpdateHeight(root_);
	
}

void AVLTree::CheckBanlance(shared_ptr<AVLNode> cNode) {

	// parent node's bf and current node's bf
	int p_bf, c_bf;
	shared_ptr<AVLNode> pNode = cNode->parent_.lock();
	while (pNode != nullptr) {
		// Update the parent node's bf
		if (cNode == pNode->left_) {
			pNode->bf_--;
		}
		else {
			pNode->bf_++;
		}

		p_bf = pNode->bf_;
		if (p_bf == 0) {
			// If p_bf is 0, parent node's height do not change
			// it will no longer affect the bf of subsequent ancestors
			return ;
		}
		else if (p_bf == 1 || p_bf == -1) {
			// Change the parent node's height, 
			// we need to updated the upper layer's bf
			cNode = pNode;
			pNode = cNode->parent_.lock();
		}
		else {
			// If p_bf == 2 or p_bf == -2
			// We need to be adjust this AVL tree 
			// according to different situations
			c_bf = cNode->bf_;
			
			if (p_bf == 2 && c_bf == 1) {
				// Left Rotation
				L_Rotation(cNode, pNode);
			}
			else if (p_bf == -2 && c_bf == -1) {
				// Right Rotation
				R_Rotation(cNode, pNode);
			}
			else if (p_bf == -2 && c_bf == 1) {
				// Left Right Rotation
				LR_Rotation(cNode, pNode);
			}
			else {
				// 2, -1, Right Left Rotation
				RL_Rotation(cNode, pNode);
			}
			
			return ;
		}
	}
}

int AVLTree::UpdateHeight(shared_ptr<AVLNode> node) {
	int res;
	// leaf node
	if (node->left_ == nullptr && node->right_ == nullptr) {
		res = 0;
	}
	else if (node->left_ == nullptr) {
		res = UpdateHeight(node->right_) + 1;
	}
	else if (node->right_ == nullptr) {
		res = UpdateHeight(node->left_) + 1;
	}
	else {
		res = max(UpdateHeight(node->right_), UpdateHeight(node->left_)) + 1;
	}
	node->height_ = res;
	return res;
}


// Rotations
void AVLTree::L_Rotation(shared_ptr<AVLNode> cNode, shared_ptr<AVLNode> pNode) {
	shared_ptr<AVLNode> gNode = pNode->parent_.lock();
	shared_ptr<AVLNode> cNodeL = cNode->left_;

	// Rotate the current node to the root of the subtree
	if (gNode == nullptr) {
		root_ = cNode;
		cNode->parent_.reset();
	}
	else {
		if (gNode->left_ == pNode) {
			gNode->ReplaceLChild(gNode, cNode);
		}
		else {
			gNode->ReplaceRChild(gNode, cNode);
		}
	}

	// Rotate the current node's lchild to parent node
	pNode->right_ = cNodeL;
	if (cNodeL != nullptr)
		cNodeL->parent_ = pNode;

	// Rotate the parent node to current node
	cNode->ReplaceLChild(cNode, pNode);

	// Update bf
	cNode->bf_ = pNode->bf_ = 0;

}

void AVLTree::R_Rotation(shared_ptr<AVLNode> cNode, shared_ptr<AVLNode> pNode) {
	shared_ptr<AVLNode> gNode = pNode->parent_.lock();
	shared_ptr<AVLNode> cNodeR = cNode->right_;

	// Rotate the current node to the root of the subtree
	if (gNode == nullptr) {
		root_ = cNode;
		cNode->parent_.reset();
	}
	else {
		if (gNode->left_ == pNode) {
			gNode->ReplaceLChild(gNode, cNode);
		}
		else {
			gNode->ReplaceRChild(gNode, cNode);
		}
	}

	// Rotate the current node's rchild to parent node
	pNode->left_ = cNodeR;
	if (cNodeR != nullptr)
		cNodeR->parent_ = pNode;

	// Rotate the parent node to current node
	cNode->ReplaceRChild(cNode, pNode);

	// Update bf
	cNode->bf_ = pNode->bf_ = 0;
}

void AVLTree::LR_Rotation(shared_ptr<AVLNode> cNode, shared_ptr<AVLNode> pNode) {
	shared_ptr<AVLNode> cNodeR = cNode->right_;
	int LR_bf = cNodeR->bf_;

	// First do left rotation in current node
	L_Rotation(cNodeR, cNode);
	// Then right rotation in parent node
	R_Rotation(cNodeR, pNode);

	// Check the bf of cNodeR to update
	if (LR_bf == 0) {
		cNode->bf_ = pNode->bf_ = cNodeR->bf_ = 0;
	}
	else if (LR_bf == 1) {
		cNode->bf_ = -1;
		pNode->bf_ = cNodeR->bf_ = 0;
	}
	else {
		cNode->bf_ = cNodeR->bf_ = 0;
		pNode->bf_ = 1;
	}
}

void AVLTree::RL_Rotation(shared_ptr<AVLNode> cNode, shared_ptr<AVLNode> pNode) {
	shared_ptr<AVLNode> cNodeL = cNode->left_;
	int RL_bf = cNodeL->bf_;

	// First do right rotation in current node
	R_Rotation(cNodeL, cNode);
	// Then left rotation in parent node
	L_Rotation(cNodeL, pNode);

	// Check the bf of cNodeR to update
	if (RL_bf == 0) {
		cNode->bf_ = pNode->bf_ = cNodeL->bf_ = 0;
	}
	else if (RL_bf == 1) {
		cNode->bf_ = cNodeL->bf_ = 0;
		pNode->bf_ = -1;
	}
	else {
		cNode->bf_ = 1;
		pNode->bf_ = cNodeL->bf_ = 0;
	}
}


string AVLTree::JSON() const {
	json result;
	queue<shared_ptr<AVLNode> > nodes;
	if (root_ != nullptr) {
		nodes.push(root_);
		result["root"] = root_->key_;
		result["height"] = root_->height_;
		while (!nodes.empty()) {
			shared_ptr<AVLNode> node = nodes.front();
			nodes.pop();
			string key = to_string(node->key_);
			result[key]["height"] = node->height_;
			result[key]["balance factor"] = node->bf_;
			if (node->left_ != nullptr) {
				result[key]["left"] = node->left_->key_;
				nodes.push(node->left_);
			}
			if (node->right_ != nullptr) {
				result[key]["right"] = node->right_->key_;
				nodes.push(node->right_);
			}
			if (node->parent_.lock() != nullptr) {
				result[key]["parent"] = node->parent_.lock()->key_;
			} else {
				result[key]["root"] = true;
			}
		}
	}
	result["size"] = size_;
	return result.dump(2) + "\n";
}

