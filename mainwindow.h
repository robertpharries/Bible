#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTextBlock>
#include <QCloseEvent>
#include <QScrollBar>
#include <vector>
#include <QFileDialog>
#include "searchdiag.h"
#include "resultsdiag.h"
#include "gotodiag.h"
#include "bible.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, int ident = -1);
    ~MainWindow();

public slots:
    void openNewWindow();

    void loadBible();    
    void searchGui();
    void searchClose();
    void searchPhrase(QString*, Location*, Location*);
    void matchSelected(int);
    void resultsClose();
    void manualBookSelect(int);
    void textClicked();
    void setScroll(int, int);

    void close();
    void closeEvent(QCloseEvent *event);

    void gotoGui();
    void gotoClose();
    void dogoto(Location*);

    void gotoLocation(Location*, int origId = -1);

    void nextBook();
    void prevBook();

    void fontGui();
    void setFont(QFont);

    void manageTemplGui();
    void setTemplList(std::vector<SearchTempl>);
    void addedTempl(SearchTempl);

private:
    void moveCursor(int);
    void changeBook();
    int windowId = -1;
    Ui::MainWindow *ui;
    SearchDiag *searchWindow = NULL;
    ResultsDiag *resultsWindow = NULL;
    gotodiag *gotoWindow = NULL;

    BibleRec *curBible = NULL;

    //remembers the current book displayed
    int bookState;

    //list of search templates
    std::vector<SearchTempl> stempls;

signals:
    void newWin();
    void moveSignal(Location*, int);
    void closedSignal(int);
    void scrollSignal(int, int);
    void fontGuiSignal();
    void manageTemplSignal();
    void addedTemplSignal(SearchTempl);
};

#endif // MAINWINDOW_H
