#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
class FileReader
{
	std::vector<int> m_fileContents;
	
public:
	FileReader(char* filePath);

	~FileReader();

	std::vector<int> GetContents();
};