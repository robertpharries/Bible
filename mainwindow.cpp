#include "mainwindow.h"

#include "ui_mainwindow.h"

void outputLocation(BibleRec* curBible, int lineNum) {
    Location curLoc = curBible->getLocation(lineNum);
}

MainWindow::MainWindow(QWidget *parent, int ident) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    windowId = ident;

    ui->bibleText->setReadOnly(true);

    //setup actions
    this->bookState = 0;

    connect(ui->openBible, SIGNAL(triggered()), this, SLOT(loadBible()));
    connect(ui->search, SIGNAL(triggered()), this, SLOT(searchGui()));
    connect(ui->goTo, SIGNAL(triggered()), this, SLOT(gotoGui()));
    connect(ui->fontSettings, SIGNAL(triggered()), this, SLOT(fontGui()));
    connect(ui->manageTempl, SIGNAL(triggered()), this, SLOT(manageTemplGui()));
    connect(ui->quit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->openWindow, SIGNAL(triggered()), this, SLOT(openNewWindow()));
    connect(ui->bibleText, SIGNAL(cursorPositionChanged()), this, SLOT(textClicked()));

    connect(ui->bookCtrlNext, SIGNAL(pressed()), this, SLOT(nextBook()));
    connect(ui->bookCtrlBack, SIGNAL(pressed()), this, SLOT(prevBook()));

    connect(ui->bookSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(manualBookSelect(int)));
}

MainWindow::~MainWindow() {
    delete curBible;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if(searchWindow)
        searchWindow->close();
    if(resultsWindow)
        resultsWindow->close();
    if(gotoWindow)
        gotoWindow->close();

    emit closedSignal(windowId);
    event->accept();
}

void MainWindow::close() {

}

void MainWindow::openNewWindow() {
    emit newWin();
}

void MainWindow::loadBible() {
    QString filter = "Bible Text File (*.btx)";
    QString selection = QFileDialog::getOpenFileName(this, "Select a Bible text", QDir::currentPath(),filter);
    string str = selection.toStdString();

    std::size_t found = str.find_last_of(".");
    string path = str.substr(0, found);

    if(str.length() > 4) {
        BibleRec* newBible = new BibleRec(path + ".btx", path + ".idx");

        vector<string>* bookList = newBible->getBookList();

        if(bookList) {
            if(curBible)
                delete curBible;
            curBible = newBible;

            foreach(string str, *bookList) {
                ui->bookSelector->addItem(QString(str.c_str()));
            }

            bookState = 0;

            this->changeBook();
        } else {
            QMessageBox::information(this, "Bible Selection Invalid...", "Bible text selected is not valid");
        }
    }
}

void MainWindow::moveCursor(int lineNum) {
    int useLine = lineNum;
    if(useLine != 0) {
        useLine -= 1;
    }
    //scroll to bottom then back up to have selected line on the top of the screen
    QTextCursor cursor = ui->bibleText->textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    ui->bibleText->setTextCursor(cursor);
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    //scroll to the line before the one selected
    cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, useLine);
    ui->bibleText->setTextCursor(cursor);
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, lineNum);
    cursor.select(QTextCursor::LineUnderCursor);
    ui->bibleText->setTextCursor(cursor);
}

void MainWindow::textClicked() {
    cout << ui->bibleText->verticalScrollBar()->value() << endl;
    emit scrollSignal(ui->bibleText->verticalScrollBar()->value(), bookState);
}

void MainWindow::setScroll(int value, int bs) {
    if(!ui->lockMove->checkState()) {
        ui->bibleText->verticalScrollBar()->setValue(value);
        if(bookState != bs) {
            bookState = bs;
            changeBook();
        }
    }
}

void MainWindow::searchGui() {
    if(!searchWindow && curBible) {
        searchWindow = new SearchDiag(0, this->curBible, &this->stempls);
        connect(searchWindow, SIGNAL(addedTempl(SearchTempl)), this, SLOT(addedTempl(SearchTempl)));
        connect(searchWindow, SIGNAL(closedSignal(QString*, Location*, Location*)), this, SLOT(searchPhrase(QString*, Location*, Location*)));
        connect(searchWindow, SIGNAL(closedSignalNP()), this, SLOT(searchClose()));
        searchWindow->show();
    }
}

void MainWindow::searchClose() {
    delete searchWindow;
    searchWindow = NULL;
}

void MainWindow::searchPhrase(QString *phrase, Location *fromLoc, Location *toLoc) {
    delete searchWindow;
    searchWindow = NULL;

    vector<int> matched = curBible->searchPhrase(phrase->toStdString(), fromLoc->lineNum, (toLoc->lineNum)+1);
    if(matched.size() == 0) {
        QMessageBox msgBox;
        msgBox.setText("No Matches Found.");
        msgBox.exec();
    } else {
        if(!resultsWindow) {
            resultsWindow = new ResultsDiag(0, this->curBible, &matched, phrase->toStdString());
            connect(resultsWindow, SIGNAL(selectedSignal(int)), this, SLOT(matchSelected(int)));
            connect(resultsWindow, SIGNAL(cancelSignal()), this, SLOT(resultsClose()));
            resultsWindow->show();
        }
    }
}

void MainWindow::matchSelected(int lineNum) {
    Location *newLoc = new Location(this->curBible->getLocation(lineNum));
    emit moveSignal(newLoc, windowId);
    delete resultsWindow;
    resultsWindow = NULL;
}

void MainWindow::resultsClose() {
    delete resultsWindow;
    resultsWindow = NULL;
}

void MainWindow::gotoGui() {
    if(!gotoWindow && curBible) {
        gotoWindow = new gotodiag(0, this->curBible);
        connect(gotoWindow, SIGNAL(closedSignal(Location*)), this, SLOT(dogoto(Location*)));
        connect(gotoWindow, SIGNAL(closedSignalNP()), this, SLOT(gotoClose()));
        gotoWindow->show();
    }
}

void MainWindow::gotoClose() {
    delete gotoWindow;
    gotoWindow = NULL;
}

void MainWindow::dogoto(Location* newLoc) {
    emit moveSignal(newLoc, windowId);
    delete gotoWindow;
    gotoWindow = NULL;
}

void MainWindow::gotoLocation(Location* newLoc, int origId) {
    if(!ui->lockMove->checkState() || origId == windowId) {
        bookState = newLoc->bookIdx;
        this->changeBook();

        int relLineNum = newLoc->lineNum - curBible->getBookInfo(bookState).getStartLine();

        this->moveCursor(relLineNum);
    }
}

void MainWindow::nextBook() {
    if(this->curBible) {
        //change the state
        if(this->bookState < this->curBible->getBookNum()-1) {
            this->bookState++;
        }

        this->changeBook();
    }
}

void MainWindow::prevBook() {
    if(this->curBible) {
        //change the state
        if(this->bookState > 0) {
            this->bookState--;
        }

        this->changeBook();
    }
}

void MainWindow::changeBook() {
    if(curBible) {
        //edit the label
        QString bookName = this->curBible->getBookInfo(bookState).getName().c_str();
        ui->bookSelector->setCurrentIndex(bookState);

        //set the new text
        ui->bibleText->clear();

        QTextBlockFormat format;
        format.setLineHeight(0, QTextBlockFormat::SingleHeight);

        QTextCursor curs = ui->bibleText->textCursor();
        curs.setBlockFormat(format);

        //show book
        TextSec newBook = curBible->getBookText(bookState);
        QString temp;

        string tag = "<v style='color:#a40013'>";

        for(int i = 0; i < newBook.len; ++i) {
            string line = newBook.sec.at(i);
            line.insert(0, tag);

            for(int n = tag.length(); n < tag.length()+10; n++) {
                if(!(isdigit((line.at(n))) || line.at(n) == ':')) {
                    line.insert(n, " </v>");
                    break;
                }
            }

            temp.append("<p>");
            temp.append(line.c_str());
            temp.append("</p>");
        }
        curs.insertHtml(temp);

        this->moveCursor(0);
    }
}

void MainWindow::manualBookSelect(int index) {
    bookState = index;
    changeBook();
}

void MainWindow::fontGui() {
    emit fontGuiSignal();
}

void MainWindow::setFont(QFont font) {
    ui->bibleText->setFont(font);
}

void MainWindow::manageTemplGui() {
    emit manageTemplSignal();
}

void MainWindow::setTemplList(std::vector<SearchTempl> templList) {
    stempls = templList;
}

void MainWindow::addedTempl(SearchTempl addedTempl) {
    emit addedTemplSignal(addedTempl);
}
