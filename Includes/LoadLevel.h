#ifndef _LOADLEVEL_H_
#define _LOADLEVEL_H_

#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<string>
#include<iostream>

using namespace std;
using std::string;

class LoadLevel
{
	FILE *f;
	int startparse;
	char getbricks[200];
	char nrlevel[100];
	char *pch,*pch2;
	string currentlevel;
	string nextlevel;

public:
	
	LoadLevel();
    virtual ~LoadLevel();
	void LoadBricks(int levelnr);

public:

	string		bricks;
	int			Winner;

};

#endif // _LOADLEVEL_H_