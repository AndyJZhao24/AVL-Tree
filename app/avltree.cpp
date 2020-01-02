#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

#include "avltree.hpp"

std::string keepOnlyLetters(std::string s);


void countWords(std::istream & in, MyAVLTree<std::string, unsigned> & counter)
{
	std::string line, word;
	std::stringstream ss;

	while(	getline(in, line) )
	{
		ss.clear();
		ss << line;
		while( ss >> word )
		{
			word = keepOnlyLetters(word);
			if( word != "")	// "" means no letters got retained. 
			{
				if(counter.contains(word))
				{
					counter.find(word)++;
				}
				else
				{
					counter.insert(word, 1);
				}
			}
			else
			{
				std::cout << "Skipped because found something like a number or special characters." << std::endl; 
			}
		}
	}

}


std::string keepOnlyLetters(std::string s)
{
	std::string ret = "";
	for (size_t i=0; i < s.length(); i++)
	{
		if ( isalpha(s[i]) ) // isalpha() checks if this is a letter.
		{
			ret += std::tolower( s[i] );
		}

	}
	return ret;
}