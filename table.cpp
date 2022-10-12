#include "table.h"
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

table::table(string diskname, int blocksize, int numberofblocks, string flatfile, string indexfile) : filesys(diskname, numberofblocks, blocksize){
	
	//initializing the variables
	this->flatfile = flatfile;
	this->indexfile = indexfile;

	//creating the file
	newfile(flatfile);
	newfile(indexfile);
}


int table::build_table(string input_file) {
	ifstream infile;
	ostringstream indexstream;
	ostringstream flatstream;


	infile.open(input_file.c_str());
	if (infile.bad()) { return 0; }
	string s;
	getline(infile, s);
	while (infile.good()) {
		//proccess line
		//write the record through the fat file and add the index record
		string key = s.substr(05);
		vector<string>blocks = block(s, getblocksize());
		int b = addblock(flatfile, blocks[0]);
		indexstream << key << ' ' << b << ' ';
		numrec++;
		string buffer = indexstream.str();
		vector<string>blocks2 = block(buffer, getblocksize());
		for (int x = 0; x < blocks2.size(); x++) {
			int b = addblock(indexfile, blocks2[x]);
		}

		getline(infile, s);
	}
	return 1;
}

int table::indexsearch(string value) {
	istringstream instream;
	int b = getfirstblock(indexfile);
	string bigbuffer;
	if (b <= 0) { return 0; }
	while (b != 0) {
		string buffer;
		int error = readblock(indexfile, b, buffer);
		bigbuffer += buffer;
		b = nextblock(indexfile, b);

	}
	//indexfile is a string in bigbuffer
	instream.str(bigbuffer);
	for (int x = 0; x < numrec; x++) {
		string s;
		int block;

		instream >> s >> block;
		if (value == s) { return block; }
	}
	return -1;
}

int table::search(string value) {
	int b = indexsearch(value);
	if (b <= 0) { return -1; }
	string buffer;
	readblock(flatfile, b, buffer);
	cout << "buffer" << endl;
	return b;

}