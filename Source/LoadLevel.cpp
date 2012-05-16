#include "LoadLevel.h"

LoadLevel::LoadLevel()
{

}

LoadLevel::~LoadLevel()
{

}

void LoadLevel::LoadBricks(int levelnr)
{
	bricks="";	
	startparse=0;
	currentlevel="Level";
	nextlevel="Level";


	itoa(levelnr,nrlevel,10);
	currentlevel+=nrlevel;
	levelnr++;
	itoa(levelnr,nrlevel,10);
	nextlevel+=nrlevel;


	f=fopen("level.txt","r");
	while(!feof(f))
	{
		fscanf(f,"%s ",&getbricks);
		pch=strstr(getbricks,currentlevel.c_str());
		if(pch!=NULL)
		{
			startparse=1;
		}
		if(startparse==1)
		{
			pch=strstr(getbricks,nextlevel.c_str());
			pch2=strstr(getbricks,"END");
			if(pch!=NULL || pch2!=NULL)
			{
				if(pch2!=NULL)
				{
					Winner=1;
				}
				break;
			}
			pch=strstr(getbricks,"Level");
			if(pch==NULL){
			bricks+=getbricks;
			}
		}
	}
	fclose(f);
}
