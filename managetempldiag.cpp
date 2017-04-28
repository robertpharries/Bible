#include "managetempldiag.h"
#include "ui_managetempldiag.h"

ManageTemplDiag::ManageTemplDiag(QWidget *parent, BibleRec *newBible, std::vector<SearchTempl> *newstempl) : QDialog(parent), ui(new Ui::ManageTemplDiag)
{
    ui->setupUi(this);

    stempl = newstempl;
    curBible = newBible;

    connect(ui->closeBtn, SIGNAL(clicked()), this, SLOT(doClose()));
    connect(ui->deleteBtn, SIGNAL(clicked()), this, SLOT(doDelete()));

    loadTable();
}

ManageTemplDiag::~ManageTemplDiag()
{
    delete ui;
}

void ManageTemplDiag::doClose() {
    emit closedSignalNP();
}

void ManageTemplDiag::closeEvent(QCloseEvent *e) {
    emit closedSignalNP();
}

void ManageTemplDiag::doDelete() {
    QModelIndexList indexes = ui->listView->selectionModel()->selection().indexes();
    QModelIndex index = indexes.at(0);
    int curRow = index.row();

    stempl->erase(stempl->begin() + curRow+3);

    loadTable();
}

void ManageTemplDiag::loadTable() {
    ui->listView->clear();
    QStringList headers;
    headers << "Name" << "From" << "To";
    ui->listView->setColumnCount(3);
    ui->listView->setHorizontalHeaderLabels(headers);
    ui->listView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listView->setRowCount(stempl->size()-3);

    for(int i = 3; i < stempl->size(); i++) {
        QTableWidgetItem* item = new QTableWidgetItem(QString(stempl->at(i).name.c_str()));
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->listView->setItem(i-3, 0, item);

        std::ostringstream chapt, verse;

        Location f_curLoc = curBible->getLocation(stempl->at(i).f_bookIdx, stempl->at(i).f_chapter, stempl->at(i).f_verse);
        Location t_curLoc = curBible->getLocation(stempl->at(i).t_bookIdx, stempl->at(i).t_chapter, stempl->at(i).t_verse);

        chapt << stempl->at(i).f_chapter;
        verse << stempl->at(i).f_verse;
        QString fromStr = (f_curLoc.bookName + ", " + chapt.str() + ", " + verse.str()).c_str();
        item = new QTableWidgetItem(fromStr);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->listView->setItem(i-3, 1, item);

        chapt.str("");
        verse.str("");
        chapt << stempl->at(i).t_chapter;
        verse << stempl->at(i).t_verse;
        QString toStr = (t_curLoc.bookName + ", " + chapt.str() + ", " + verse.str()).c_str();
        item = new QTableWidgetItem(toStr);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->listView->setItem(i-3, 2, item);

        ui->listView->resizeColumnsToContents();
    }
}
