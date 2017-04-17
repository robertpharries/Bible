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

            mw->show();
        }
    }
}

void Control::closeWindow(int id) {
    mwList.rem(id);
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

