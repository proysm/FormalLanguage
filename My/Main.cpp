#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "Scanner.h"

#include <iostream>
using namespace std;

void icg_error(int n);


FILE *astFile;                          // AST file
FILE *sourceFile;                       // miniC source program
FILE *ucodeFile;                        // ucode file

#define FILE_LEN 30

void main(int argc, char *argv[])
{
	char fileName[FILE_LEN];
	int err;

	cout << " *** start of Mini C Compiler\n";
	if (argc != 2) {
		icg_error(1);
		exit(1);
	}
	//strcpy_s(fileName, argv[1]);

	cout << "   * source file name: " << fileName << "\n";

	//err = fopen_s(&sourceFile, fileName, "r");
	if (err != 0) {
		icg_error(2);
		exit(1);
	}
	
	
	struct tokenType token;
	
	cout << " === start of Scanner\n";
	
	token = scanner();
	
	while (token.number != teof) {

		cout << "Current Token --> ";
		printToken(token);
		token = scanner();
		
	} /* while (1) */


	cout << " === start of Parser\n";
	cout << " > Not yet implemented...\n";
	//root = parser();
	//printTree(root, 0);
	
	cout << " === start of ICG\n";
	cout << " > Not yet implemented...\n";

	//codeGen(root);
	cout << " *** end   of Mini C Compiler\n";
} // end of main

void icg_error(int n)
{
	cout << "icg_error: " << n << "\n";
	//3: cout << "A Mini C Source file must be specified.!!!\n";
	//"error in DCL_SPEC"
	//"error in DCL_ITEM"
}
