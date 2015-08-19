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
#include "castandcrewlinks.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int argc, char *argv[], QWidget *parent = 0);
    void openLibrary(const QString &fileName);
protected:
    void closeEvent(QCloseEvent *event);
signals:
    void playMovie(QTreeWidgetItem* item, int col);
private slots:
    void newFile();

    void importFiles();
    void updateCoverFlow(QTreeWidgetItem* item,int col);
    bool save();
    bool saveAs();
    void callGetData();
    void columns();
    void getLibraryFile();
    void getFilmData(QList<QTreeWidgetItem*> items);
    void filmDataDownloaded(GetMovieData *movieData);
    void treeSelectionChanged();
    void listSelectionChanged(QModelIndex index);
    void currentSlideChanged(int index);
    void dataSorted(int col, Qt::SortOrder);
    void listViewToggled(bool checked);
    void iconViewToggled(bool checked);
    void coverFlowViewToggled(bool checked);
    void play();
    void coverItemActivated(int index);
    void selectCover();
    void removeMovie();
    void timerExpired();
    void sidebarTimerExpired();
    void openInNewWindow();
    void callOpen();

private:
    void createActions();
    void createMenus();
    void createContextMenu();
    void createConnections();
    void createToolBars();\
    void setupPictureFlow();
    bool okToContinue();
    bool saveLibrary(const QString &fileName);
    void writeSettings(QXmlStreamWriter *writer);

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
    QHash<QTreeWidgetItem*, CastAndCrewLinks*> itemLinksHash;
    QString imageFolder;
    QScrollArea *scrollArea;
    SideBar *sidebar;
    QTimer *sidebarTimer;
    QTimer *timer;
};

#endif // MAINWINDOW_H
