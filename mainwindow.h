#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTextBlock>
#include <QCloseEvent>
#include <vector>
#include <QFileDialog>
#include "searchdiag.h"
#include "resultsdiag.h"
#include "gotodiag.h"
#include "fontdialog.h"
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

public slots:
    void loadBible();
    void searchGui();
    void searchClose();
    void searchPhrase(QString*, Location*, Location*);
    void matchSelected(int);
    void resultsClose();
    void manualBookSelect(int);

    void closeEvent(QCloseEvent *event);

    void gotoGui();
    void gotoClose();
    void gotoLocation(Location*);

    void nextBook();
    void prevBook();

    void fontGui();
    void fontClose();
    void setFont(QString, int);

    void saveConf();
    void loadConf();

private:
    void moveCursor(int);
    void changeBook();
    Ui::MainWindow *ui;
    SearchDiag *searchWindow = NULL;
    ResultsDiag *resultsWindow = NULL;
    gotodiag *gotoWindow = NULL;
    FontDialog *fontWindow = NULL;

    BibleRec *curBible = NULL;

    //remembers the current book displayed
    int bookState;

    //list of fonts availble
    std::vector<QString> fonts;
    //list of search templates
    std::vector<SearchTempl> stempls;
};

#endif // MAINWINDOW_H
