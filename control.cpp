#include "control.h"

Control::Control(QWidget *parent) : QMainWindow(parent)
{
    mwList = windowControl();
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
            mw->show();
        }
    }
}

void Control::closeWindow(int id) {
    mwList.rem(id);
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


//window control

windowControl::windowControl() {

}

windowControl::~windowControl() {
    if(mw_0) {
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

