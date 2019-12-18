#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "proj3.hpp"


// If you prefer to develop here, this is one way to do it.

void test1()
{
	MyAVLTree<std::string, unsigned> tree; 
	std::fstream fs;
	fs.open("gtest/hamletact1.txt");
	countWords(fs, tree);
	// do things here if you want.
}

void test2()
{
	MyAVLTree<int, std::string> tree;
	tree.insert(5, "Mike");
	bool b1 = tree.contains(5) && tree.find(5) == "Mike";
	if( b1 )
	{
		std::cout << "test2() passes." << std::endl; 
	}
	else
	{
		std::cout << "test2() fails." << std::endl; 
	}
}

int main()
{

	int choice;
	std::cin >> choice;
	switch(choice)
	{
		case 1: test1(); break;
		case 2: test2(); break;
		default: std::cout << "Not a choice.\n" << std::endl; break; 
	}
    return 0;
}

