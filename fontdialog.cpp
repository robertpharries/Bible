#include "fontdialog.h"
#include "ui_fontdialog.h"

FontDialog::FontDialog(QWidget *parent, vector<QString>* fonts, QFont *curFont) : QDialog(parent), ui(new Ui::FontDialog) {
    ui->setupUi(this);

    this->fontsList = fonts;

    TextSec preview;
    preview.len = 3;
    preview.sec.push_back("16'For God so loved the world that he gave his one and only Son, that whoever believes in him shall not perish but have eternal life. ");
    preview.sec.push_back("17For God did not send his Son into the world to condemn the world, but to save the world through him. ");
    preview.sec.push_back("18Whoever believes in him is not condemned, but whoever does not believe stands condemned already because he has not believed in the name of God's one and only Son. ");

    QTextBlockFormat format;
    format.setLineHeight(0, QTextBlockFormat::SingleHeight);

    QTextCursor curs = ui->plainTextEdit->textCursor();
    curs.setBlockFormat(format);

    //show book
    QString temp;

    for(int i = 0; i < preview.len; ++i) {
        string line = preview.sec.at(i);
        line.insert(0, "<sup>");

        for(int n = 5; n < 10; n++) {
            if(!isdigit((line.at(n)))) {
                line.insert(n, "</sup>");
                break;
            }
        }

        temp.append("<p>");
        temp.append(line.c_str());
        temp.append("</p>");
    }
    curs.insertHtml(temp);

    connect(ui->doneBtn, SIGNAL(clicked()), this, SLOT(doDone()));
    connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(doCancel()));
    connect(ui->fontList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(doChange()));
    connect(ui->fontSize, SIGNAL(valueChanged(int)), this, SLOT(doChange()));

    int idx = 0;
    for(int i = 0; i < fontsList->size(); i++) {
        ui->fontList->addItem(fontsList->at(i));
        if(fontsList->at(i) == curFont->family()) {
            idx = i;
        }
    }

    ui->fontList->setCurrentRow(idx);

    ui->fontSize->setValue(curFont->pointSize());
}

FontDialog::~FontDialog(){
    delete ui;
}

void FontDialog::closeEvent(QCloseEvent *) {
    emit closedSignalNP();
}

void FontDialog::doDone() {
    QModelIndexList resList = ui->fontList->selectionModel()->selectedIndexes();
    if(resList.size() > 0) {
        emit closedSignal(resList[0].data().toString(), ui->fontSize->value());
    } else {
        emit closedSignal("Arial", ui->fontSize->value());
    }
}

void FontDialog::doCancel() {
    emit closedSignalNP();
}

void FontDialog::doChange() {
    QModelIndexList resList = ui->fontList->selectionModel()->selectedIndexes();
    QFont font;

    if(resList.size() > 0) {
        font.setFamily(resList[0].data().toString());
        font.setPointSize(ui->fontSize->value());
    } else {
        font.setFamily("Arial");
        font.setPointSize(ui->fontSize->value());
    }

    ui->plainTextEdit->setFont(font);
}

