#pragma once
#ifndef SHELL_H
#define SHELL_H

#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "sdis
#include "filesys.h"
#include "table.h"

class shell : public filesys{
public:
	shell(string filename, int blocksize, int numberofblocks);
	int dir();
	int add(string file, string buffer);
	int del(string file);
	int type(string file);
	int copy(string file1, string file2);

};

#endif