#include <sstream>
#include "FileReader.h"

#pragma warning(disable : 4996)

FileReader::FileReader(char* filePath)
{
	std::ifstream file;
	file.open(filePath);
	std::string str;

	if (file.is_open())
	{
		file >> str;
		std::istringstream stm(str);
		std::string temp;
		while (std::getline(stm, temp, ','))
		{
			m_fileContents.push_back(std::stoi(temp));
		}
	}
	file.close();
}

FileReader::~FileReader() {}

std::vector<int> FileReader::GetContents() { return m_fileContents; }