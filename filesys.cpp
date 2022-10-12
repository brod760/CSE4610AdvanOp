#include "filesys.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <fstream>
#include <string>
#include "sdisk.h"
#include <sstream>
using namespace std;

filesys::filesys(string diskname, int numberofblocks, int blocksize) : sdisk(diskname, numberofblocks, blocksize){
	//the fat we will be following will be a filesystem with 16 blocks
	string buffer;
	int code = getblock(0, buffer);
	if (code == 0) {
		cout << "This is a bad drive" << endl;
		return;
	}
	rootsize = (blocksize / 10);
	fatsize = (getnmberofblocks() * 5)/(getblocksize() + 1);
	//we have to build the root and the fat
	code = getblock(1, buffer);
	if (buffer[0] == '#') {
		//no file system
		buildfs();
	}
	else {
		readfs();
	}
	
	fssynch();
	cout << "it works" << endl;

}

vector<string> filesys::block(string buffer, int b)
{
	// blocks the buffer into a list of blocks of size b

	vector<string> blocks;
	int numberofblocks = 0;

	if (buffer.length() % b == 0)
	{
		numberofblocks = buffer.length() / b;
	}
	else
	{
		numberofblocks = buffer.length() / b + 1;
	}

	string tempblock;
	for (int i = 0; i < numberofblocks; i++)
	{
		tempblock = buffer.substr(b * i, b);
		blocks.push_back(tempblock);
	}

	int lastblock = blocks.size() - 1;

	for (int i = blocks[lastblock].length(); i < b; i++)
	{
		blocks[lastblock] += "#";
	}

	return blocks;

}

int filesys::fssynch() {
	//this part will write root and fat to the disk
	ostringstream ostream;
	
	for (int x = 0; x < rootsize; x++) {
		ostream << filename[x] << ' ' << firstblock[x] << ' ';
	}
	string buffer = ostream.str();
	
	vector <string>blocks = block(buffer, getblocksize());
	putblock(1, blocks[0]);

	ostringstream ostream2;
	for (int x = 0; x < fat.size(); x++) {
		ostream2 << fat[x] << " ";
	}
	string buffer2 = ostream2.str();
	vector<string> blocks2 = block(buffer2, getblocksize());
	
	for (int x = 0; x < blocks.size(); x++) {
		putblock(x + 2, blocks[x]);

	}
	return 1;

}

void filesys::readfs() {
	//this part is for the root
	istringstream istream;
	string buffer;
	getblock(1, buffer);
	istream.str(buffer);
	for (int x = 0; x < rootsize; x++) {
		string s;
		int t;
		istream >> s;
		istream >> t;
		filename.push_back(s);
		firstblock.push_back(t);

	}

	//this part is for the fat
	istringstream istream2;
	for (int x = 0; x < fatsize; x++) {
		string b;
		getblock(x + 2, b);
		buffer += b;

	}

	istream2.str(buffer);
	for (int x = 0; x < getnmberofblocks(); x++) {
		int t;
		istream2 >> t;
		fat.push_back(t);

	}

}

int filesys::rmfile(string file) {
	for (int x = 0; x < filename.size(); x++) {
		if (filename[x] == file && firstblock[x] == 0) {
			filename[x] = "XXXXXX";
			firstblock[x] = 0;
		}
	}
	fssynch();
	return 1;
}
int filesys::newfile(string file) {
	//returns 1 if successful or else it returns 0

	for (int x = 0; x < filename.size(); x++) {
		if (filename[x] == file) {
			cout << "File exist" << endl;
			return 0;
		}

	}
	for (int x = 0; x < filename.size(); x++) {

		if (filename[x] == "xxxx") {
			filename[x] = file;
			firstblock[x] = 0;
			fssynch();
			return 1;
		}
	}

	cout << "No room in root" << endl;
	return 0;
}

int filesys::getfirstblock(string file) {
//returns the first block of a file
	//return 0 f the file is empty or else returns -1 if file doesnt exist
	for (int x = 0; x < filename.size(); x++) {
		if (filename[x] == file) {
			return firstblock[x];
		}
		
	}
 	cout << "No such file exist " << endl;
	return -1;


}

int filesys::addblock(string file, string block) {
	int first = getfirstblock(file);

	if (first == -1) { cout << "No such file" << endl;
	return 0;}
	int allocate = fat[0];
	fat[0] = fat[fat[0]];
	fat[allocate] = 0;
	if (first == 0) {
		for (int x = 0; x < filename.size(); x++) {
			if (filename[x] == file) {
				firstblock[x] = allocate;
			}
			
		}
	
	}
	else {
		int b = first;
		while (fat[b] != 0) {

			b = fat[b];
		}
		fat[b] = allocate;
	}
	fssynch();
	putblock(allocate, block);
	return allocate;
}

bool filesys::checkblock(string file, int blocknumber) {
	int b = getfirstblock(file);
	if (b == -1) {
		return false;
	}
	while (fat[b]!= 0) {
		if (fat[b] == blocknumber) {
			return true;
			
		}
		b = fat[b];
		
	}
	return false;
}

int filesys::readblock(string file, int blocknumber, string& buffer) {
	if (checkblock(file, blocknumber)) {
		getblock(blocknumber, buffer);
		return 1;

	}
	else {

		return 0;
	}

}

int filesys::writeblock(string file, int blocknumber, string buffer) {
	if (checkblock(file, blocknumber)) {
		putblock(blocknumber, buffer);
		return 1;

	}
	else {

		return 0;
	}

}


int filesys::nextblock(string file, int blocknumber) {
	if (checkblock(file, blocknumber)) {
		return fat[blocknumber];

	}
	else {

		return 0;
	}

}
//CHECK THIS ONE LATER
int filesys::delblock(string file, int blocknumber) {
	if (!checkblock(file, blocknumber)) {
		return 0;

	}
	int b = getfirstblock(file);
	if (blocknumber == b) {
		for (int x = 0; x < filename.size(); x++) {
			if (filename[x] == file) {
				firstblock[x] = fat[blocknumber];
			}
		
		}
		
	}
	else {
		while (fat[b] != blocknumber) {
			b = fat[b];
		}
		fat[b] = fat[blocknumber];

	}

	fat[blocknumber] = fat[0];
	fat[0] = blocknumber;

}

void filesys::buildfs() {
	ostringstream ostream;
	for (int x = 0; x < rootsize; x++) {
		filename.push_back("xxxx");
		firstblock.push_back(0);
		ostream << "xxxx" << " " << 0 << " ";
	}
	string buffer = ostream.str();
	vector<string>blocks = block(buffer, getblocksize());
	putblock(1, blocks[0]);
	////////////////////////////////////////////////////////
	ostringstream ostream2;
	fat.push_back(fatsize + 2);
	ostream2 << fatsize + 2 << " ";
	fat.push_back(1);
	ostream2 << 1 << " ";
	for (int x = 0; x < fatsize; x++) {
		fat.push_back(1);
		ostream2 << 1 << " ";
	}
	for (int x = fatsize + 2; x < getnmberofblocks(); x++) {
		fat.push_back(x + 1);
		ostream2 << x + 1 << " ";
	}
	fat[fat.size() - 1] = 0;
	for (int x = 0; x < fat.size(); x++) {
		ostream2 << fat[x] << ' ';
	}

	string buffer2 = ostream2.str();
	vector<string> blocks2 = block(buffer2, getblocksize());

	for (int x = 0; x < blocks2.size(); x++) {
		putblock(2 + x, blocks2[x]);
	}



}


vector<string> filesys::ls()
{
	vector <string> flist;
	for (int i = 0; i < filename.size(); i++)
	{
		if (filename[i] != "XXXXX")
		{
			flist.push_back(filename[i]);
		}
	}
	return flist;
}