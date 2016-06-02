// ïðîáàoptget.cpp: îïðåäåëÿåò òî÷êó âõîäà äëÿ êîíñîëüíîãî ïðèëîæåíèÿ.
//

// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "string"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fstream>

using namespace std;
#define TRUE 1
#define FALSE 0
#define bool unsigned char



void create(string word, string alfavit, int *F, int *G, string key)
{
	int alfavit_flag = alfavit.size()+1;

	for (int i = 0; i < word.size(); i++)
	{
		F[i] = G[i] = alfavit_flag;

		for (int n = 0; n < alfavit.size(); n++)
		{
			if (word[i] == alfavit[n])
				F[i] = n;	
				
			if (key[i%key.size()] == alfavit[n])
				G[i] = n;
		}
	}
}

string Enc(int *F, int *G, string prov, string word, string alfavit)
{
	for (int i = 0; i < word.size(); i++)
	{
		if ((F[i] == 27) || (G[i] == 27))
			prov.push_back(word[i]);
		else
		{
			int e = ((F[i] + G[i]) % 26);
			prov.push_back(alfavit[e]);
		}	
	}
		cout << prov << endl;
		return prov;
}

string Dec(int *F, int *G, string prov, string word, string alfavit)
{

	for (int i = 0; i < word.size(); i++)
	{
		if ((F[i] == 27) || (G[i] == 27))
			prov.push_back(word[i]);
		else
		{
			int e = ((F[i] - G[i]) % 26);
			if (e < 0)
				e = 26 + e;
			prov.push_back(alfavit[e]);
		}
		
	}cout << prov << endl;
	return prov;
}
void WriteFile(string prov, string word)
{
	ofstream fp ("out.txt");
	if (fp.fail())
		printf(" Can't open file");

	fp << prov;
	fp.close();
	printf("Save file\n");
}


int main(int argc, char **argv)
{
	
	string alfavit = "abcdefghijklmnopqrstuvwxyz";
	char menu;
	int e = 0; 

	string prov;
	string key;
	string word;
	bool encrypt = FALSE;
	bool dencrypt = FALSE;
	char c;

	while ((c = getopt (argc, argv, "w:k:edf:")) != -1)
	{
		switch (c)
		{
		case'w':
			word = optarg;
			break;
		case 'k':
			key = optarg;
			break;
		case'e':
			encrypt = TRUE;
			break;
		case 'd':
			dencrypt = TRUE;
			break;
		case 'f':
			WriteFile(prov, word);
			break;
		case '?':/*
			if (optopt == 'k' || optopt == 'f' || optopt == 'w')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);*/
			return 1;
		default:
			printf("error\n");
			//abort();
		}
	}

	int *F = new int[word.size()];
	int *G = new int[word.size()];

	if(!(encrypt ^ dencrypt))
	        printf("You must specify cifer\n");
	if (word == "")
		printf ("You must specify word\n");
	if (key == "")
		printf("You must specify key\n");

	create(word, alfavit, F, G, key);

	//Нужно заменить auto на конкрентный тип
	// указатель на функцию
	
 	string (*f) (int *F, int *G, string prov, string word, string alfavit) = (encrypt ? Enc : Dec);
 	f(F, G, prov, word, alfavit);
	
	return 0;
}


