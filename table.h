#pragma once
#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "sdisk.h"
#include "filesys.h"
#include "shell.h"

using namespace std;

class table : public filesys{
public:
	table(string diskname, int blocksize, int numberofblocks, string flatfile, string indexfile);
	int build_table(string input_file);
	int search(string value);

protected:
	string flatfile;
	string indexfile;
	int numrec;	//number of records being displayed
	int indexsearch(string value);
	vector<string> list;


};
#endif

