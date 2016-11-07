#ifndef BIBLE
#define BIBLE

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <locale>
using namespace std;

struct TextSec {
	int lineStart;
	int len;
	vector<string> sec;
};

struct Location {
    int bookIdx;
	string bookName;
	int chapter;
	int verse;
	int lineNum;
};

class BibleText {
public:
	BibleText(string);
	~BibleText();
	TextSec getList(int, int); //end 0 for all
	int getListLen();
private:
    int numLines = 0;
	vector<string> lineList;
};

class Book {
public:
	Book(string, int, int, int, vector<int>);
	~Book();
	string getName();
	int getNumChapt();
	int getNumVerse(int);	//int reverence chapter
	int getStartLine();
	int getEndLine();
    Location getLocation(int); //get location by line position
    int getLineNum(Location); //get line number by location, needs only bookIdx, chapter, verse
private:
	string name;
    int startLine = 0, endLine = 0;
    int numChapters = 0;
	vector<int> numVerses;
};

class BibleIdx {
public:
	BibleIdx(string);
	~BibleIdx();
	vector<string> getList();
	Book getBook(int);
	Location getLocation(int);
private:
    int numBooks = 0;
	vector<Book> bookList;
};

class BibleRec {
public:
	BibleRec(string, string);	//biblepath, indexpath
	~BibleRec();
	TextSec getText(int, int);
	vector<string> getBookList();
	Book getBookInfo(int);
    vector<int> searchPhrase(string, int, int);
	Location getLocation(int);
private:
	BibleIdx* curIdx;
	BibleText* curText;
};

#endif
