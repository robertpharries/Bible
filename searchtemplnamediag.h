#ifndef SEARCHTEMPLNAMEDIAG_H
#define SEARCHTEMPLNAMEDIAG_H

#include <QDialog>

namespace Ui {
class SearchTemplNameDiag;
}

class SearchTemplNameDiag : public QDialog
{
    Q_OBJECT

public:
    explicit SearchTemplNameDiag(QWidget *parent = 0);
    ~SearchTemplNameDiag();

public slots:
    void doOk();
    void doCancel();

private:
    void closeEvent(QCloseEvent*);

    Ui::SearchTemplNameDiag *ui;

signals:
    void closedSignalNP();
    void closedSignal(QString*);
};

#endif // SEARCHTEMPLNAMEDIAG_H
