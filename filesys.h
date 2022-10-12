#pragma once
#ifndef FILESYS_H
#define FILESYS_H

#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <fstream>
#include <string>
#include "sdisk.h"
using namespace std;

class filesys : public sdisk
{
public:
filesys(string diskname, int numberofblocks, int blocksize);
int fsclose();
int fssynch();
int newfile(string file);
int rmfile(string file);
int getfirstblock(string file);
int addblock(string file, string block);
int delblock(string file, int blocknumber);
int readblock(string file, int blocknumber, string& buffer);
int writeblock(string file, int blocknumber, string buffer);
int nextblock(string file, int blocknumber);
vector<string> block(string buffer, int b);

protected:
	int rootsize;           // maximum number of entries in ROOT
	int fatsize;            // number of blocks occupied by FAT
	vector<string>ls();
	vector<string> filename;   // filenames in ROOT
	vector<int> firstblock; // firstblocks in ROOT
	vector<int> fat;
	void readfs();
	void buildfs();
	bool checkblock(string filename, int blocknumber);
};

#endif


