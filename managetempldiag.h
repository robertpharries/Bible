#ifndef MANAGETEMPLDIAG_H
#define MANAGETEMPLDIAG_H

#include <QDialog>
#include "bible.h"
#include <QTableWidget>
#include <sstream>

namespace Ui {
class ManageTemplDiag;
}

class ManageTemplDiag : public QDialog
{
    Q_OBJECT

public:
    explicit ManageTemplDiag(QWidget *parent = 0, std::vector<SearchTempl> *newstempl = NULL);
    ~ManageTemplDiag();

public slots:
    void doClose();
    void doDelete();

    void loadTable();

private:
    void closeEvent(QCloseEvent*);
    Ui::ManageTemplDiag *ui;

    std::vector<SearchTempl> *stempl = NULL;

signals:
    void closedSignalNP();
};

#endif // MANAGETEMPLDIAG_H
