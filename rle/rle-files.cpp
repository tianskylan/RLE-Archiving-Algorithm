#include "rle-files.h"
#include "rle-algo.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <windows.h>

void RLE_v1::CreateArchive(const std::string& source)
{
	// Read in file and compress into array
	std::ifstream::pos_type size;
	char* memblock;
	std::ifstream file(source, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char[static_cast<unsigned int>(size)];
		file.seekg(0, std::ios::beg);
		file.read(memblock, size);
		file.close();
		// File data has now been loaded into memblock array
		m_Data.Compress(memblock, static_cast<int>(size));
		std::cout << "Compression finished." << std::endl;
		// Make sure to clean up!
		delete[] memblock;
	}
	else
	{
		std::cout << "Wrong file name provided. Try again." << std::endl;
		return;
	}

	//Creating output data
	std::string output_name = source;
	output_name.append(".rl1");
	std::cout << output_name << std::endl;

	//Store header information
	m_Header.sig[0] = 'R';
	m_Header.sig[1] = 'L';
	m_Header.sig[2] = 'E';
	m_Header.sig[3] = 0x01;

	m_Header.fileSize = static_cast<int>(size);

	m_Header.fileNameLength = source.length();

	m_Header.fileName = source;


	// Writing output file
	std::ofstream arc(output_name, std::ios::out | std::ios::binary | std::ios::trunc);
	if (arc.is_open())
	{
		// write the header
		arc.write(m_Header.sig, 4);
		arc.write(reinterpret_cast<char*>(&(m_Header.fileSize)), 4);
		arc.write(reinterpret_cast<char*>(&(m_Header.fileNameLength)), 1);
		arc.write(m_Header.fileName.c_str(), static_cast<int>(m_Header.fileNameLength));

		// write the compressed data of file content
		arc.write(m_Data.m_Data, m_Data.m_Size);

		float percent_compressed = 100 - (float)m_Data.m_Size * 100 / m_Header.fileSize;
		std::cout << "Compression rate : " << percent_compressed << "%" << std::endl;
	}
	else {
		std::cout << "Wrong file name provided. Try again." << std::endl;
		return;
	}
	arc.close();

}

void RLE_v1::ExtractArchive(const std::string& source)
{
	std::ifstream::pos_type size;
	char* memblock;
	std::ifstream file(source, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char[static_cast<unsigned int>(size)];
		file.seekg(0, std::ios::beg);
		file.read(memblock, size);
		file.close();
		// File data has now been loaded into memblock array

		for (int i = 0; i < 4; i++)
		{
			m_Header.sig[i] = memblock[i];
		}

		if (static_cast<int>(m_Header.sig[3]) == 2)
		{
			std::cout << "Wrong file type selected. Try again." << std::endl;
			return;
		}

		m_Header.fileSize = *(reinterpret_cast<int*>(&memblock[4]));
		m_Header.fileNameLength = memblock[8];

		const char *temp = &memblock[9];
		for (int i = 0; i < static_cast<int>(m_Header.fileNameLength); i++)
		{
			m_Header.fileName += *temp;
			temp++;
		}

		std::cout << m_Header.fileName << std::endl;

		m_Data.Decompress(temp,
			(static_cast<int>(size)-9 - static_cast<int>(m_Header.fileNameLength)),
			static_cast<int> (m_Header.fileSize));


		// Writing output file
		std::ofstream arc(m_Header.fileName, std::ios::out | std::ios::binary | std::ios::trunc);
		if (arc.is_open())
		{
			// write the compressed data of file content
			arc.write(m_Data.m_Data, m_Data.m_Size);
		}
		arc.close();
	}
	else
	{
		std::cout << "Wrong file name provided. Try again." << std::endl;
		return;
	}
}

void RLE_v2::CreateArchive(const std::string& source)
{
	//Generating the file list

	WIN32_FIND_DATA ffd;
	std::string path = source + "\\*.*";
	HANDLE hFind = FindFirstFile(path.c_str(), &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		// No files found, display error message and return!

		std::cout << "Wrong directory name provided. Try again." << std::endl;

		return;
	}
	// Add all files in this directory to a list of strings
	std::list<std::string> files;
	do
	{
		std::string temp = ffd.cFileName;
		if (temp != "." && temp != "..")
		{
			files.push_back(ffd.cFileName);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	std::cout << "File list in the directory has been generated."
		<< std::endl;
	// file list generation finished at this point

	std::ifstream::pos_type size;
	char* memblock;
	std::string path_and_name;
	file_info *temp;

	for (std::list<std::string>::iterator it = files.begin();
		it != files.end(); it++)
	{
		//Setting up the full path and name of the file
		path_and_name.clear();
		path_and_name = "letters/";
		path_and_name.append(*it);

		std::cout << path_and_name << std::endl;



		//Read in the uncompressed content
		std::ifstream file(path_and_name, std::ios::in | std::ios::binary | std::ios::ate);
		if (file.is_open())
		{
			size = file.tellg();
			memblock = new char[static_cast<unsigned int>(size)];
			file.seekg(0, std::ios::beg);
			file.read(memblock, size);
			file.close();
			// File data has now been loaded into memblock array

			temp = new file_info();

			temp->data.Compress(memblock, static_cast<int>(size));
			temp->fileSize = static_cast<int>(size);
			temp->compressedSize = temp->data.m_Size;
			temp->fileName = *it;
			temp->fileNameLength = (*it).length();

			m_Files.push_back(temp);

			std::cout << "Compression for file < " << path_and_name << " > finished." << std::endl;
			// Make sure to clean up!
			delete[] memblock;
		}
		else
		{
			std::cout << "Wrong file name provided. Try again." << std::endl;
			return;
		}
	}

	//Creating output data
	std::string output_name = source;
	output_name.append(".rl2");
	std::cout << output_name << std::endl;

	//Create overall header
	m_Header.sig[0] = 'R';
	m_Header.sig[1] = 'L';
	m_Header.sig[2] = 'E';
	m_Header.sig[3] = 0x02;

	m_Header.dirNameLength = source.length();

	m_Header.dirName = source;

	// Writing output file
	std::ofstream arc(output_name, std::ios::out | std::ios::binary | std::ios::trunc);
	if (arc.is_open())
	{
		// write the overall header
		arc.write(m_Header.sig, 4);
		arc.write(reinterpret_cast<char*>(&(m_Header.dirNameLength)), 1);
		arc.write(m_Header.dirName.c_str(), static_cast<int>(m_Header.dirNameLength));

		// write the file info for each file
		for (std::list<file_info*>::iterator it = m_Files.begin();
			it != m_Files.end(); it++)
		{
			arc.write(reinterpret_cast<char*>(&(*it)->fileSize), 4);
			arc.write(reinterpret_cast<char*>(&(*it)->compressedSize), 4);
			arc.write(reinterpret_cast<char*>(&(*it)->fileNameLength), 1);
			arc.write((*it)->fileName.c_str(), static_cast<int>((*it)->fileNameLength));
			arc.write((*it)->data.m_Data, (*it)->data.m_Size);
		}
	}
	else
	{
		std::cout << "Wrong file name provided. Try again." << std::endl;
		return;
	}
	arc.close();

}

void RLE_v2::ExtractArchive(const std::string& source)
{
	std::ifstream::pos_type size;
	char* memblock;
	std::ifstream file(source, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char[static_cast<unsigned int>(size)];
		file.seekg(0, std::ios::beg);
		file.read(memblock, size);
		file.close();
		// File data has now been loaded into memblock array

		for (int i = 0; i < 4; i++)
		{
			m_Header.sig[i] = memblock[i];
		}

		if (static_cast<int>(m_Header.sig[3]) == 1)
		{
			std::cout << "Wrong file type selected. Try again." << std::endl;
			return;
		}

		m_Header.dirNameLength = static_cast<int>(memblock[4]);
		
		char* temp = &memblock[5];

		for (int i = 0; i < static_cast<int>(m_Header.dirNameLength); i++)
		{
			m_Header.dirName += *temp;
			temp++;
		}

		while (temp < &memblock[static_cast<int>(size)]) {

			file_info *info = new file_info();

			info->fileSize = *(reinterpret_cast<int*>(temp));
			temp += 4;

			info->compressedSize = *(reinterpret_cast<int*>(temp));
			temp += 4;

			info->fileNameLength = static_cast<int>(*temp);
			temp++;

			for (int i = 0; i < static_cast<int>(info->fileNameLength); i++)
			{
				info->fileName += *temp;
				temp++;
			}

			info->data.Decompress(temp, info->compressedSize, info->fileSize);
			temp += info->compressedSize;

			m_Files.push_back(info);
		}




		


		// Create directory
		CreateDirectory(m_Header.dirName.c_str(), NULL);


		// Writing output file
		for (std::list<file_info*>::iterator it = m_Files.begin();
			it != m_Files.end(); it++)
		{
			std::ofstream arc((m_Header.dirName + "\\" +(*it)->fileName) , std::ios::out | std::ios::binary | std::ios::trunc);
			if (arc.is_open())
			{
				// write the compressed data of file content
				arc.write((*it)->data.m_Data, (*it)->data.m_Size);
			}
			arc.close();
		}
	}
	else
	{
		std::cout << "Wrong file name provided. Try again." << std::endl;
		return;
	}
}

RLE_v2::~RLE_v2()
{
	for (std::list<file_info*>::iterator it = m_Files.begin();
		it != m_Files.end(); it++)
	{
		delete *it;
	}
}
