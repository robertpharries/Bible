#include "searchdiag.h"
#include "ui_searchdiag.h"

SearchDiag::SearchDiag(QWidget *parent, BibleRec *newBible) : QDialog(parent), ui(new Ui::SearchDiag) {
    ui->setupUi(this);

    curBible = newBible;

    connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(doCancel()));
    connect(ui->searchBtn, SIGNAL(clicked()), this, SLOT(doSearch()));
    connect(ui->fromBook, SIGNAL(currentIndexChanged(int)), this, SLOT(setFromChapters(int)));
    connect(ui->toBook, SIGNAL(currentIndexChanged(int)), this, SLOT(setToChapters(int)));
    connect(ui->fromChp, SIGNAL(valueChanged(int)), this, SLOT(setFromVerses(int)));
    connect(ui->toChp, SIGNAL(valueChanged(int)), this, SLOT(setToVerses(int)));

    vector<string> bookList = curBible->getBookList();

    for(int i =  0; i < bookList.size(); i++) {
        ui->fromBook->addItem(QString::fromStdString(bookList.at(i)));
        ui->toBook->addItem(QString::fromStdString(bookList.at(i)));
    }

    ui->fromBook->setCurrentIndex(0);
    ui->toBook->setCurrentIndex(bookList.size()-1);
    ui->toChp->setValue(ui->toChp->maximum());
    ui->toVrs->setValue(ui->toVrs->maximum());
}

SearchDiag::~SearchDiag() {
    delete ui;
}

void SearchDiag::doSearch() {
    QString *phrase = new QString(ui->phraseEdit->text());

    Location *fromLoc = new Location;
    Book fromBook = curBible->getBookInfo(ui->fromBook->currentIndex());
    fromLoc->bookIdx = ui->fromBook->currentIndex();
    //fromLoc->bookName = fromBook.getName();
    fromLoc->chapter = ui->fromChp->value()-1;
    fromLoc->verse = ui->fromVrs->value()-1;
    fromLoc->lineNum = fromBook.getLineNum(*(fromLoc));

    Location *toLoc = new Location;
    Book toBook = curBible->getBookInfo(ui->toBook->currentIndex());
    toLoc->bookIdx = ui->toBook->currentIndex();
    //toLoc->bookName = toBook.getName();
    toLoc->chapter = ui->toChp->value()-1;
    toLoc->verse = ui->toVrs->value()-1;
    toLoc->lineNum = toBook.getLineNum(*(toLoc));

    emit closedSignal(phrase, fromLoc, toLoc);
}

void SearchDiag::doCancel() {
    emit closedSignalNP();
}

void SearchDiag::setFromChapters(int bookNum) {
    Book newBook = curBible->getBookInfo(bookNum);
    ui->fromChp->setRange(1, newBook.getNumChapt());
    ui->fromChp->setValue(1);
}

void SearchDiag::setToChapters(int bookNum) {
    Book newBook = curBible->getBookInfo(bookNum);
    ui->toChp->setRange(1, newBook.getNumChapt());
    ui->toChp->setValue(1);
}

void SearchDiag::setFromVerses(int chapterNum) {
    Book newBook = curBible->getBookInfo(ui->fromBook->currentIndex());
    ui->fromVrs->setRange(1, newBook.getNumVerse(chapterNum-1));
    ui->fromVrs->setValue(1);
}

void SearchDiag::setToVerses(int chapterNum) {
    Book newBook = curBible->getBookInfo(ui->toBook->currentIndex());
    ui->toVrs->setRange(1, newBook.getNumVerse(chapterNum-1));
    ui->toVrs->setValue(1);
}
