#pragma once
#ifndef SDISK_H
#define SDISK_H

#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class sdisk
{
public:
	sdisk(string diskname, int numbofblocks, int blocksize);
	int getnmberofblocks();
	int getblocksize();		//accessor function
	int getblock(int blocknumber, string& buffer);
	int putblock(int blocknumber, string buffer);

private:
	string diskname;
	int numofblocks;
	int blocksize;
};

#endif
