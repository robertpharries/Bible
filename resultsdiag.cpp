#include "resultsdiag.h"
#include "ui_resultsdiag.h"

ResultsDiag::ResultsDiag(QWidget *parent, BibleRec *newBible, vector<int> *matched, string phrase) : QDialog(parent), ui(new Ui::ResultsDiag) {
    ui->setupUi(this);
    curBible = newBible;
    resultsList = new vector<int>(*matched);
    clip = QApplication::clipboard();

    QStringList headers;
    headers << "Location" << "Sample";
    ui->resultTable->setColumnCount(2);
    ui->resultTable->setHorizontalHeaderLabels(headers);
    ui->resultTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->resultTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->resultTable->setRowCount(resultsList->size());

    ui->previewEdit->setReadOnly(true);

    for(int i = 0; i < resultsList->size(); i++) {
        Location curLoc = curBible->getLocation(resultsList->at(i));
        std::ostringstream chapt;
        std::ostringstream vers;
        chapt << curLoc.chapter+1;
        vers << curLoc.verse+1;
        QString locStr = (curLoc.bookName + " " + chapt.str() + ":" + vers.str()).c_str();
        QTableWidgetItem* item = new QTableWidgetItem(locStr);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->resultTable->setItem(i, 0, item);

        TextSec curSec = curBible->getText(resultsList->at(i), resultsList->at(i)+1);
        string fullStr = curSec.sec.at(0);
        int pos = (curSec.sec.at(0).find(phrase) < 30?0:curSec.sec.at(0).find(phrase)-30);
        string partStr = fullStr.substr(pos, 60);
        partStr = "..." + partStr + "...";
        QString sampleStr = partStr.c_str();
        item = new QTableWidgetItem(partStr.c_str());
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->resultTable->setItem(i, 1, item);
    }

    ui->resultTable->resizeColumnsToContents();

    ui->resultNumLabel->setText(QString::number(matched->size()));

    connect(ui->resultTable, SIGNAL(itemSelectionChanged()), SLOT(changePreview()));
    //connect(ui->resultTable, SIGNAL(cellDoubleClicked(int,int)), SLOT(select(int, int)));
    connect(ui->selectBtn, SIGNAL(clicked(bool)), SLOT(select()));
    connect(ui->cancelBtn, SIGNAL(clicked(bool)), SLOT(cancel()));
    connect(ui->copyAllBtn, SIGNAL(clicked(bool)), SLOT(copyAll()));
}

ResultsDiag::~ResultsDiag() {
    delete resultsList;
    delete ui;
}

void ResultsDiag::changePreview() {
    ui->previewEdit->clear();

    QModelIndexList indexes = ui->resultTable->selectionModel()->selection().indexes();
    QModelIndex index = indexes.at(0);
    int curRow = index.row();

    TextSec matchedLines = curBible->getText(resultsList->at(curRow)-1, resultsList->at(curRow)+2);
    for(int i = 0; i < matchedLines.sec.size(); i++) {
        QString temp = matchedLines.sec.at(i).c_str();
        temp = temp.left(temp.length() - 1);
        ui->previewEdit->appendPlainText(temp);
    }

    QTextCursor cursor = ui->previewEdit->textCursor();
    cursor.setPosition(0);
    ui->previewEdit->setFocus();
    ui->previewEdit->setTextCursor(cursor);
}

void ResultsDiag::select() {
    QModelIndexList indexes = ui->resultTable->selectionModel()->selection().indexes();
    QModelIndex index = indexes.at(0);
    int curRow = index.row();
    int lineNum = resultsList->at(curRow);
//    cout << lineNum << endl;
    emit selectedSignal(lineNum);
}

void ResultsDiag::select(int row, int col) {
//    cout << row << endl;
    int lineNum = resultsList->at(row);
    emit selectedSignal(lineNum);
}

void ResultsDiag::copyAll() {
    QString dump = "";

    for(int i = 0; i < resultsList->size(); i++) {
        Location curLoc = curBible->getLocation(resultsList->at(i));
        std::ostringstream chapt;
        std::ostringstream vers;
        chapt << curLoc.chapter+1;
        vers << curLoc.verse+1;
        QString locStr = (curLoc.bookName + " " + chapt.str() + ":" + vers.str()).c_str();
        bool context = ui->contextChk->checkState();
        if(context)
            dump += locStr + ":\r\n";

        int preContext = ui->preSpin->value();
        int postContext = ui->postSpin->value();
        TextSec matchedLines = curBible->getText(resultsList->at(i)-preContext, resultsList->at(i)+1+postContext);
        for(int i = 0; i < matchedLines.sec.size(); i++) {
            QString temp = matchedLines.sec.at(i).c_str();
            temp = temp.left(temp.length() - 1);
            dump += temp + "\r\n";
        }
        dump += "\r\n";

        clip->setText(dump);
    }
}

void ResultsDiag::cancel() {
    emit cancelSignal();
}

void ResultsDiag::closeEvent(QCloseEvent *e) {
    emit cancelSignal();
}
