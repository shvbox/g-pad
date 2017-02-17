#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gcode.h"
#include "gline.h"
#include "gcodemodel.h"
#include "gmovesmodel.h"
#include "ggraphicsmodel.h"

#include "gcodehighlighterdelegate.h"
#include "gtableview.h"
#include "ggraphicsview.h"

#include <QtWidgets>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    GCodeHighlighterDelegate* codeHighlighter = new GCodeHighlighterDelegate();
    ui->codeView->setItemDelegateForColumn(GCodeModel::GCodeLineColumn, codeHighlighter);
    
    // Create data
    mGCode = new GCode(this);
    
    // Create G-Code View model
    mCodeModel = new GCodeModel(mGCode, this);
    ui->codeView->setModel(mCodeModel);
    
    // Create Moves View model
    mMovesModel = new GMovesModel(mGCode, this);
    ui->movesView->setModel(mMovesModel);
    
    // Create Graphics View model

    mGraphicsModel = new GGraphicsModel(mGCode, this);
    ui->graphicsView->setScene(mGraphicsModel);
    
    // Restore window settings
    for (int i = 0; i < MaxRecentFiles; ++i) {
        mRecentFileActs[i] = new QAction(this);
        mRecentFileActs[i]->setVisible(false);
        connect(mRecentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }
    readSettings();
    for (int i = 0; i < MaxRecentFiles; ++i) {
        ui->menuRecentFiles->addAction(mRecentFileActs[i]);
    }
    
    
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(mCodeModel, SIGNAL(modelReset()), this, SLOT(resizeTableColumns()));
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::openFile(const QString &path)
{
    QString fileName = path;

    if (fileName.isNull()) {
        fileName = QFileDialog::getOpenFileName(this,
            tr("Open G-Code"), "", tr("G-Code Files (*.gcode)"));
    }

    if (!fileName.isEmpty()) {
        loadFile(fileName);
    }
}

void MainWindow::loadFile(const QString &path)
{
    if (!mGCode->read(path)) {
        QMessageBox::warning(this, tr("Open G-Code"),
                             tr("The g-code file could not be loaded."),
                             QMessageBox::Cancel);
        return;
    }
    setCurrentFile(path);
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        loadFile(action->data().toString());
    }
}

void MainWindow::syncMovesViewScroll(int val)
{
    int movesVal = mGCode->lineToMove(val, true);
    ui->movesView->verticalScrollBar()->setValue(movesVal < 0 ? mMovesModel->rowCount() : movesVal);
}

void MainWindow::syncCodeViewScroll(int val)
{
    int codeVal = mGCode->moveToLine(val);
    ui->codeView->verticalScrollBar()->setValue(codeVal);
}

void MainWindow::test3(const QModelIndex &/*index*/)
{
//    int row = index.row();
//    if (row != prevRow || keyMod == 0) {
//        return;
//    }
    
//    switch (keyMod) {
//    case Qt::SHIFT: {
//        gCode->select(row);
//    }
//        break;
//    case Qt::CTRL: {
//        gCode->deselect(row);
//    }
//        break;
//    case Qt::ALT: {
//        gCode->toggleSelection(row);
//    }
//        break;
//    default:
//        break;
//    }
}

bool MainWindow::maybeSave()
{
//    if (textEdit->document()->isModified()) {
//        QMessageBox::StandardButton ret;
//        ret = QMessageBox::warning(this, tr("Application"),
//                     tr("The document has been modified.\n"
//                        "Do you want to save your changes?"),
//                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
//        if (ret == QMessageBox::Save)
//            return save();
//        else if (ret == QMessageBox::Cancel)
//            return false;
//    }
    return true;
}

void MainWindow::resizeTableColumns()
{
    // Setup unsynchronous scrolling
    disconnect(ui->codeView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(syncMovesViewScroll(int)));
    disconnect(ui->movesView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(syncCodeViewScroll(int)));

    // Resize columns
    ui->codeView->scrollToBottom();

    ui->movesView->resizeColumnToContents(GMovesModel::LineNumberColumn);
//    ui->movesView->resizeColumnToContents(GMovesModel::CodeColumn);
    
    ui->codeView->resizeColumnToContents(GCodeModel::LineNumberColumn);
    ui->codeView->resizeColumnToContents(GCodeModel::GCodeLineColumn);
    
    ui->codeView->scrollToTop();
    
    // Setup synchronous scrolling
    connect(ui->codeView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(syncMovesViewScroll(int)));
    connect(ui->movesView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(syncCodeViewScroll(int)));
}

void MainWindow::readSettings()
{
    QSettings s;
    restoreGeometry(s.value("MainWindow/geometry").toByteArray());
    
    restoreGeometry(s.value("MainWindow/geometry").toByteArray());
    if (isMaximized()) {
        setGeometry(QApplication::desktop()->availableGeometry(this));
    }
    
    restoreState(s.value("MainWindow/state").toByteArray());
        
    QByteArray headers = s.value("MovesView/headers", "").toByteArray();
    ui->movesView->horizontalHeader()->restoreState(headers);
    
    updateRecentFileActions();
}

void MainWindow::writeSettings()
{
    QSettings s;
    s.setValue("MainWindow/geometry", saveGeometry());
    s.setValue("MainWindow/state", saveState());
    
    QByteArray headers = ui->movesView->horizontalHeader()->saveState();
    s.setValue("MovesView/headers", headers);
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    QSettings s;
    setWindowTitle(fileName + " - " + s.applicationName());
    
    QStringList files = s.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles) {
        files.removeLast();
    }

    s.setValue("recentFileList", files);

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin) {
            mainWin->updateRecentFileActions();
        }
    }
}

void MainWindow::updateRecentFileActions()
{
    QSettings s;
    QStringList files = s.value("recentFileList").toStringList();
    int numRecentFiles = qMin(files.size(), (int) MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        mRecentFileActs[i]->setText(text);
        mRecentFileActs[i]->setData(files[i]);
        mRecentFileActs[i]->setVisible(true);
    }
    
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j) {
        mRecentFileActs[j]->setVisible(false);
    }

    ui->menuRecentFiles->setEnabled(numRecentFiles > 0);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

bool MainWindow::eventFilter(QObject */*obj*/, QEvent */*event*/)
{
    return false;
}
