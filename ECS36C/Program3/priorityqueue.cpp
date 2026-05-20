#include <iostream>
#include "json.hpp"

#include "priorityqueue.h"

PriorityQueue::PriorityQueue(std::size_t max_size) :
	nodes_(max_size + 1, KeyValuePair()), 
	max_size_(max_size), 
	size_(0) {
}

void PriorityQueue::insert(Key k) {
	insert(std::make_pair(k, std::make_pair(0, 0)));
}

void PriorityQueue::insert(KeyValuePair kv) {
	// TODO: complete this function
	
	size_++;
	nodes_[size_] = kv;
	heapifyUp(size_);
}

KeyValuePair PriorityQueue::min() {
	// TODO: complete this function
	
	return nodes_[ROOT];
}

KeyValuePair PriorityQueue::removeMin() {
	// TODO: complete this function
	
	KeyValuePair kv(nodes_[ROOT]);
	removeNode(ROOT);
	heapifyDown(ROOT);
	return kv;
}

bool PriorityQueue::isEmpty() const {
	// TODO: complete this function
	return size_ == 0;
}

size_t PriorityQueue::size() const {
	// TODO: complete this function
	return size_; 
}

nlohmann::json PriorityQueue::JSON() const {
	nlohmann::json result;
	for (size_t i = 1; i <= size_; i++) {
		nlohmann::json node;
		KeyValuePair kv = nodes_[i];
		node["key"] = kv.first;
		node["value"] = kv.second;
		if (i != ROOT) {
			node["parent"] = std::to_string(i / 2);
		}
		if (2 * i <= size_) {
			node["leftChild"] = std::to_string(2 * i);
		}
		if (2 * i + 1 <= size_) {
			node["rightChild"] = std::to_string(2 * i + 1);
		}
		result[std::to_string(i)] = node;
	}
	result["metadata"]["max_size"] = max_size_;
	result["metadata"]["size"] = size_;
	return result;
}

void PriorityQueue::heapifyUp(size_t i) {
	// TODO: complete this function
	
	if (i <= ROOT)
		return;
		
	size_t fa = i / 2; // father
	if (getKey(i) < getKey(fa)) {
		std::swap(nodes_[i], nodes_[fa]);
		heapifyUp(fa);
	}
	
}

void PriorityQueue::heapifyDown(size_t i) {
	// TODO: complete this function
	
	if (2 * i > size_)
		return;
	
	size_t lc = 2 * i, rc = lc + 1;
	// left child and right child
	size_t min_pos = lc;
	
	if (rc <= size_ && getKey(lc) > getKey(rc))
		min_pos = rc;
	if (getKey(i) > getKey(min_pos)) {
		std::swap(nodes_[i], nodes_[min_pos]);
		heapifyDown(min_pos);
	}	
	
}

void PriorityQueue::removeNode(size_t i) {
	// TODO: complete this function
	
	nodes_[i] = nodes_[size_];
	size_--;
}

Key PriorityQueue::getKey(size_t i) {
	// TODO: complete this function
	return nodes_[i].first;
}


