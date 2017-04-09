#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class GCode;
class GNavigator;
class GCodeModel;
class GMovesModel;
class GNavigatorModel;
class GGraphicsProxy;
class GFilterProxy;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    // QObject interface
public:
    bool eventFilter(QObject *obj, QEvent *event);
    
public slots:
    void openFile(const QString &path = QString());
    void reloadFile();
    
private slots:
    void resizeTableColumns();
    void openRecentFile();
    void syncMovesViewScroll(int val);
    void syncCodeViewScroll(int val);
    void test3(const QModelIndex &index);
    
protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    
private:
    void readSettings();
    void writeSettings();
    bool maybeSave();
    
    void loadFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);
    QString mostRecentPath();

    Ui::MainWindow *ui;
    
    GCode *mGCode;
    GNavigator*mNavigator;
    
    GCodeModel *mCodeModel;
    GFilterProxy *mCodeProxy;
    
    GMovesModel *mMovesModel;
    GFilterProxy *mMovesProxy;
    GGraphicsProxy *mGraphicsProxy;
    
    GNavigatorModel *mNavModel;
    
    enum { MaxRecentFiles = 10 };
    QAction *mRecentFileActs[MaxRecentFiles];
    
    QString mCurrentFileName;
};

#endif // MAINWINDOW_H
