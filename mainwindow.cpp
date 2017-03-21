#include "mainwindow.h"

#include "ui_mainwindow.h"

void outputLocation(BibleRec* curBible, int lineNum) {
    Location curLoc = curBible->getLocation(lineNum);

    cout << "LineNum: " << curLoc.lineNum << " Book: " << curLoc.bookName << " Chapter: " << curLoc.chapter+1 << " Verse: " << curLoc.verse+1 << endl;
    cout << endl;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->bibleText->setReadOnly(true);

    //setup actions
    this->bookState = 0;

    ui->bookLabel->setText("");

    connect(ui->openBible, SIGNAL(triggered()), this, SLOT(loadBible()));
    connect(ui->search, SIGNAL(triggered()), this, SLOT(searchGui()));

    connect(ui->bookCtrlNext, SIGNAL(pressed()), this, SLOT(nextBook()));
    connect(ui->bookCtrlBack, SIGNAL(pressed()), this, SLOT(prevBook()));
}

MainWindow::~MainWindow() {
    delete curBible;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if(searchWindow)
        searchWindow->close();
    if(resultsWindow)
        resultsWindow->close();
    event->accept();
}

void MainWindow::loadBible() {
    string name = "niv";
    curBible = new BibleRec(name + ".txt", "./" + name + ".idx");
    cout << "Loading Bible..." << endl;

    this->changeBook();
}

void MainWindow::moveCursor(int lineNum) {
    int useLine = lineNum;
    if(useLine != 0) {
        useLine -= 1;
    }
    //scroll to bottom then back up to have selected line on the top of the screen
    QTextCursor cursor = ui->bibleText->textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    ui->bibleText->setTextCursor(cursor);
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    //scroll to the line before the one selected
    cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, useLine);
    ui->bibleText->setTextCursor(cursor);
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, lineNum);
    cursor.select(QTextCursor::LineUnderCursor);
    ui->bibleText->setTextCursor(cursor);
}

void MainWindow::searchGui() {
    if(!searchWindow) {
        searchWindow = new SearchDiag(0, this->curBible);
        connect(searchWindow, SIGNAL(closedSignal(QString*, Location*, Location*)), this, SLOT(searchPhrase(QString*, Location*, Location*)));
        connect(searchWindow, SIGNAL(closedSignalNP()), this, SLOT(searchClose()));
        searchWindow->show();
    }
}

void MainWindow::searchClose() {
    delete searchWindow;
    searchWindow = NULL;
}

void MainWindow::searchPhrase(QString *phrase, Location *fromLoc, Location *toLoc) {
    cout << "searching..." << endl;
    delete searchWindow;
    searchWindow = NULL;

    vector<int> matched = curBible->searchPhrase(phrase->toStdString(), fromLoc->lineNum, (toLoc->lineNum)+1);
    if(matched.size() == 0) {
        QMessageBox msgBox;
        msgBox.setText("No Matches Found.");
        msgBox.exec();
    } else {
        for(int i = 0; i < matched.size(); i++) {
            cout << matched.at(i) << endl;
            outputLocation(curBible, matched.at(i));
        }
        if(!resultsWindow) {
            resultsWindow = new ResultsDiag(0, this->curBible, &matched, phrase->toStdString());
            connect(resultsWindow, SIGNAL(selectedSignal(int)), this, SLOT(matchSelected(int)));
            connect(resultsWindow, SIGNAL(cancelSignal()), this, SLOT(resultsClose()));
            resultsWindow->show();
        }
    }
}

void MainWindow::matchSelected(int lineNum) {
    cout << lineNum << endl;

    Location newLoc = this->curBible->getLocation(lineNum);
    cout << newLoc.bookIdx << endl;
    bookState = newLoc.bookIdx;
    this->changeBook();

    int relLineNum = lineNum - curBible->getBookInfo(bookState).getStartLine();

    moveCursor(relLineNum);
    delete resultsWindow;
    resultsWindow = NULL;
}

void MainWindow::resultsClose() {
    delete resultsWindow;
    resultsWindow = NULL;
}

void MainWindow::nextBook() {
    if(this->curBible) {
        //change the state
        if(this->bookState < this->curBible->getBookNum()-1) {
            this->bookState++;
        }

        cout << this->bookState << endl;
        this->changeBook();
    }


}

void MainWindow::prevBook() {
    if(this->curBible) {
        //change the state
        if(this->bookState > 0) {
            this->bookState--;
        }

        cout << this->bookState << endl;
        this->changeBook();
    }
}

void MainWindow::changeBook() {
    //edit the label
    QString bookName = this->curBible->getBookInfo(bookState).getName().c_str();
    ui->bookLabel->setText(bookName);

    //set the new text
    ui->bibleText->clear();

    QTextBlockFormat format;
    format.setLineHeight(0, QTextBlockFormat::SingleHeight);

    QTextCursor curs = ui->bibleText->textCursor();
    curs.setBlockFormat(format);

    //show book
    TextSec newBook = curBible->getBookText(bookState);
    QString temp;
    for(int i = 0; i < newBook.len; ++i) {
        temp = (newBook.sec.at(i)).c_str();
        curs.insertText(temp);
    }

    moveCursor(0);
}

