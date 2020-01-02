#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "avltree.hpp"


void test1()
{
	MyAVLTree<std::string, unsigned> tree; 
	std::fstream fs;
	fs.open("gtest/hamletact1.txt");
	countWords(fs, tree);
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
	std::string line;
	std::getline(std::cin, line);

	std::istringstream stream(line);

	MyAVLTree<std::string, unsigned> tree;

	countWords(stream, tree);

	int choice = 0;
	while(choice != 4)
	{
		std::cout << "Enter 1 for Pre-Order Traversal, 2 for In-Order, 3 for Post-Order, or 4 to exit:";
		std::cin >> choice;
		switch(choice)
		{
			case 1:
				for(int i = 0; i < tree.preOrder().size(); i++)
				{
					std::cout << tree.preOrder()[i] << std::endl;
				}
				break;
			case 2:
				for(int i = 0; i < tree.inOrder().size(); i++)
				{
					std::cout << tree.inOrder()[i] << std::endl;
				}
				break;
			case 3:
				for(int i = 0; i < tree.postOrder().size(); i++)
				{
					std::cout << tree.postOrder()[i] << std::endl;
				}
				break;
			default:
				break;
		}
	}
    return 0;
}

