#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class GCode;
class GCodeModel;
class GMovesModel;
class GGraphicsProxy;

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

    Ui::MainWindow *ui;
    
    GCode *mGCode;
    
    GCodeModel *mCodeModel;
    GMovesModel *mMovesModel;
    GGraphicsProxy *mGraphicsProxy;
    
    enum { MaxRecentFiles = 5 };
    QAction *mRecentFileActs[MaxRecentFiles];
};

#endif // MAINWINDOW_H
