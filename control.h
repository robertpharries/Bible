#ifndef CONTROL_H
#define CONTROL_H

#include <QDialog>
#include "mainwindow.h"
#include <vector>
#include <string>
#include "fontdialog.h"
#include "managetempldiag.h"

const static int WINDOW_LIMIT = 2;

class windowControl {
public:
    windowControl();
    ~windowControl();

    int add();
    MainWindow* get(int id);
    void rem(int id);
    bool hasFree();
    int count();
    int max();

private:
    MainWindow* newWin(int id);

    int _max = 2;

    MainWindow *mw_0 = NULL;
    MainWindow *mw_1 = NULL;
};


//Control

class Control : public QMainWindow
{
    Q_OBJECT

public:
    explicit Control(QWidget *parent = 0);
    ~Control();

public slots:
    void newWindow();
    void closeWindow(int);
    void moveAll(Location*, int);
    void scrollAll(int, int);

    void fontGui();
    void fontClose();
    void setFontAll(QString, int);

    void manageTemplGui();
    void manageTemplClose();
    void setTemplAll();
    void addedTempl(SearchTempl);

    void saveConf();
    void loadConf();

private:

    windowControl mwList;

    long inc = 0;

    FontDialog *fontWindow = NULL;
    ManageTemplDiag *manageTemplWindow = NULL;

    //fonts
    std::vector<QString> fonts;
    QFont currentFont;

    //templates
    std::vector<SearchTempl> stempls;
};

#endif // CONTROL_H
