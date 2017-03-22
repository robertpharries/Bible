#include "gotodiag.h"
#include "ui_gotodiag.h"

gotodiag::gotodiag(QWidget *parent, BibleRec *newBible) :  QDialog(parent), ui(new Ui::gotodiag) {
    ui->setupUi(this);

    curBible = newBible;

    connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(doCancel()));
    connect(ui->goBtn, SIGNAL(clicked()), this, SLOT(doGo()));
    connect(ui->goBook, SIGNAL(currentIndexChanged(int)), this, SLOT(setFromChapters(int)));
    connect(ui->goChp, SIGNAL(valueChanged(int)), this, SLOT(setFromVerses(int)));

    vector<string> bookList = curBible->getBookList();

    for(int i =  0; i < bookList.size(); i++) {
        ui->goBook->addItem(QString::fromStdString(bookList.at(i)));
    }

    ui->goBook->setCurrentIndex(0);
}

gotodiag::~gotodiag() {
    delete ui;
}

void gotodiag::doGo() {
    Location *fromLoc = new Location;
    Book goBook = curBible->getBookInfo(ui->goBook->currentIndex());
    fromLoc->bookIdx = ui->goBook->currentIndex();
    //fromLoc->bookName = goBook.getName();
    fromLoc->chapter = ui->goChp->value()-1;
    fromLoc->verse = ui->goVrs->value()-1;
    fromLoc->lineNum = goBook.getLineNum(*(fromLoc));

    emit closedSignal(fromLoc);
}

void gotodiag::doCancel() {
    emit closedSignalNP();
}

void gotodiag::closeEvent(QCloseEvent *e) {
    emit closedSignalNP();
}

void gotodiag::setFromChapters(int bookNum) {
    Book newBook = curBible->getBookInfo(bookNum);
    ui->goChp->setRange(1, newBook.getNumChapt());
    ui->goChp->setValue(1);
}

void gotodiag::setFromVerses(int chapterNum) {
    Book newBook = curBible->getBookInfo(ui->goBook->currentIndex());
    ui->goVrs->setRange(1, newBook.getNumVerse(chapterNum-1));
    ui->goVrs->setValue(1);
}


