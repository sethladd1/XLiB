#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QtGui>
#include <QMainWindow>
#include <QSysInfo>
#include <QMessageBox>
#include <QHeaderView>
#include <QFileDialog>
#include <QFileIconProvider>
#include <QMenuBar>
#include <QListView>
#include <QSplitter>
#include <QLineEdit>
#include <QToolBar>
#include <QUndoStack>

#include "tree.h"
#include "pictureflow.h"
#include "getmoviedata.h"
#include "getmoviedatapromptdialog.h"
#include "columneditdialog.h"
#include "sidebar.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int argc, char *argv[], QWidget *parent = 0); //d
    void openLibrary(const QString &fileName);
protected:
    void closeEvent(QCloseEvent *event); //d
signals:
    void playMovie(QTreeWidgetItem* item, int col);
private slots:
    void newFile(); //d

    void importFiles(); //d
    void updateCoverFlow(QTreeWidgetItem* item,int col);
    bool save(); //d
    bool saveAs(); //d
    void callGetData();
    void columns(); //d
    void getLibraryFile(); //d
    void getFilmData(QList<QTreeWidgetItem*> items); //d
    void filmDataDownloaded(GetMovieData *movieData); //d
    void treeSelectionChanged(); //d
    void listSelectionChanged(QModelIndex index); //d
    void currentSlideChanged(int index); //d
    void dataSorted(int col, Qt::SortOrder); //d
    void listViewToggled(bool checked); //d
    void iconViewToggled(bool checked); //d
    void coverFlowViewToggled(bool checked); //d
    void play(); //d
    void coverItemActivated(int index);
    void selectCover();
    void removeMovie();
    void timerExpired();
    void openInNewWindow();
    void callOpen();
private:
    void createActions(); //d
    void createMenus(); //d
    void createContextMenu(); //d
    void createConnections(); //d
    void createToolBars();\
    void setupPictureFlow(); //d
    bool okToContinue(); //d
    bool saveLibrary(const QString &fileName); //d
    void writeSettings(QXmlStreamWriter *writer); //d

    GetMovieDataPromptDialog *movieDataPrompt;
    QMap<GetMovieData*, QTreeWidgetItem*> filmDataItemMap;
    QList<QString> unsuccessfulDataDL;

    QMenu *fileMenu, *editMenu, *viewMenu, *openMenu;
    QToolBar *libraryToolbar;
    QToolBar *fileToolbar;
    QAction *saveAction, *saveAsAction, *newAction,
    *openAction, *importAction, *playAction, *columnAction,
    *getFilmDataAction, *deleteAction, *selectCoverAction, *listViewAction,
    *iconViewAction, *pictureFlowAction, *options, *inNewWindow, *here;
    QAbstractItemModel *data;
    int downloading, finished;
    QListView *list;
    Tree *tree;
    QTabWidget *tabwidget;
    PictureFlow *pictureFlow;
    QString curFile;
    QString openFile;
    QString app;
    QSplitter *page;
    QString type;
    QString imageFolder;
    QScrollArea *scrollArea;
    SideBar *sidebar;

    QTimer *timer;
};

#endif // MAINWINDOW_H
