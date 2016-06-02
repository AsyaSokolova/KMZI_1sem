#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdlib>
#include "des.h"
#include <unistd.h>

using namespace std ;


void help(const string & str)
{
	string header("DES") ;
	string command("Command : des option srcfile tarfile keyword") ;
	string options("Options :") ;
	string e("-e -w [input file] -k [key file] -f [output file] : encryption") ;
	string d("-d -w [input file] -k [key file] -f [output file] : decryption") ;
	string help("-h : show this manual") ;

	cout<<endl<<str<<endl<<"---------------------"<<header<<"---------------------"<<endl<<command<<endl<<options<<endl<<e<<endl<<d<<endl<<help<<endl;

	exit(-1) ;
}

void StrFromBlock(char * str , const Block & block)
{
	memset(str , 0 , 8) ;
	for(size_t i = 0 ; i < block.size() ; ++i)
	{
		if(true == block[i])
			*((unsigned char *)(str) + i / 8) |= (1 << (7 - i % 8)) ;
	}
}

void BlockFromStr(Block & block , const char * str)
{
	for(size_t i = 0 ; i < block.size() ; ++i)
	{
		if(0 != (*((unsigned char *)(str) + i / 8) & (1 << (7 - i % 8))))
			block[i] = true ;
		else 	block[i] = false ;
	}	
}

int main(int argc , char * argv[])
{	
	Method method ;
	char c;
	string in;
    	string out;
	string fkey;
	while ((c = getopt (argc, argv, "w:f:k:ed")) != -1)
	{
		switch(c)
		{
			case'w':
            			in = optarg;
				break;
			case 'e':
				method = e ;
				break ;
			case 'd':
				method = d ;
				break ;
			case 'h':
				help("") ;
				break ;
			case 'f':
				out=optarg;
				break;
			case 'k':
				fkey = optarg;
				break;
			default:
				help("Command Args Error") ;
				break ;
		}
	}
	ifstream key(fkey.c_str());
	char key1[8];
	key.read(key1, 8) ;
	ifstream srcFile(in.c_str()) ;
	ofstream tarFile(out.c_str()) ;
	if(argc < 5)
		help("Command Args Error") ;
	if(!srcFile || !tarFile || !key) help("File Open Error") ;
	
	Block block , bkey ;
	BlockFromStr(bkey , key1) ;
	char buffer[8] ;
	while(1)
	{
		memset(buffer , 0 , 8) ;
		srcFile.read(buffer , 8) ;
		if(srcFile.eof()) break ;
		BlockFromStr(block , buffer) ;
		des(block , bkey , method) ;
		StrFromBlock(buffer , block) ;
		tarFile.write(buffer , 8) ;
	}
	srcFile.close() ;
	tarFile.close() ;
	
	return 0 ;
}
