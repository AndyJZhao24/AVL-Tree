#ifndef __MY_AVL_TREE_HPP
#define __MY_AVL_TREE_HPP

#include "runtimeexcept.hpp"
#include <string>
#include <vector>

class ElementNotFoundException : public RuntimeException 
{
public:
	ElementNotFoundException(const std::string & err) : RuntimeException(err) {}
};

template<typename Key,  typename Value>
struct Node
{
	Key key;
	Value value;
	Node<Key, Value>* left; //left child
	Node<Key, Value>* right; //right child
	int height; //Height from current node to lowest leaf node
	Node<Key, Value>* parent; //parent of current node, root's parent will be null though.
	Node(const Key& x, const Value& y) : key(x), value(y), left(nullptr), right(nullptr), height(0), parent(nullptr){}
	~Node() //Node destructor, resembles recursion in that the destructor for the node gets called as well.
	{
		delete left;
		delete right;
	}
};

template<typename Key, typename Value>
class MyAVLTree
{
private:
	Node<Key, Value>* root; //Keeps track of root node.
	int counter; //Counter of how many nodes are in the tree
	int updateHeight(Node<Key,Value>* n); //Updates the height of every node in the tree
	void rightRotate(Node<Key,Value>* z); //Right rotation
	void leftRotate(Node<Key,Value>* z); //Left rotation
	void rebalance(Node<Key,Value>* z); //Starting from recently inserted, find first unbalanced, then balance
	void rInOrder(Node<Key,Value>* n, std::vector<Key>& key_vector) const;
	void rPreOrder(Node<Key,Value>* n, std::vector<Key>& key_vector) const;
	void rPostOrder(Node<Key,Value>* n, std::vector<Key>& key_vector) const;

public:
	MyAVLTree();

	~MyAVLTree()
	{
		delete root; //Calls the destructor for root node, since node has a destructor, it will also destruct every node in the tree(similar to recursion).
	}

	// size() returns the number of distinct keys in the tree.
	size_t size() const noexcept;

	// isEmpty() returns true if and only if the tree has no values in it. 
	bool isEmpty() const noexcept;

	// contains() returns true if and only if there
	//  is a (key, value) pair in the tree
	//	that has the given key as its key.
	bool contains(const Key & k) const; 

	// find returns the value associated with the given key
	// If !contains(k), this will throw an ElementNotFoundException
	// There needs to be a version for const and non-const MyAVLTrees.
	Value & find(const Key & k);
	const Value & find(const Key & k) const;

	// Inserts the given key-value pair into 
	// the tree and performs the AVL re-balance
	// operation, as described in lecture. 
	void insert(const Key & k, const Value & v);


	// The following three functions all return
	// the set of keys in the tree as a standard vector.
	// Each returns them in a different order.
	std::vector<Key> inOrder() const;
	std::vector<Key> preOrder() const;
	std::vector<Key> postOrder() const;


};

template<typename Key, typename Value>
MyAVLTree<Key,Value>::MyAVLTree()
{
	root = nullptr; //When tree is initialized, root is null
	counter = 0; //No nodes in the tree yet.
}


template<typename Key, typename Value>
int MyAVLTree<Key,Value>::updateHeight(Node<Key, Value>* n)
{

	if(n == nullptr) //If n is a nullptr, then its height should be -1
		return -1;
	else
	{
		int LHeight = updateHeight(n->left);
		int RHeight = updateHeight(n->right);
		n->height = ((LHeight>RHeight) ? LHeight : RHeight) + 1; //max(height of leftside, height of right side) + 1
		return  n->height; 
	}
}

template<typename Key, typename Value>
void MyAVLTree<Key,Value>::rightRotate(Node<Key, Value>* z)
{
	Node<Key,Value>* y = z->right; //y is the unbalanced node's right child.
	z->right = y->left; //z's right child becomes y's left child
	if(z->right != nullptr) //If z's right child is not a nullptr
		z->right->parent = z; //That child's parent is now z.

	y->left = z; //y's left is now z, since y was the "middle node" in terms of comparing the size of keys.
	y->parent = z->parent;  //y's parent becomes what z's parent was.
	z->parent = y; //Since z is now y's left child, z's parent is y.

	if(y->parent == nullptr) //If y's parent is a nullptr, that means y is the root since the only node with a nullptr as its parent is the root.
		root = y;
	else //If y is not the root
	{
		if(y->key > y->parent->key) //If y's key is greater than the parent's key, then y is the parent's right child
		{
			y->parent->right = y;
		}
		else //y's key is less than the parent's key, so y is the parent's left child.
		{
			y->parent->left = y;
		}
	}

}

template<typename Key, typename Value>
void MyAVLTree<Key,Value>::leftRotate(Node<Key, Value>* z)
{
	Node<Key,Value>* y = z->left; //y the unbalanced node's left child.

	z->left = y->right; //z's left child becomes y's left child
	if(z->left != nullptr) //If z's left child is not a nullptr
		z->left->parent = z; //Then that child's parent is now z.

	y->right = z; //y's right child is now z.
	y->parent= z->parent; //y's parent is z's parent.
	z->parent = y; //Since z is now y's right child, z's parent should be y

	if(y->parent == nullptr) //If y's parent is a nullptr, that means y is the root since the only node with a nullptr as its parent is the root.
		root = y;
	else //If y is not the root
	{
		if(y->key > y->parent->key) //If y's key is greater than it's parent's key, then y is the parent's right child.
		{
			y->parent->right = y;
		}
		else //y is less than the parent's key, so y is the parent's left child.
		{
			y->parent->left = y;
		}
	}

}

template<typename Key, typename Value>
void MyAVLTree<Key, Value>::rebalance(Node<Key, Value>* z)
{
	Key key = z->key; //Save Key of Inserted Node, this is the node that was most recently inserted into the AVL tree.
	while(z != root)
	{
		z = z->parent; //Move up one node, will be used to compare heights of left and right child.
		int balance = ((z->left != nullptr) ? z->left->height : -1) - ((z->right != nullptr) ?  z->right->height : -1); //if balance is negative, right is higher;
		if(balance > 1 || balance < -1)  //If unbalance, balance it
		{
			//If LeftLeft. Since balance is > 1, that means the unbalance is along the left side of z. since key(most recently inserted node) is less than z's left child's key, this is leftleft.
			if(balance > 1 && key < z->left->key) 
			{
				leftRotate(z);
				break;
			}
			//LeftRight. Since balance is > 1, that means the unbalance is along the left side of z. since key(most recently inserted node) is greater than z's left child's key, this is leftright.
			else if(balance > 1 && key > z->left->key)
			{
				rightRotate(z->left);
				leftRotate(z);
				break;
			}
			//RightRight. Since balance is < -1, that means the unbalance is along the right side of z. since key(most recently inserted node) is greater than z's right child's key, this is rightright.
			else if(balance < -1 && key > z->right->key) 
			{
				rightRotate(z);
				break;
			}
			//RightLeft. Since balance is < -1, that means the unlanace is along the right side of z, since key(most recently insereted node) is less than z's right child's key, this is right, left
			//balance < -1 && key < z->right->key
			else
			{
				leftRotate(z->right);
				rightRotate(z);
				break;
			}
		}
	}
}

template<typename Key, typename Value>
size_t MyAVLTree<Key, Value>::size() const noexcept
{
	return counter;
}

template<typename Key, typename Value>
bool MyAVLTree<Key, Value>::isEmpty() const noexcept
{
	return counter == 0;
}


template<typename Key, typename Value>
bool MyAVLTree<Key, Value>::contains(const Key &k) const
{
	Node<Key, Value>* currentNode = root; //Start searh at root.

	while(currentNode != nullptr)
	{
		if(currentNode->key == k) //Key found
		{
			return true;
		}
		else if(currentNode->key > k) //Node's key is greater than target, target must be on left side if exists.
		{
			currentNode = currentNode->left;
		}
		else //Node's key is less than target, target must be on right side if exists.
		{
			currentNode = currentNode->right;
		}
	}

	return false; //Reached a nullptr meaning target was not found.
}

template<typename Key, typename Value>
Value & MyAVLTree<Key, Value>::find(const Key & k)
{
	if(!contains(k)) //Target not in tree.
		throw ElementNotFoundException("Element not in MyAVLTree");

	Node<Key, Value>* currentNode = root; //Start at root
	while(currentNode != nullptr)
	{
		if(currentNode->key == k) //Key was found
		{
			break;
		}

		else if(currentNode->key > k) //Node's key is greater than target, target must be on left side if exists.
			currentNode = currentNode->left;

		else //Node's key is less than target, target must be on right side if exists.
			currentNode = currentNode->right;
	}
	return currentNode->value;
}

template<typename Key, typename Value>
const Value & MyAVLTree<Key, Value>::find(const Key & k) const
{
	if(!contains(k)) //Target not in tree.
		throw ElementNotFoundException("Element not in MyAVLTree");

	Node<Key, Value>* currentNode = root; //Start at root
	while(currentNode != nullptr)
	{
		if(currentNode->key == k) //Key was found
		{
			break;
		}

		else if(currentNode->key > k) //Node's key is greater than target, target must be on left side if exists.
			currentNode = currentNode->left;

		else //Node's key is less than target, target must be on right side if exists.
			currentNode = currentNode->right;
	}
	return currentNode->value;
}

template<typename Key, typename Value>
void MyAVLTree<Key, Value>::insert(const Key & k, const Value & v)
{
	Node<Key, Value>* insertedNode;
	if(root == nullptr) //Tree is empty
	{
		root = new Node<Key, Value>(k, v); counter++; return; 
	}
	else if(contains(k)) //If tree contains the key already
	{
		return;
	}
	else //Key is not in the tree
	{
		Node<Key, Value>* current = root; //Start at root

		while(true) //Actually Inserting
		{
			if(current->key > k) //If key is greater than key to insert
			{
				if(current->left == nullptr) //If left child is nullptr
				{
					current->left = new Node<Key, Value>(k, v); //insert new node there, replacing nullptr.
					counter++; //Tree has one more node.
					insertedNode = current->left; //Node that was just inserted
					insertedNode->parent = current; //Inserted Node's parent pointer is to the current node
					break;
				}
				current = current->left; //Since current node's key is greater than key to insert, traverse to the left node.
			}
			else //If key is less than key to insert
			{
				if(current->right == nullptr) //If right child is nullptr
				{
					current->right = new Node<Key, Value>(k, v); //insert new node, replacing nullptr.
					counter++; //Tree has one more node.
					insertedNode = current->right; //Node that was just inserted.
					insertedNode->parent = current; //Inserted Node's parent is the current node.
					break;
				}
				current = current->right; //Since current node's key is less than key to insert, traverse to the right node.
			}
		}

		updateHeight(root); //Updates Height of all nodes in the tree.
		//This  is where rebalancing happens
		rebalance(insertedNode); //Rebalance, passing the node that was just inserted.
		updateHeight(root); //After rebalancing, update the heights of all nodes again.
	}

}

template<typename Key, typename Value>
void MyAVLTree<Key, Value>::rInOrder(Node<Key, Value>* n,  std::vector<Key>& key_vector) const
{
	if(n == nullptr)
		return;
	else
	{
		rInOrder(n->left, key_vector);
		key_vector.push_back(n->key);
		rInOrder(n->right, key_vector);
	}
}

template<typename Key, typename Value>
void MyAVLTree<Key, Value>::rPreOrder(Node<Key, Value>* n, std::vector<Key>& key_vector) const
{
	if(n == nullptr)
		return;
	else
	{
		key_vector.push_back(n->key);
		rPreOrder(n->left, key_vector);
		rPreOrder(n->right, key_vector);
	}
}

template<typename Key, typename Value>
void MyAVLTree<Key, Value>::rPostOrder(Node<Key, Value>* n, std::vector<Key>& key_vector) const
{
	if(n == nullptr)
		return;
	else
	{
		rPostOrder(n->left, key_vector);
		rPostOrder(n->right, key_vector);
		key_vector.push_back(n->key);
	}
}

template<typename Key, typename Value>
std::vector<Key> MyAVLTree<Key, Value>::inOrder() const
{
	std::vector<Key> key_vector;
	rInOrder(root, key_vector);
	return key_vector;
}


template<typename Key, typename Value>
std::vector<Key> MyAVLTree<Key, Value>::preOrder() const
{
	std::vector<Key> key_vector;
	rPreOrder(root, key_vector);
	return key_vector; 
}


template<typename Key, typename Value>
std::vector<Key> MyAVLTree<Key, Value>::postOrder() const
{
	std::vector<Key> key_vector;
	rPostOrder(root, key_vector);
	return key_vector; 
}

#endif