#include "LoadLevel.h"

LoadLevel::LoadLevel()
{
	Winner=false;
}

LoadLevel::~LoadLevel()
{

}

void LoadLevel::LoadBricks(int levelnr)
{
	bricks = "";	
	startparse = false;
	currentlevel = "Level";
	nextlevel = "Level";


	itoa(levelnr,nrlevel,10);
	currentlevel += nrlevel;
	levelnr++;
	itoa(levelnr,nrlevel,10);
	nextlevel += nrlevel;


	f = fopen("level.txt","r");
	while(!feof(f))
	{
		fscanf(f,"%s ",&getbricks);
		pch = strstr(getbricks,currentlevel.c_str());
		if(pch != NULL)
		{
			startparse = true;
		}
		if(startparse == true)
		{
			pch = strstr(getbricks,nextlevel.c_str());
			pch2 = strstr(getbricks,"END");
			if(pch != NULL || pch2 != NULL)
			{
				if(pch2 != NULL)
				{
					Winner = true;
				}
				break;
			}
			pch = strstr(getbricks,"Level");
			if(pch == NULL){
				bricks += getbricks;
			}
		}
	}
	fclose(f);
}

void LoadLevel::RandomBricks(char without[])
{
	srand ( unsigned ( time (NULL) ) );
	int contor = 0;
	int contor2 = 0;
	char swapbrick;
	string bricksmask;
	bricksmask = bricks;
	string::iterator it;
	string::iterator it2;

	// using built-in random generator:
	random_shuffle ( bricks.begin(), bricks.end() );

	// keep position for specified type of bricks
	for(int i = 0; i < sizeof(without); i++)
		for(int j = 0; j < sizeof(without); j++)
		{
			contor = 0;
			for(it = bricksmask.begin(); it != bricksmask.end(); ++it)
			{	
				if(bricksmask[contor] != without[j] && bricks[contor] != without[j])
				{
					contor++;
					continue;
				}

				if(bricksmask[contor] == without[j] && bricks[contor] == without[j])
				{
					contor++;
					continue;
				}

				if(bricksmask[contor] != without[j] && bricks[contor] == without[j])
				{	
					contor2 = contor;
					for(it2 = bricksmask.begin(); it2 != bricksmask.end(); ++it2)
					{
						if(contor2 < bricksmask.size())
						{					
							if(bricks[contor2] == without[j])
							{
								contor2++;
								continue;
							}
							if(bricks[contor2] != without[j])
							{
								swapbrick = bricks[contor];
								bricks[contor] = bricks[contor2];
								bricks[contor2] = swapbrick;
								break;
							}

						}
						else
						{
							break;
						}
					}
				}

				if(bricksmask[contor] == without[j] && bricks[contor] != without[j])
				{
					contor2 = contor;
					for(it2 = bricksmask.begin(); it2 != bricksmask.end(); ++it2)
					{
						if(contor2 < bricksmask.size())
						{
							if(bricks[contor2] != without[j])
							{
								contor2++;
								continue;
							}
							if(bricks[contor2] == without[j])
							{	
								swapbrick = bricks[contor];
								bricks[contor] = bricks[contor2];
								bricks[contor2] = swapbrick;
								break;
							}	
						}
						else
						{
							break;
						}
					}
				}

				contor++;
			}

		}
}
