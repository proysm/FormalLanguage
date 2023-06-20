/***************************************************************
*      scanner routine for Mini C language                    *
*                                       컴퓨터공학과 유수민        *
*                                          2023. 5. 29        *
***************************************************************/

#pragma once // 컴파일러가 한 번만 컴파일 하도록 명령


#define NO_KEYWORD 16 // (Expand) 7 -> 16
#define ID_LENGTH 12



struct tokenType {
	int number;
	union {
		char id[ID_LENGTH];
		int num;
	} value;
};


enum tsymbol {
	tnull = -1,
	tnot,        tnotequ,    tremainder, tremAssign, tident,      tnumber,
	/* 0         1           2           3           4            5        */
	tand,        tlparen,    trparen,    tmul,       tmulAssign,  tplus,
	/* 6         7           8           9           10           11       */
	tinc,        taddAssign, tcomma,     tminus,     tdec,        tsubAssign,
	/* 12        13          14          15          16           17       */
	tdiv,        tdivAssign, tsemicolon, tless,      tlesse,      tassign,
	/* 18        19          20          21          22           23       */
	tequal,      tgreat,     tgreate,    tlbracket,  trbracket,   teof,
	/* 24        25          26          27          28           29       */
	//   ...........    word symbols ..................................... //
	tconst,      telse,      tif,        tint,       treturn,     tvoid,
	/* 30        31          32          33          34           35       */
	twhile,      tlbrace,    tor,        trbrace,
	/* 36        37          38          39                                */
	//   ...........    (Expand) Keyword ................................. //
	tchar,       tdouble,    tfor,       tdo,        tgoto,      tswitch,   
	/* 40        41          42          43          44           45       */
	tcase,       tbreak,     tdefault,   tcharacter, tstring,    trealnum,
	/* 46        47          48          49          50           51       */
	tcolon
	/* 52                                                                  */
};


struct tokenType scanner();
void printToken(struct tokenType token, char* fileName);