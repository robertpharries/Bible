#include "control.h"

Control::Control(QWidget *parent) : QMainWindow(parent)
{
    mwList = windowControl();

    this->fonts.push_back(QString("Arial"));
    this->fonts.push_back(QString("Courier New"));
    this->fonts.push_back(QString("Helvetica"));
    this->fonts.push_back(QString("Times New Roman"));

    loadConf();

    this->newWindow();
}

Control::~Control()
{

}

void Control::newWindow() {
    if(mwList.hasFree()) {
        int idx = mwList.add();

        MainWindow* mw = mwList.get(idx);

        if(mw) {
            connect(mw, SIGNAL(newWin()), this, SLOT(newWindow()));
            connect(mw, SIGNAL(closedSignal(int)), this, SLOT(closeWindow(int)));
            connect(mw, SIGNAL(moveSignal(Location*, int)), this, SLOT(moveAll(Location*, int)));
            connect(mw, SIGNAL(scrollSignal(int, int)), this, SLOT(scrollAll(int, int)));
            connect(mw, SIGNAL(fontGuiSignal()), this, SLOT(fontGui()));
            connect(mw, SIGNAL(manageTemplSignal()), this, SLOT(manageTemplGui()));
            connect(mw, SIGNAL(addedTemplSignal(SearchTempl)), this, SLOT(addedTempl(SearchTempl)));

            mw->setTemplList(stempls);
            mw->setFont(currentFont);
            mw->show();
        }
    }
}

void Control::closeWindow(int id) {
    mwList.rem(id);

    if(mwList.count() == 0) {
        saveConf();
    }
}

void Control::moveAll(Location* newLoc, int id) {
    for(int i = 0; i < mwList.max(); i++) {
        if(mwList.get(i)) {
            mwList.get(i)->gotoLocation(newLoc, id);
        }
    }
}

void Control::scrollAll(int value, int bs) {
    for(int i = 0; i < mwList.max(); i++) {
        if(mwList.get(i)) {
            mwList.get(i)->setScroll(value, bs);
        }
    }
}

//font gui

void Control::fontGui() {
    if(!fontWindow) {
        fontWindow = new FontDialog(0, &this->fonts, &currentFont);
        connect(fontWindow, SIGNAL(closedSignal(QString,int)), this, SLOT(setFontAll(QString,int)));
        connect(fontWindow, SIGNAL(closedSignalNP()), this, SLOT(fontClose()));
        fontWindow->show();
    }
}

void Control::fontClose() {
    if(fontWindow) {
        delete fontWindow;
        fontWindow = NULL;
    }
}

void Control::setFontAll(QString fontFamily, int fontSize) {
    QFont font;
    font.setFamily(fontFamily);
    font.setPointSize(fontSize);

    currentFont = font;

    for(int i = 0; i < mwList.max(); i++) {
        if(mwList.get(i)) {
            mwList.get(i)->setFont(font);
        }
    }

    if(fontWindow) {
        delete fontWindow;
        fontWindow = NULL;
    }
}

//templates

void Control::manageTemplGui() {
    if(!manageTemplWindow) {
        manageTemplWindow = new ManageTemplDiag(0, &stempls);
        connect(manageTemplWindow, SIGNAL(closedSignalNP()), this, SLOT(manageTemplClose()));
        manageTemplWindow->show();
    }
}

void Control::manageTemplClose() {
    this->setTemplAll();

    if(manageTemplWindow) {
        delete manageTemplWindow;
        manageTemplWindow = NULL;
    }
}

void Control::setTemplAll() {
    for(int i = 0; i < mwList.max(); i++) {
        if(mwList.get(i)) {
            mwList.get(i)->setTemplList(stempls);
        }
    }
}

void Control::addedTempl(SearchTempl newTempl) {
    stempls.push_back(newTempl);
    this->setTemplAll();
}

//config

void Control::loadConf() {
    ifstream confins;
    QFont font;
    confins.open("Bible_Gui.conf");

    char buffer[128];
    confins.getline(buffer, 128, '\n');

    font.setFamily(QString(buffer));

    confins.getline(buffer, 128, '\n');
    font.setPointSize(atoi(buffer));

    currentFont = font;

    //go load the templates
    SearchTempl temp;
    temp = (SearchTempl) {"Entire Bible", "Genesis", "Revelation", 0, 1, 1, 65, 22, 21};
    stempls.push_back(temp);
    temp = (SearchTempl) {"Old Testament", "Genesis", "Malachi", 0, 1, 1, 38, 4, 6};
    stempls.push_back(temp);
    temp = (SearchTempl) {"New Testament", "Matthew", "Revelation", 39, 1, 1, 65, 22, 21};
    stempls.push_back(temp);

    confins.getline(buffer, 128, '\n');

    int templNum = atoi(buffer);

    for(int i = 0; i < templNum; i++) {
        string name, x, y;
        int a, b, c, d, e, f;

        confins >> name >> x >> y >> a >> b >> c >> d >> e >> f;

        temp = (SearchTempl) {name,x,y,a,b,c,d,e,f};

        stempls.push_back(temp);
    }

}

void Control::saveConf() {
    cout << "saving config" << endl;
    ofstream confos;
    confos.open("Bible_Gui.conf", ios::trunc);

    confos << currentFont.family().toStdString() << '\n' << currentFont.pointSize() << endl;

    if(stempls.size()-3 > 0) {
        confos << stempls.size()-3 << endl;

        for(int i = 0; i < stempls.size()-3; i++) {
            confos << stempls.at(i+3).name << " " << stempls.at(i+3).f_bookName << " " << stempls.at(i+3).t_bookName <<  " " << stempls.at(i+3).f_bookIdx << " " << stempls.at(i+3).f_chapter << " " << stempls.at(i+3).f_verse << " "<< stempls.at(i+3).t_bookIdx << " " << stempls.at(i+3).t_chapter << " " << stempls.at(i+3).t_verse << endl;
        }
    }

    confos.close();
}



//window control

windowControl::windowControl() {

}

windowControl::~windowControl() {
    if(mw_0) {              //wtf
        delete mw_0;
    }
    if(mw_1) {
        delete mw_1;
    }
}

int windowControl::add() {
    if(!mw_0) {
        mw_0 = newWin(0);
        return 0;
    } else if(!mw_1) {
        mw_1 = newWin(1);
        return 1;
    }

    return -1;
}

MainWindow* windowControl::get(int id) {
    switch(id) {
    case 0:
        return mw_0; break;
    case 1:
        return mw_1; break;
    default:
        cout << "mw out of range" << endl; return NULL;
    }
}

void windowControl::rem(int id) {
    switch(id) {
    case 0:
        delete mw_0; mw_0 = NULL; break;
    case 1:
        delete mw_1; mw_1 = NULL; break;
    default:
        cout << "mw out of range" << endl;
    }
}

int windowControl::count() {
    int c = 0;

    if(mw_0)
        c++;
    if(mw_1)
        c++;
    return c;
}

int windowControl::max() {
    return _max;
}

bool windowControl::hasFree() {
    if(!mw_0 || !mw_1) {
        return true;
    }

    return false;
}

MainWindow* windowControl::newWin(int id) {
    return new MainWindow(0, id);
}

