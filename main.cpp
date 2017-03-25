#include "mainwindow.h"
#include "bible.h"
#include <QApplication>
#include <iostream>

//void outputLocation(BibleRec* curBible, int lineNum) {
//    Location curLoc = curBible->getLocation(lineNum);

//    cout << "Book: " << curLoc.bookName << " Chapter: " << curLoc.chapter+1 << " Verse: " << curLoc.verse+1 << endl;
//    cout << endl;

////    Book curBook = getBookInfo(curLoc.bookIdx);
////    cout << curBook.getLineNum(curLoc) << endl;
//}

void outputSelection(BibleRec* curBible, int start, int end) {
    TextSec entireText = curBible->getText(start, end);

    for(int i = 0; i < entireText.len; ++i) {
        cout << entireText.sec.at(i) << endl;
    } cout << endl;
}

void outputIndex(BibleRec* curBible) {
    vector<string>* nameList = curBible->getBookList();

    for(int i = 0; i < nameList->size(); ++i) {
        cout << nameList->at(i) << endl;
        Book curBook = curBible->getBookInfo(i);
        cout << "chapters: " << curBook.getNumChapt() << endl;
        for(int i = 0; i < curBook.getNumChapt(); ++i) {
            cout << "chapter " << i+1 << " has " << curBook.getNumVerse(i) << endl;
        }
        cout << endl;
    }
}

void find(BibleRec* curBible, string phrase) {
    vector<int> matched = curBible->searchPhrase(phrase, 0, 0);
    if(matched.size() == 0)
        cout << "No matches found" << endl;
    else {
        for(int i = 0; i < matched.size(); ++i) {
            TextSec matchedLine = curBible->getText(matched.at(i), matched.at(i)+1);
            cout << matchedLine.sec.at(0) << endl;
            //outputLocation(curBible, matchedLine.lineStart);
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(0);
    w.show();

    return a.exec();
}
