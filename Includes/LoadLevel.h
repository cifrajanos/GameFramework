#ifndef _LOADLEVEL_H_
#define _LOADLEVEL_H_

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
#include <ctime>
#include <cstdlib>


using namespace std;
using std::string;

class LoadLevel
{
	FILE *f;
	bool startparse;
	char getbricks[200];
	char nrlevel[100];
	char *pch,*pch2;

public:

	LoadLevel();
	virtual ~LoadLevel(	);

	void LoadBricks(int levelnr);
	void LoadBricks_Load(int levelnr);
	void RandomBricks(char without[]);
	void SaveLevel();

public:

	string currentlevel;
	string nextlevel;
	string		bricks;
	bool		Winner;

};

#endif // _LOADLEVEL_H_