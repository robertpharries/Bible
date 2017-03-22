#ifndef GOTODIAG_H
#define GOTODIAG_H

#include <QDialog>
#include <QCloseEvent>
#include "bible.h"

namespace Ui {
class gotodiag;
}

class gotodiag : public QDialog
{
    Q_OBJECT

public:
    explicit gotodiag(QWidget *parent = 0, BibleRec *curBible = NULL);
    ~gotodiag();

public slots:
    void doCancel();
    void doGo();
    void setFromChapters(int);
    void setFromVerses(int);

private:
    void closeEvent(QCloseEvent*);
    Ui::gotodiag *ui;

    BibleRec *curBible = NULL;

signals:
    void closedSignal(Location*);
    void closedSignalNP();
};

#endif // GOTODIAG_H
