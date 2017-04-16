#include "searchtemplnamediag.h"
#include "ui_searchtemplnamediag.h"

SearchTemplNameDiag::SearchTemplNameDiag(QWidget *parent) : QDialog(parent), ui(new Ui::SearchTemplNameDiag)
{
    ui->setupUi(this);

    connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(doCancel()));
    connect(ui->okBtn, SIGNAL(clicked()), this, SLOT(doOk()));
}

SearchTemplNameDiag::~SearchTemplNameDiag()
{
    delete ui;
}

void SearchTemplNameDiag::doOk() {
    QString *phrase = new QString(ui->name->text());

    emit closedSignal(phrase);
}

void SearchTemplNameDiag::doCancel() {
    emit closedSignalNP();
}

void SearchTemplNameDiag::closeEvent(QCloseEvent *e) {
    emit closedSignalNP();
}
