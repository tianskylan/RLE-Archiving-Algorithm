// rle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "rletest.h"
#include "rle-algo.hpp"
#include <iostream>
#include "rle-files.h"

using std::cout;
using std::endl;

void Part1Tests()
{
	std::cout << "Testing compression..." << std::endl;
	RLE<char> r;
	r.Compress(CHAR_1, strlen(CHAR_1));
	std::cout << r << std::endl;
	r.Compress(CHAR_2, strlen(CHAR_2));
	std::cout << r << std::endl;
	r.Compress(CHAR_3, strlen(CHAR_3));
	std::cout << r << std::endl;
	r.Compress(CHAR_4, strlen(CHAR_4));
	std::cout << r << std::endl << std::endl;
	std::cout << "Testing decompression..." << std::endl;
	r.Decompress(RLE_CHAR_1, strlen(RLE_CHAR_1), 40);
	std::cout << r << std::endl;
	r.Decompress(RLE_CHAR_2, strlen(RLE_CHAR_2), 12);
	std::cout << r << std::endl;
	r.Decompress(RLE_CHAR_3, strlen(RLE_CHAR_3), 14);
	std::cout << r << std::endl << std::endl;
}

void Part2Options()
{
	char option[256];

	do
	{
		cout << "1. Create an archive" << endl;
		cout << "2. Extract an archive" << endl;
		cout << "3. Return to previous level" << endl;

		std::cin.getline(option, 256);

		if (option[1] != '\0')
		{
			cout << "Wrong option. Try again." << endl << endl;
		}
		else if (*option == '1')
		{
			char filename[256];
			cout << "Please enter the target path and filename: " << endl;
			std::cin.getline(filename, 256);

			cout << "Creating an archive for file " << filename << endl;

			RLE_v1 single_file;
			single_file.CreateArchive(filename);
		}
		else if (*option == '2')
		{
			char filename[256];
			cout << "Please enter the target path and filename: " << endl;
			std::cin.getline(filename, 256);

			cout << "extracting an archive for file " << filename << endl;

			RLE_v1 single_file;
			single_file.ExtractArchive(filename);
		}
		else if (*option == '3')
		{
			cout << "Returning to previous menu..." << endl;
		}
	} while (*option != '3');
}

void Part3Options()
{
	char option[256];

	do
	{
		cout << "1. Create an archive" << endl;
		cout << "2. Extract an archive" << endl;
		cout << "3. Return to previous level" << endl;

		std::cin.getline(option, 256);

		if (option[1] != '\0')
		{
			cout << "Wrong option. Try again." << endl << endl;
		}
		else if (*option == '1')
		{
			char directoryName[256];
			cout << "Please enter the target directory: " << endl;
			std::cin.getline(directoryName, 256);

			cout << "Creating an archive for directory " << directoryName << endl;

			RLE_v2 directory;
			directory.CreateArchive(directoryName);
		}
		else if (*option == '2')
		{
			char directoryName[256];
			cout << "Please enter the target directory: " << endl;
			std::cin.getline(directoryName, 256);

			cout << "extracting an archive for directory " << directoryName << endl;

			RLE_v2 directory;
			directory.ExtractArchive(directoryName);
		}
		else if (*option == '3')
		{
			cout << "Returning to previous menu..." << endl;
		}
	} while (*option != '3');
}

void printMenu() 
{
	char option[256];

	do 
	{
		cout << "Select an option: " << endl;
		cout << "1. Part 1 (Tests)" << endl;
		cout << "2. Part 2 (Single Files)" << endl;
		cout << "3. Part 3 (Directories)" << endl;
		cout << "4. Exit" << endl;
		std::cin.getline(option, 256);

		// If the command is longer than 1 character, try again
		if (option[1] != '\0') 
		{
			cout << "Wrong option. Try again." << endl << endl;
		}

		// Option 1 - Part 1
		else if (*option == '1') 
		{
			Part1Tests();
		}
		else if (*option == '2') 
		{
			Part2Options();
		}
		else if (*option == '3')
		{
			Part3Options();
		}
		// Option 4 - Exit
		else if (*option == '4') 
		{
			cout << "Exiting..." << endl;
		}
		// Wrong option, try again
		else 
		{
			cout << "Wrong option. Try again." << endl << endl;
		}
	} while (*option != '4');


}

int _tmain(int argc, _TCHAR* argv[])
{
	// Main entry point!
	printMenu();
	return 0;
}

