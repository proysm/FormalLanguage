/***************************************************************
*      scanner routine for Mini C language                    *
***************************************************************/

#include <stdio.h>
#include <string.h>
#include <string>
#include <cctype>
#include "Scanner.h"

#define _CRT_SECURE_NO_WARNINGS
using namespace std;
 
extern int line, column;                       // (Expand)

extern FILE *sourceFile;                       // miniC source program, Main.cpp의 전역변수 호출

int superLetter(char ch);
int superLetterOrDigit(char ch);
int getNumber(char firstCharacter);
int hexValue(char ch);
void lexicalError(int n);

char *tokenName[] = {
	"!",        "!=",      "%",       "%=",     "%ident",   "%number",
	/* 0          1           2         3          4          5        */
	"&&",       "(",       ")",       "*",      "*=",       "+",
	/* 6          7           8         9         10         11        */
	"++",       "+=",      ",",       "-",      "--",	    "-=",
	/* 12         13         14        15         16         17        */
	"/",        "/=",      ";",       "<",      "<=",       "=",
	/* 18         19         20        21         22         23        */
	"==",       ">",       ">=",      "[",      "]",        "eof",
	/* 24         25         26        27         28         29        */
	//   ...........    word symbols ................................. //
	"const",    "else",     "if",      "int",     "return",  "void",	
	/* 30         31         32        33         34         35        */
	"while",    "{",        "||",      "}",
	/* 36         37         38        39                              */
	//   ...........    (Expand)     ................................. //
	"char",     "double",    "for",      "do",      "goto",     "switch",  
	/* 40        41          42           43         44          45    */
	"case",     "break",    "default",   "%char",   "%string",  "%realnum",
	/* 46        47          48           49         50          51    */
	":"
	/* 52                                                              */
};


char *keyword[NO_KEYWORD] = {
	"const",   "else",     "if",     "int",     "return",  "void",    "while",
	//   ......................    (Expand)   .......................  //
	"char",    "double",   "for",    "do",      "goto",    "switch",
	"case",    "break",    "default"
};

enum tsymbol tnum[NO_KEYWORD] = {
	tconst,    telse,     tif,     tint,     treturn,   tvoid,     twhile,
	//   ......................    (Expand)   .......................  //
	tchar,     tdouble,   tfor,    tdo,      tgoto,     tswitch,   tcase,     tbreak,    tdefault
};

struct tokenType scanner()
{
	struct tokenType token;
	int i, index;
	char ch, id[ID_LENGTH], realnumid[ID_LENGTH]; // (Expand)

	token.number = tnull;

	do {
		while (isspace(ch = fgetc(sourceFile))){ // state 1: skip blanks	
			
			//   ...........   (Expand)  ............  //
			// printf("%d ", ch);   // for debug
			if(ch == 13){
				line++;
				column=0;
			}
		}
		column++;
		if (superLetter(ch)) { // identifier or keyword
			i = 0;
			do {
				if (i < ID_LENGTH) id[i++] = ch;
				ch = fgetc(sourceFile);
			} while (superLetterOrDigit(ch)); 

			if (i >= ID_LENGTH) lexicalError(1);
			id[i] = '\0';
			ungetc(ch, sourceFile);  //  retract

			// find the identifier in the keyword table
			for (index = 0; index < NO_KEYWORD; index++)
				if (!strcmp(id, keyword[index])) break;
			if (index < NO_KEYWORD)    // found, keyword exit
				token.number = tnum[index];
			else {                     // not found, identifier exit
				token.number = tident;
				strcpy(token.value.id, id);
			}
		}  // end of identifier or keyword

		//   ...........   (Expand)  ............  //
		else if (isdigit(ch)) {  // number
			i = 0;
			do{
				if (i < ID_LENGTH) realnumid[i++] = ch;
				ch = fgetc(sourceFile);
			} while (isdigit(ch));

			if (i >= ID_LENGTH) lexicalError(1);

			if(ch == '.'){     //  double literal
				do{
					if (i < ID_LENGTH) realnumid[i++] = ch;
					ch = fgetc(sourceFile);
				} while (isdigit(ch));

				if(ch == 'E' || ch == 'e') {
					realnumid[i++] = ch;
					ch = fgetc(sourceFile);
					if(ch == '+' || ch == '-'){
						realnumid[i++] = ch;
						ch = fgetc(sourceFile);
						if(isdigit(ch)){
							do{
								realnumid[i++] = ch;
								ch = fgetc(sourceFile);
							} while(isdigit(ch));
						}
						else lexicalError(7);
					}
					else if(isdigit(ch)){
						do{
							realnumid[i++] = ch;
							ch = fgetc(sourceFile);
						} while(isdigit(ch));
					}
					else lexicalError(8);
				}

				ungetc(ch, sourceFile);
				realnumid[i] = '\0';
				token.number = trealnum;
				strcpy(token.value.id, realnumid);

			}
			else {                     //  integer
				for (int x = i - 1; x > 0; x--) {
					ungetc(realnumid[x], sourceFile);
				}
				ch = realnumid[0];
				token.number = tnumber;
				strcpy(token.value.id, realnumid);
				token.value.num = getNumber(ch);
				
			}
		}

		//   ...........   (Expand)  ............  //
		else switch (ch) {  // special character
		case '/':
			ch = fgetc(sourceFile);
			if (ch == '*'){             // text comment
				ch = fgetc(sourceFile);
				if(ch == '*'){          // documented comments (/** ~ */)
					printf("Documented Comments ------> ");
					do{
						while (ch != '*') {
							printf("%c", ch);
							ch = fgetc(sourceFile);
						}
						ch = fgetc(sourceFile);
					} while (ch != '/');
					printf("\n");
				}
				else{
					do {
						while (ch != '*') ch = fgetc(sourceFile);
						ch = fgetc(sourceFile);
					} while (ch != '/');
				}
			}					
			else if (ch == '/'){		// line comment
				ch = fgetc(sourceFile);
				if (ch == '/'){         // single line documented comments (///)
					printf("Documented Comments ------> ");
					do{
						ch = fgetc(sourceFile);
						printf("%c", ch);
					} while (ch != '\n');
					printf("\n");
				}
				else {
					while (fgetc(sourceFile) != '\n');
					ch = 13; // retract
					ungetc(ch, sourceFile);
					printf("here%c", ch);
				}
			}
			else if (ch == '=')  token.number = tdivAssign;
			else {
				token.number = tdiv;
				ungetc(ch, sourceFile); // retract
			}
			break;
		case '!':
			ch = fgetc(sourceFile);
			if (ch == '=')  token.number = tnotequ;
			else {
				token.number = tnot;
				ungetc(ch, sourceFile); // retract
			}
			break;
		case '%':
			ch = fgetc(sourceFile);
			if (ch == '=') {
				token.number = tremAssign;
			}
			else {
				token.number = tremainder;
				ungetc(ch, sourceFile);
			}
			break;
		case '&':
			ch = fgetc(sourceFile);
			if (ch == '&')  token.number = tand;
			else {
				lexicalError(2);
				ungetc(ch, sourceFile);  // retract
			}
			break;
		case '*':
			ch = fgetc(sourceFile);
			if (ch == '=')  token.number = tmulAssign;
			else {
				token.number = tmul;
				ungetc(ch, sourceFile);  // retract
			}
			break;
		case '+':
			ch = fgetc(sourceFile);
			if (ch == '+')  token.number = tinc;
			else if (ch == '=') token.number = taddAssign;
			else {
				token.number = tplus;
				ungetc(ch, sourceFile);  // retract
			}
			break;
		case '-':
			ch = fgetc(sourceFile);
			if (ch == '-')  token.number = tdec;
			else if (ch == '=') token.number = tsubAssign;
			else {
				token.number = tminus;
				ungetc(ch, sourceFile);  // retract
			}
			break;
		case '<':
			ch = fgetc(sourceFile);
			if (ch == '=') token.number = tlesse;
			else {
				token.number = tless;
				ungetc(ch, sourceFile);  // retract
			}
			break;
		case '=':
			ch = fgetc(sourceFile);
			if (ch == '=')  token.number = tequal;
			else {
				token.number = tassign;
				ungetc(ch, sourceFile);  // retract
			}
			break;
		case '>':
			ch = fgetc(sourceFile);
			if (ch == '=') token.number = tgreate;
			else {
				token.number = tgreat;
				ungetc(ch, sourceFile);  // retract
			}
			break;
		case '|':
			ch = fgetc(sourceFile);
			if (ch == '|')  token.number = tor;
			else {
				lexicalError(3);
				ungetc(ch, sourceFile);  // retract
			}
			break;
		case '(': token.number = tlparen;         break;
		case ')': token.number = trparen;         break;
		case ',': token.number = tcomma;          break;
		case ';': token.number = tsemicolon;      break;
		case '[': token.number = tlbracket;       break;
		case ']': token.number = trbracket;       break;
		case '{': token.number = tlbrace;         break;
		case '}': token.number = trbrace;         break;
		//   ...........   (Expand)  ............  //
		case ':'  : token.number = tcolon;        break;
		case '\'' :                                // character literal
			i = 0;
			ch = fgetc(sourceFile);
			token.number = tcharacter;

			token.value.id[i++] = ch;
			token.value.id[i++] = '\0';
			token.value.id[i] = '\0';
			ch = fgetc(sourceFile);
			if(ch != '\'') lexicalError(5);
			break;
		case '"'  :                                // string literal
			i = 0;
			ch = fgetc(sourceFile);
			while(ch != '"'){
				if(i < ID_LENGTH) id[i++] = ch;
				ch = fgetc(sourceFile);
			}
			if(i >= ID_LENGTH) lexicalError(1);
			id[i] = '\0';

			if(ch == '"'){	
				token.number = tstring;
				strcpy(token.value.id, id);
			}
			else lexicalError(6);
			break;

		case '.'  :                               // double literal (short form : .123)
			i = 0;
			do {
				if (i < ID_LENGTH) realnumid[i++] = ch;
				ch = fgetc(sourceFile);
			} while (isdigit(ch)); 

			if (i >= ID_LENGTH) lexicalError(1);

			ungetc(ch, sourceFile);
			realnumid[i] = '\0';
			token.number = trealnum;
			strcpy(token.value.id, realnumid);
			break;
		case EOF: token.number = teof;            break;
		default: {
			printf("Current character : %c", ch);
			lexicalError(4);
			break;
		}

		} // switch end
	} while (token.number == tnull);
	return token;
} // end of scanner

void lexicalError(int n)
{
	printf(" *** Lexical Error : ");
	switch (n) {
	case 1: printf("an identifier length must be less than 12.\n");
		break;
	case 2: printf("next character must be &\n");
		break;
	case 3: printf("next character must be |\n");
		break;
	case 4: printf("invalid character\n");
		break;
	//   ...........   (Expand)  ............  //
	case 5 : printf("next character must be \'\n");
		break;
	case 6 : printf("next character must be \"\n");
		break;
	case 7 : printf("next character must be digit\n");
		break;
	case 8 : printf("next character must be '+' or '-'\n");
		break;
	}
}

int superLetter(char ch)
{
	if (isalpha(ch) || ch == '_') return 1;
	else return 0;
}

int superLetterOrDigit(char ch)
{
	if (isalnum(ch) || ch == '_') return 1;
	else return 0;
}

int getNumber(char firstCharacter)
{
	int num = 0;
	int value;
	char ch;

	if (firstCharacter == '0') {
		ch = fgetc(sourceFile);
		if ((ch == 'X') || (ch == 'x')) {		// hexa decimal
			while ((value = hexValue(ch = fgetc(sourceFile))) != -1)
				num = 16 * num + value;
		}
		else if ((ch >= '0') && (ch <= '7'))	// octal
			do {
				num = 8 * num + (int)(ch - '0');
				ch = fgetc(sourceFile);
			} while ((ch >= '0') && (ch <= '7'));
		else num = 0;						// zero
	}
	else {									// decimal
		ch = firstCharacter;
		do {
			num = 10 * num + (int)(ch - '0');
			ch = fgetc(sourceFile);
		} while (isdigit(ch));
	}
	ungetc(ch, sourceFile);  /*  retract  */
	return num;
}

int hexValue(char ch)
{
	switch (ch) {
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		return (ch - '0');
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
		return (ch - 'A' + 10);
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
		return (ch - 'a' + 10);
	default: return -1;
	}
}

//   ...........   (Expand)  ............  //
//   Token -----> int (token number, token value, file name, line number, column number)
void printToken(struct tokenType token, char* fileName)
{
	if (token.number == tident
		|| token.number == trealnum
		|| token.number == tcharacter
		|| token.number == tstring){
		printf("Token -----> %12s (", tokenName[token.number]);
		printf("%2d, %8s, %12s, %2d, %2d", 
				token.number, token.value.id, fileName, line, column);
		printf(")\n");
	}
	else if (token.number == tnumber){
		printf("Token -----> %12s (", tokenName[token.number]);
		printf("%2d, %8d, %12s, %2d, %2d", 
				token.number, token.value.num, fileName, line, column);
		printf(")\n");
	}
	else{
		printf("Token -----> %12s (", tokenName[token.number]);
		printf("%2d, %8s, %12s, %2d, %2d", 
				token.number, tokenName[token.number], fileName, line, column);
		printf(")\n");
	}


}
