#include "searchdiag.h"
#include "ui_searchdiag.h"

SearchDiag::SearchDiag(QWidget *parent, BibleRec *newBible, std::vector<SearchTempl> *newstempl) : QDialog(parent), ui(new Ui::SearchDiag) {
    ui->setupUi(this);

    curBible = newBible;
    stempl = newstempl;

    connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(doCancel()));
    connect(ui->searchBtn, SIGNAL(clicked()), this, SLOT(doSearch()));
    connect(ui->fromBook, SIGNAL(currentIndexChanged(int)), this, SLOT(setFromChapters(int)));
    connect(ui->toBook, SIGNAL(currentIndexChanged(int)), this, SLOT(setToChapters(int)));
    connect(ui->fromChp, SIGNAL(valueChanged(int)), this, SLOT(setFromVerses(int)));
    connect(ui->toChp, SIGNAL(valueChanged(int)), this, SLOT(setToVerses(int)));
    connect(ui->searchTempl, SIGNAL(currentIndexChanged(int)), this, SLOT(setTempl(int)));
    connect(ui->saveTempl, SIGNAL(clicked()), this, SLOT(doSaveTempl()));
    //set the template back to custom if a value changes
    connect(ui->fromBook, SIGNAL(currentIndexChanged(int)), this, SLOT(setCustom()));
    connect(ui->toBook, SIGNAL(currentIndexChanged(int)), this, SLOT(setCustom()));
    connect(ui->fromChp, SIGNAL(valueChanged(int)), this, SLOT(setCustom()));
    connect(ui->toChp, SIGNAL(valueChanged(int)), this, SLOT(setCustom()));
    connect(ui->fromVrs, SIGNAL(valueChanged(int)), this, SLOT(setCustom()));
    connect(ui->toVrs, SIGNAL(valueChanged(int)), this, SLOT(setCustom()));

    vector<string>* bookList = curBible->getBookList();

    for(int i = 0; i < bookList->size(); i++) {
        ui->fromBook->addItem(QString::fromStdString(bookList->at(i)));
        ui->toBook->addItem(QString::fromStdString(bookList->at(i)));
    }

    loadTempl(2);
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

void SearchDiag::closeEvent(QCloseEvent *e) {
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
    ui->toChp->setValue(ui->toChp->maximum());
}

void SearchDiag::setFromVerses(int chapterNum) {
    Book newBook = curBible->getBookInfo(ui->fromBook->currentIndex());
    ui->fromVrs->setRange(1, newBook.getNumVerse(chapterNum-1));
    ui->fromVrs->setValue(1);
}

void SearchDiag::setToVerses(int chapterNum) {
    Book newBook = curBible->getBookInfo(ui->toBook->currentIndex());
    ui->toVrs->setRange(1, newBook.getNumVerse(chapterNum-1));
    ui->toVrs->setValue(ui->toVrs->maximum());
}

void SearchDiag::loadTempl(int index) {
    ui->searchTempl->clear();
    ui->searchTempl->addItem("Custom");
    ui->searchTempl->insertSeparator(ui->searchTempl->count());

    if(stempl) {
        for(int i = 0; i < stempl->size(); i++) {
            if(i == 3)
                ui->searchTempl->insertSeparator(ui->searchTempl->count());
            ui->searchTempl->addItem(QString(stempl->at(i).name.c_str()));
        }
    }

    ui->searchTempl->setCurrentIndex(index);
}

void SearchDiag::setCustom() {
    ui->searchTempl->setCurrentIndex(0);
}

void SearchDiag::setTempl(int index) {
    QString templName = ui->searchTempl->currentText();
    if(stempl) {
        for(int i = 0; i < stempl->size(); i++) {
            if(stempl->at(i).name == templName.toStdString()) {
                ui->fromBook->setCurrentIndex(stempl->at(i).f_bookIdx);
                ui->toBook->setCurrentIndex(stempl->at(i).t_bookIdx);
                ui->toVrs->setValue(stempl->at(i).t_verse);
                ui->toChp->setValue(stempl->at(i).t_chapter);
                ui->fromVrs->setValue(stempl->at(i).f_verse);
                ui->fromChp->setValue(stempl->at(i).f_chapter);
            }
            ui->searchTempl->setCurrentText(templName);
        }
    }
}

void SearchDiag::doSaveTempl() {
    if(!saveDiag) {
        saveDiag = new SearchTemplNameDiag();
        connect(saveDiag, SIGNAL(closedSignal(QString*)), this, SLOT(saveTempl(QString*)));
        connect(saveDiag, SIGNAL(closedSignalNP()), this, SLOT(saveClose()));
        saveDiag->show();
    }
}

void SearchDiag::saveTempl(QString* name) {
    if(stempl->size() < 50) {
        Location f_curLoc = curBible->getLocation(ui->fromBook->currentIndex(), ui->fromChp->value(), ui->fromVrs->value());
        Location t_curLoc = curBible->getLocation(ui->toBook->currentIndex(), ui->toChp->value(), ui->toVrs->value());

        SearchTempl temp = (SearchTempl) {name->toStdString(), f_curLoc.bookName, t_curLoc.bookName, ui->fromBook->currentIndex(), ui->fromChp->value(), ui->fromVrs->value(), ui->toBook->currentIndex(), ui->toChp->value(), ui->toVrs->value() };
        stempl->push_back(temp);
        emit addedTempl(temp);
    }

    loadTempl(stempl->size()+2);

    delete saveDiag;
    saveDiag = NULL;
}

void SearchDiag::saveClose() {
    if(saveDiag) {
        delete saveDiag;
        saveDiag = NULL;
    }
}
