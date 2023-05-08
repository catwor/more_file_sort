#include "Adapter.h"

void adapter::ReadNext()
{
	eof = file.eof();
	if (!eof)
		file >> elem;
}

void adapter::startRead(std::string filename)
{
	file.open(filename, std::ios::in);
	ReadNext();
	eor = eof;
}

void adapter::startWrite(std::string filename)
{
	file.open(filename, std::ios::out);
}

void adapter::Close()
{
	file.close();
}

void adapter::Copy(adapter& x)
{
	elem = x.elem;
	file << ' ' << elem;
	x.ReadNext();
	x.eor = x.eof || x.elem < elem;
}

void adapter::CopyRun(adapter& x)
{
	do {
		Copy(x);
	} while (!x.eor);
}