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

    connect(ui->openBible, SIGNAL(triggered()), this, SLOT(loadBible()));
    connect(ui->search, SIGNAL(triggered()), this, SLOT(searchGui()));
    connect(ui->goTo, SIGNAL(triggered()), this, SLOT(gotoGui()));

    connect(ui->bookCtrlNext, SIGNAL(pressed()), this, SLOT(nextBook()));
    connect(ui->bookCtrlBack, SIGNAL(pressed()), this, SLOT(prevBook()));

    connect(ui->bookSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(manualBookSelect(int)));
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
    if(gotoWindow)
        gotoWindow->close();
    event->accept();
}

void MainWindow::loadBible() {
    string name = "niv";
    curBible = new BibleRec(name + ".txt", "./" + name + ".idx");
    cout << "Loading Bible..." << endl;

    vector<string> bookList = curBible->getBookList();
    QStringList qbookList;

    foreach(string str, bookList) {
        qbookList.push_back(str.c_str());
    }

    ui->bookSelector->addItems(qbookList);

    this->changeBook();
}

void MainWindow::moveCursor(int lineNum) {
    cout << "mvoing to: " << lineNum << endl;
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
    cout << QTextCursor::End << endl;
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

    this->moveCursor(relLineNum);
    delete resultsWindow;
    resultsWindow = NULL;
}

void MainWindow::resultsClose() {
    delete resultsWindow;
    resultsWindow = NULL;
}

void MainWindow::gotoGui() {
    if(!gotoWindow) {
        gotoWindow = new gotodiag(0, this->curBible);
        connect(gotoWindow, SIGNAL(closedSignal(Location*)), this, SLOT(gotoLocation(Location*)));
        connect(gotoWindow, SIGNAL(closedSignalNP()), this, SLOT(gotoClose()));
        gotoWindow->show();
    }
}

void MainWindow::gotoClose() {
    delete gotoWindow;
    gotoWindow = NULL;
}

void MainWindow::gotoLocation(Location* newLoc) {
    bookState = newLoc->bookIdx;
    this->changeBook();

    cout << newLoc->lineNum << endl;
    cout << curBible->getBookInfo(bookState).getStartLine() << endl;

    int relLineNum = newLoc->lineNum - curBible->getBookInfo(bookState).getStartLine();

    cout << relLineNum << endl;

    this->moveCursor(relLineNum);
    delete gotoWindow;
    gotoWindow = NULL;
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
    ui->bookSelector->setCurrentIndex(bookState);

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
        string line = newBook.sec.at(i);
        line.insert(0, "<sup>");

        for(int n = 5; n < 10; n++) {
            if(!isdigit((line.at(n)))) {
                line.insert(n, "</sup>");
                break;
            }
        }

        cout << line << endl;

        temp.append("<p>");
        temp.append(line.c_str());
        temp.append("</p>");
    }
    curs.insertHtml(temp);

    this->moveCursor(0);
}

void MainWindow::manualBookSelect(int index) {
    bookState = index;
    changeBook();
}

