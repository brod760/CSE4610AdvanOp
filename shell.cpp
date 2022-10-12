#include "shell.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "sdisk.h"
#include "filesys.h"
#include "table.h"

using namespace std;

shell::shell(string diskname, int blocksize, int numberofblocks) :filesys(diskname, numberofblocks, blocksize) {
}

int shell::add(string file, string buffer) {
	int error = newfile(file);
	if (error < 1) { 
		cout << "sorry" <<endl; 
		return 0;
	};
	vector<string>blocks = block(buffer, getblocksize());
	for (int x = 0; x < blocks.size(); x++) {
		addblock(file, blocks[x]);
	}
	return 1;
}

int shell::dir()
{
	vector<string> list = ls();
	for (int x = 0; x < list.size(); x++)
	{
		cout << list[x] << endl;
	}
	return 0;
}

int shell::del(string file) {
	int block = getfirstblock(file);
	while (block > 0) {
		delblock(file, block);
		block = getfirstblock(file);
		//I used the getfirstblock method because
		//in the lecture "firstblock(file) always 
		//produced an error on my end
	}
	rmfile(file);
	return 0;
}

int shell::copy(string file1, string file2) {
	string buffer;
	int block = getfirstblock(file1);
	if (block < 0) {
		cout << "no such file" << endl;
		return 0;
	}
	if (block == 0) {
		cout << "empty file" << endl;
		return 1;
	}

	while (block != 0) {
		string b;
			readblock(file1, block, b);
			addblock(file2, b);

			buffer += b;
		block = nextblock(file1, block);

		return 1;
	}

	return 0;
}

int shell::type(string file) {
	string buffer;
	int block = getfirstblock(file);
	vector<string>return_file;

	if (block < 0) {
		cout << "no such file" << endl;
		return 0;
	}
	if (block == 0) {
		cout << "empty file" << endl;
		return 1;
	}

	while (block != 0) {
		string b;
		readblock(file, block, b);
		return_file.push_back(b);

		buffer += b;
		block = nextblock(file, block);
	}
	for (int x = 0; x < return_file.size(); x++) {
		cout << return_file[x] << endl;
	}
	return 0;
}

