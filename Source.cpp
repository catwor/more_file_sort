#include "Adapter.h"
#include <Windows.h>
#include <time.h>
#include <string>

const int NUMBER_OF_FILES = 3;

void Create(std::string filename, int cnt) {
	std::ofstream file(filename);
	srand(time(NULL));
	for (int i = 0; i < cnt; ++i)
		file << ' ' << rand() % 100;
	file.close();
}

bool CheakFile(std::string filename) {
	std::ifstream file(filename);
	int x, y;
	bool cheak = true;
	file >> y;
	while (!file.eof() && cheak) {
		x = y;
		file >> y;
		cheak = x <= y;
	}
	file.close();
	return cheak;
}

void create_support(std::string* arr,int* t) {
	for (int i = 0; i < 2 * NUMBER_OF_FILES; ++i) {
		arr[i] = "file_" + std::to_string(i + 1) + ".txt";
		t[i] = i;
	}
}

int distribure(std::string sort_file, adapter* f, std::string* files_name,int* t) {
	adapter f0;
	f0.startRead(sort_file);
	for (int i = 0; i < NUMBER_OF_FILES; ++i)
		f[t[i]].startWrite(files_name[t[i]]);

	//считаем максимальное колличество используемых файлов
	int max_index=0;
	while (!f0.eof && max_index < NUMBER_OF_FILES) {
		f[t[max_index]].CopyRun(f0);
		++max_index;
	}

	//если все файлы используются, и остались еще подпоследовательности
	if (max_index == NUMBER_OF_FILES) {
		int i = 0;
		while (!f0.eof) {// равномерность
			if (f0.elem >= f[t[i]].elem)//если подпоследовательность может быть продолжена
				f[t[i]].CopyRun(f0);
			if (!f0.eof)//обычная подпоследовательность
				f[t[i]].CopyRun(f0);
			i = (i + 1) % NUMBER_OF_FILES;
		}
	}

	f0.Close();
	for (int i = 0; i < NUMBER_OF_FILES; ++i)
		f[t[i]].Close ();

	return max_index;
}
//1 2 -1 4 3 5 -1 1 2 -6 7 2 8 -1 5 4 9 0 1 -1 1
int merge(std::string* files_name, int* t, adapter* f, int file_use) {
	for (int i = 0; i < NUMBER_OF_FILES;++i) {
		f[t[i]].startRead(files_name[t[i]]);
		f[t[i + NUMBER_OF_FILES]].startWrite(files_name[t[i + NUMBER_OF_FILES]]);
	}
	int k = file_use;
	file_use = 0;
	int i = 0;
	while (k > 1) {
		int k1 = k;
		while (k1 > 1) {
			int min = 0;
			for (int j = 1; j < k1; ++j)
				if (f[t[min]].elem > f[t[j]].elem)
					min = j;
			f[t[NUMBER_OF_FILES+i]].Copy(f[t[min]]);
			if (f[t[min]].eof) {
				std::swap(t[min], t[--k]);
				--k1;
			}
			else
				if (f[t[min]].eor)
					std::swap(t[min], t[--k1]);
		}
		if (!f[t[0]].eor)
			f[t[NUMBER_OF_FILES+i]].CopyRun(f[t[0]]);
		for (int j = 0; j < NUMBER_OF_FILES; ++j)
			if (!f[t[j]].eof)
				f[t[j]].eor = false;
		++i;
		if (i > file_use)
			file_use = i;
		i %= NUMBER_OF_FILES;
	}

	if (!f[t[0]].eor) {
		f[t[NUMBER_OF_FILES + i]].CopyRun(f[t[0]]);
		++file_use;
	}

	for (int i = 0; i < NUMBER_OF_FILES; ++i) {
		f[t[i]].Close();
		f[t[i + NUMBER_OF_FILES]].Close();
	}

	return file_use> NUMBER_OF_FILES ? NUMBER_OF_FILES : file_use;
}

void sort_more(std::string filename) {
	std::string mas_name[2 * NUMBER_OF_FILES];
	int t[2 * NUMBER_OF_FILES];
	create_support(mas_name,t);
	adapter* f = new adapter[2 * NUMBER_OF_FILES];
	int file_use = distribure(filename, f, mas_name, t);
	do {
		file_use = merge(mas_name, t, f, file_use);
		for (int i = 0; i < NUMBER_OF_FILES; ++i)
			std::swap(t[i],t[i+NUMBER_OF_FILES]);
	} while (file_use != 1);
	adapter rez;

	rez.startWrite("result.txt");
	f[t[0]].startRead(mas_name[t[0]]);
	rez.CopyRun(f[t[0]]);
	rez.Close();
	f[t[0]].Close();

	for (int i = 0; i < 2 * NUMBER_OF_FILES; ++i)
		remove(mas_name[i].c_str());
	std::cout << "ready";
}

int main() {
	std::string sort_file("sorting.txt");
	//Create(sort_file, 10);
	sort_more(sort_file);
	std::cin.get();
	return 0;
}