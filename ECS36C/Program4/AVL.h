#ifndef _AVL_H_
#define _AVL_H_

#include <iostream>
#include <string>
#include <memory>
#include <queue>

#include "json.hpp"
using namespace nlohmann;
using namespace std;


class AVLTree;

class AVLNode {
private:
    int key_;
    int bf_;   // balance factor
    int height_; // node height
    weak_ptr<AVLNode> parent_;
    shared_ptr<AVLNode> left_;
    shared_ptr<AVLNode> right_;

 public:
    AVLNode(int key);
    AVLNode(int key, weak_ptr<AVLNode> pNode);

    // Replace the current node's lchild with newNode
    void ReplaceLChild(shared_ptr<AVLNode> cNode, shared_ptr<AVLNode> newNode);
    // Replace the current node's rchild with newNode
    void ReplaceRChild(shared_ptr<AVLNode> cNode, shared_ptr<AVLNode> newNode);

    friend AVLTree;
}; // class AVLNode

class AVLTree {
private:
    shared_ptr<AVLNode> root_;
    size_t size_;
public:
    AVLTree();

    // Insert key and keep AVL feature 
 	void Insert(int key);
    // Check and adjust AVL tree upward from the node
    void CheckBanlance(shared_ptr<AVLNode> node);
    // Update the height downward from the node 
    // return the node's height 
    int UpdateHeight(shared_ptr<AVLNode> node);
    // Output AVL tree information in JSON style
    string JSON() const;

    // Functions that used to achieve single and double rotations 
    // when adding left or right position node
    void L_Rotation(shared_ptr<AVLNode> cNode, shared_ptr<AVLNode> pNode);
    void R_Rotation(shared_ptr<AVLNode> cNode, shared_ptr<AVLNode> pNode);
    void LR_Rotation(shared_ptr<AVLNode> cNode, shared_ptr<AVLNode> pNode);
    void RL_Rotation(shared_ptr<AVLNode> cNode, shared_ptr<AVLNode> pNode);



}; // class AVLTree

#endif  // _AVL_H_



