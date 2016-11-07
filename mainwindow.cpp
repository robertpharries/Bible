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
    connect(ui->openBible, SIGNAL(triggered()), this, SLOT(loadBible()));
    connect(ui->search, SIGNAL(triggered()), this, SLOT(searchGui()));
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
    curBible = new BibleRec("./" + name + ".txt", "./" + name + ".idx");
    cout << "Loading Bible..." << endl;

    QTextBlockFormat format;
    format.setLineHeight(0, QTextBlockFormat::SingleHeight);

    QTextCursor curs = ui->bibleText->textCursor();
    curs.setBlockFormat(format);

    TextSec entireText = curBible->getText(0, 0);
    QString temp;
    for(int i = 0; i < entireText.len; ++i) {
        temp = (entireText.sec.at(i)).c_str();
        curs.insertText(temp);
    }

    moveCursor(0);
}

void MainWindow::moveCursor(int lineNum) {
    QTextCursor cursor = ui->bibleText->textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, lineNum);
    ui->bibleText->setTextCursor(cursor);
    ui->bibleText->centerCursor();
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
    moveCursor(lineNum);
    delete resultsWindow;
    resultsWindow = NULL;
}

void MainWindow::resultsClose() {
    delete resultsWindow;
    resultsWindow = NULL;
}

