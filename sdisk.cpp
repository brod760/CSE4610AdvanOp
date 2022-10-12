#include "sdisk.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;


sdisk::sdisk(string diskname, int numofblocks, int blocksize) {
	//this part will manipulate the raw disk drive
	//if the file exist it will open it or else it will create it with numberofblocks*blocksize

	//this part creates the disk and sets the constructor
	this->diskname = diskname;
	this ->numofblocks = numofblocks;
	this ->blocksize = blocksize;

	fstream f;
	//this part should open the file if it is in the file
	f.open(diskname.c_str(), ios::in);
	if (f.good()) 
	{
		cout << "RAW DISK DRIVE OPEN" << endl;
	}
	else {
		cout << "Disk " << diskname << " does not exist" << endl;
		f.open(diskname.c_str(), ios::out);
		for (int x = 0; x < (blocksize * numofblocks); x++) {
			//this part signals the items onto the file
			f.put(' ');
		}
		cout << "New disk Created" << endl;

	}
	
	

}


int sdisk::getblock(int blocknumber, string& buffer) {
	//this part retreives blocknumber from the disk and stores it into buffer
	//returns 1 successful or else 0 if it isnt
	int temp = blocknumber * blocksize;
	fstream f;
	f.open(diskname.c_str(), ios::in);
	f.seekg(temp);

	if (f.good()) {
		for (int x = 0; x < blocksize; x++) {
			buffer += f.get();
		}
		f.close();
		return 1;
	}
	else {
		return 0;
	}

}
int sdisk::putblock(int blocknumber, string buffer) {
	//this part will simply write to the buffer 
	fstream f;
	f.open(diskname.c_str(), ios::in | ios::out);
	f.seekg(blocknumber * blocksize);
	int temp = buffer.length();

	if (f.good()) {
		for (int x = 0; x < temp && x < blocksize; x++) {
			f.put(buffer[x]);
		}
		f.close();
		cout << "Write to buffer is successful" << endl;
		return 1;

	}
	else {
		cout << "Write to buffer was nto successful" << endl;
		return 0;

	}


}

int sdisk::getblocksize() {
	return blocksize;
}
int sdisk::getnmberofblocks() {

	return numofblocks;
}