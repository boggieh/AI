#include "FileWriter.h"

FileWriter::FileWriter(char* filePath) { m_file = std::ofstream(filePath); }

FileWriter::~FileWriter() { m_file.close(); }

void FileWriter::WriteToFile(char content) { m_file << content; }

void FileWriter::WriteToFile(int content) { m_file << content; }