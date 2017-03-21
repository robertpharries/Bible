#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTextBlock>
#include <QCloseEvent>
#include "searchdiag.h"
#include "resultsdiag.h"
#include "bible.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);

public slots:
    void loadBible();
    void searchGui();
    void searchClose();
    void searchPhrase(QString*, Location*, Location*);
    void matchSelected(int);
    void resultsClose();
    void manualBookSelect(int);

    void nextBook();
    void prevBook();

private:
    void moveCursor(int);
    void changeBook();
    Ui::MainWindow *ui;
    SearchDiag *searchWindow = NULL;
    ResultsDiag *resultsWindow = NULL;
    BibleRec *curBible = NULL;

    //remembers the current book displayed
    int bookState;
};

#endif // MAINWINDOW_H
