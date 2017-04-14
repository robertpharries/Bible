#ifndef SEARCHDIAG_H
#define SEARCHDIAG_H

#include <QDialog>
#include <QCloseEvent>
#include "bible.h"

namespace Ui {
class SearchDiag;
}

class SearchDiag : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDiag(QWidget *parent = 0, BibleRec *curBible = NULL, std::vector<SearchTempl> *newstempl = NULL);
    ~SearchDiag();

public slots:
    void doSearch();
    void doCancel();
    void setFromChapters(int);
    void setToChapters(int);
    void setFromVerses(int);
    void setToVerses(int);

    void setCustom();
    void setTempl(int);

private:
    void closeEvent(QCloseEvent*);
    Ui::SearchDiag *ui;

    BibleRec *curBible = NULL;

    std::vector<SearchTempl> *stempl = NULL;
signals:
    void closedSignalNP();
    void closedSignal(QString*, Location*, Location*);
};

#endif // SEARCHDIAG_H
