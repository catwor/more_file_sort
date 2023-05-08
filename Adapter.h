#pragma once
#include <iostream>
#include <fstream>
#include <string.h>

class adapter
{
public:
	std::fstream file;
	int elem;
	bool eof;
	bool eor;
	void ReadNext();
	void startRead(std::string filename);
	void startWrite(std::string filename);
	void Close();
	void Copy(adapter& x);//копирование элемента из серии
	void CopyRun(adapter& x);//копирование серию
};