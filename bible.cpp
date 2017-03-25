#include "bible.h"
using namespace std;

//BibleRec

BibleRec::BibleRec(string textPath, string idxPath) {
	curText = new BibleText(textPath);
	curIdx = new BibleIdx(idxPath);
}

BibleRec::~BibleRec() {
	delete curText;
	delete curIdx;
}

TextSec BibleRec::getText(int start, int end) {
	return curText->getList(start, end);
}

TextSec BibleRec::getBookText(int bookNum) {
    return curText->getList(curIdx->getBook(bookNum).getStartLine(), curIdx->getBook(bookNum).getEndLine());
}

vector<string>* BibleRec::getBookList() {
	return curIdx->getList();
}

int BibleRec::getBookNum() {
    return curIdx->getBookNum();
}

Book BibleRec::getBookInfo(int bookId) {
	return curIdx->getBook(bookId);
}

vector<int> BibleRec::searchPhrase(string phrase, int start, int end) {
    vector<int> matchList;

	TextSec selection = this->getText(start, end);

	for(int i = 0; i < selection.len; ++i) {
		if(selection.sec.at(i).find(phrase) != string::npos) {
            matchList.push_back(i + selection.lineStart);
		}
	}

	return matchList;
}

Location BibleRec::getLocation(int lineNum) {
	return curIdx->getLocation(lineNum);
}


//BibleText

BibleText::BibleText(string textPath) {
	ifstream ins;
    cout << textPath.c_str() << endl;
    ins.open(textPath.c_str());
    ins.clear();
    char buffer[512];

    //read by line and push into list
    while(ins.good()) {
        ins.getline(buffer, 512, '\n');
        string temp = buffer;
        lineList.push_back(buffer);
        numLines++;
    }
}

BibleText::~BibleText() {

}

TextSec BibleText::getList(int start, int end) {
    if(end == 0 || end > numLines)
        end = numLines;

    if(start < 0)
        start = 0;

	vector<string> selection;

	for(int i = start; i < end; ++i) {
		selection.push_back(lineList.at(i));
	}

	TextSec curSec;
	curSec.lineStart = start;
	curSec.len = selection.size();
	curSec.sec = selection;

	return curSec;
}

int BibleText::getListLen() {
	return numLines;
}


//BibleIdx

BibleIdx::BibleIdx(string idxPath) {
    try {
        ifstream ins;
        ins.open(idxPath.c_str());

        ins >> numBooks;

        int totalLines = 0;
        for(int i = 0; i < numBooks; ++i) {
            int numChapters, verses;
            int lineCount = 0;
            vector<int> numVerses;

            char tempName[64];
            ins.ignore();
            ins.getline(tempName, 64, '\n');
            string name = tempName;
            ins >> numChapters;
            for(int n = 0; n < numChapters; ++n) {
                ins >> verses;
                lineCount += verses;
                numVerses.push_back(verses);
            }
            Book newBook(name, numChapters, totalLines, totalLines + lineCount, numVerses);
            totalLines += lineCount;
            bookList.push_back(newBook);
        }
    } catch(exception e) {
        cout << "index read borked" << endl;
    }
}

BibleIdx::~BibleIdx() {

}

vector<string>* BibleIdx::getList() {
    if(numBooks == 0) {
        return NULL;
    } else {
        vector<string> *newList = new vector<string>;
        for(int i = 0; i < numBooks; ++i) {
            newList->push_back(bookList.at(i).getName());
        }
        return newList;
    }
}

int BibleIdx::getBookNum() {
    return numBooks;
}

Book BibleIdx::getBook(int id) {
	return bookList.at(id);
}

Location BibleIdx::getLocation(int id) {
	Location curLoc;

	for(int i = 0; i < numBooks; ++i) {
		if(bookList.at(i).getEndLine() > id) {
			curLoc = bookList.at(i).getLocation(id);
			curLoc.bookName = bookList.at(i).getName();
            curLoc.bookIdx = i;
			break;
		}
	}

	return curLoc;
}


//Book

Book::Book(string name, int numChapters, int startLine, int endLine, vector<int> numVerses) {
	this->name = name;
	this->numChapters = numChapters;
	this->startLine = startLine;
	this->endLine = endLine;
	this->numVerses = numVerses;
}

Book::~Book() {

}

string Book::getName() {
	return name;
}

int Book::getNumChapt() {
	return numChapters;
}

int Book::getNumVerse(int chapter) {
	return numVerses.at(chapter);
}

int Book::getStartLine() {
	return startLine;
}

int Book::getEndLine() {
	return endLine;
}

Location Book::getLocation(int id) {
	Location curLoc;

	int total = 0;
	for(int i = 0; i < numChapters; ++i) {
		if(startLine + total + numVerses.at(i) > id) {
			curLoc.chapter = i;
			curLoc.verse = (id - (startLine + total));
			curLoc.lineNum = id;
			break;
		}
		total += numVerses.at(i);
	}

	return curLoc;
}

int Book::getLineNum(Location curLoc) {
    int lineNum = startLine;

    for(int i = 0; i < curLoc.chapter; i++) {
        lineNum += numVerses.at(i);
    }

    lineNum += curLoc.verse;

    return lineNum;
}

