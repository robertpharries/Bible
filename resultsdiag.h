#ifndef RESULTSDIAG_H
#define RESULTSDIAG_H

#include <QDialog>
#include <QTableWidget>
#include <QClipboard>
#include <string>
#include <sstream>
#include <cstdlib>
#include "bible.h"

namespace Ui {
class ResultsDiag;
}

class ResultsDiag : public QDialog
{
    Q_OBJECT

public:
    explicit ResultsDiag(QWidget *parent = 0, BibleRec *newBible = NULL, vector<int> *matched = NULL, string phrase = "");
    ~ResultsDiag();

public slots:
    void changePreview();
    void select();
    void select(int, int);
    void copyAll();
    void cancel();

private:
    void closeEvent(QCloseEvent *);

    Ui::ResultsDiag *ui;
    BibleRec *curBible = NULL;
    vector<int> *resultsList = NULL;
    QClipboard *clip = NULL;

signals:
    void selectedSignal(int);
    void cancelSignal();
};

#endif // RESULTSDIAG_H
