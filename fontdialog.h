#ifndef FONTDIALOG_H
#define FONTDIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include "bible.h"

namespace Ui {
class FontDialog;
}

class FontDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FontDialog(QWidget *parent = 0, vector<QString> *fonts = NULL, QFont *curFont = NULL);
    ~FontDialog();

public slots:
    void doDone();
    void doCancel();

    void doChange();

private:
    void closeEvent(QCloseEvent *);
    Ui::FontDialog *ui;

    vector<QString> *fontsList;

signals:
    void closedSignalNP();
    void closedSignal(QString, int);
};

#endif // FONTDIALOG_H
