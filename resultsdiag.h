#ifndef RESULTSDIAG_H
#define RESULTSDIAG_H

#include <QDialog>
#include <QTableWidget>
#include <string>
#include <sstream>
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
    void cancel();

private:
    Ui::ResultsDiag *ui;
    BibleRec *curBible = NULL;
    vector<int> *resultsList = NULL;

signals:
    void selectedSignal(int);
    void cancelSignal();
};

#endif // RESULTSDIAG_H
