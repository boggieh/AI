#pragma once

#include <fstream>
class FileWriter
{
	std::ofstream m_file;

public:
	FileWriter(char* filePath);

	~FileWriter();

	void WriteToFile(char content);

	void WriteToFile(int content);
};

