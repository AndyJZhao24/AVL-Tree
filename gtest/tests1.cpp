#include "gtest/gtest.h"
#include "MyAVLTree.hpp"
#include "avltree.hpp"
#include <string>
#include <sstream>
#include <vector>

namespace{


TEST(BasicTreeFunctions, BasicTreeFunctions_FindTheRoot)
{
	MyAVLTree<int, std::string> tree;
	tree.insert(5, "foo");

	EXPECT_TRUE( tree.contains(5) );
}

TEST(BasicTreeFunctions, BasicTreeFunctions_FindOneHop)
{
	MyAVLTree<int, std::string> tree;
	tree.insert(5, "foo");
	tree.insert(10, "bar");

	EXPECT_TRUE( tree.contains(10) );
}

TEST(BasicTreeFunctions, BasicTreeFunctions_FindTwoHops)
{
	MyAVLTree<int, std::string> tree;
	tree.insert(5, "foo");
	tree.insert(3, "sna");
	tree.insert(10, "bar");
	tree.insert(12, "twelve");

	EXPECT_TRUE( tree.contains(12) );
}



TEST(BasicTreeFunctions, BasicTreeFunctions_Add5)
{
	MyAVLTree<int, std::string> tree;
	tree.insert(5, "foo");
	tree.insert(3, "sna");
	tree.insert(10, "bar");
	tree.insert(12, "twelve");
	tree.insert(15, "fifteen");

	EXPECT_TRUE( tree.size() == 5 );
}



TEST(FullImplementation, InOrderTraversal)
{
	MyAVLTree<int, std::string> tree;
	tree.insert(5, "foo");
	tree.insert(3, "sna");
	tree.insert(10, "bar");
	tree.insert(12, "twelve");
	tree.insert(15, "fifteen");

	std::vector<int> trav = tree.inOrder();
	std::vector<int> expected = {3, 5, 10, 12, 15};
	EXPECT_TRUE( trav == expected );
}



TEST(FullImplementation, JackSparrow)
{
	std::string quote = "I'm dishonest, and a dishonest man you can ";
	quote += "always trust to be dishonest. Honestly. It's the honest ";
	quote += "ones you want to watch out for, because you can never ";
	quote += "predict when they're going to do something incredibly... stupid.";

	std::istringstream stream( quote );

	MyAVLTree<std::string, unsigned> tree;

	countWords(stream, tree);
	EXPECT_TRUE(tree.find("dishonest") == 3);
}

}