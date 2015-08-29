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
#include <QCompleter>
#include <QProgressBar>
#include <QStatusBar>
#include <QToolButton>
#include "tree.h"
#include "pictureflow.h"
#include "getmoviedata.h"
#include "getmoviedatapromptdialog.h"
#include "getseriesdataprompt.h"
#include "columneditdialog.h"
#include "sidebar.h"
#include "castandcrewlinks.h"
#include "ui_Options.h"
#include "ui_GetSeriesName.h"
class ListView : public QListView{
    Q_OBJECT
public:
    ListView(QWidget *parent = 0) :QListView(parent){}
signals:
    void selectionChanged(QModelIndex index);
private:
    void keyPressEvent(QKeyEvent *e){
        QListView::keyPressEvent(e);
        if(e->key()==Qt::Key_Up ||e->key()==Qt::Key_Down ||e->key()==Qt::Key_Left ||e->key()==Qt::Key_Right)
            emit selectionChanged(currentIndex());
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int argc, char *argv[], QWidget *parent = 0);
    ~MainWindow();
    void openLibrary(const QString &fileName);
protected:
    void closeEvent(QCloseEvent *event);
signals:
    void playMovie(QTreeWidgetItem* item, int col);
    void playEpisode(QTreeWidgetItem* item, int col);
    void listSelectionChange(QModelIndex index);
private slots:
    void callEdit();
    void callGetData();
    void callOpen();
    void columns();
    void updateProgressBar(int steps);
    void coverFlowViewToggled(bool checked);
    void coverItemActivated(int index);
    void seriesCoverItemActivated(int index);
    void currentSlideChanged(int index);
    void dataSorted(int col, Qt::SortOrder);
    void itemExpanded(QTreeWidgetItem* item);
    void itemCollapsed(QTreeWidgetItem* item);
    void newFile();
    void importFiles();
    void importSeriesEpisodes();
    void updateCoverFlow(QTreeWidgetItem* item,int col);
    bool save();
    bool saveAs();
    void getLibraryFile();
    void getFilmData(QList<QTreeWidgetItem*> items);
    void getSeriesData(QList<QTreeWidgetItem*> items);
    void filmDataDownloaded(GetMovieData *movieData);
    void treeSelectionChanged();

    void listSelectionChanged(QModelIndex index);

    void listViewToggled(bool checked);
    void iconViewToggled(bool checked);
    void play();
    void selectCover();
    void removeMovie();
    void timerExpired();
    void sidebarTimerExpired();
    void openInNewWindow();


private:
    void createActions();
    void createConnections();
    void createContextMenu();
    void createMenus();
    void createStatusBar();
    void createToolBars();
    void setupPictureFlow();
    void setupSeriesPictureFlow();
    bool okToContinue();
    bool saveLibrary(const QString &fileName);
    void writeSettings(QXmlStreamWriter *writer);
    void setupMovieTab();
    void setupSeriesTab();
    GetMovieDataPromptDialog *movieDataPrompt;
    GetSeriesDataPrompt *seriesDataPrompt;
    QMap<GetMovieData*, QTreeWidgetItem*> filmDataItemMap;
    QList<QString> unsuccessfulDataDL;

    QMenu *fileMenu, *editMenu, *viewMenu, *openMenu;
    QToolBar *libraryToolbar;
    QToolBar *fileToolbar;
    QAction *saveAction, *saveAsAction, *newAction,
    *openAction, *importAction, *playAction, *columnAction,
    *getFilmDataAction, *deleteAction, *selectCoverAction, *listViewAction,
    *iconViewAction, *pictureFlowAction, *inNewWindow, *here, *edit, *cancelAction;
    QToolButton *canceldownload;

    int downloading, finished;
    ListView *list;
    ListView *seriesList;
    Tree *tree;
    Tree *seriesTree;
    PictureFlow *pictureFlow;
    PictureFlow *seriesPictureFlow;
    QSplitter *page;
    QSplitter *seriesPage;
    QScrollArea *scrollArea;
    QScrollArea *seriesScrollArea;
    SideBar *sidebar;
    SideBar *seriesSidebar;
    QTimer *sidebarTimer;
    QTimer *seriesSidebarTimer;
    QTimer *timer;
    QTimer *seriesTimer;
    QAbstractItemModel *seriesData;
    QAbstractItemModel *data;
    QItemSelectionModel *selections;
    QItemSelectionModel *seriesSelections;
    QTabWidget *tabwidget;
    QString curFile;
    QString openFile;
    QString app;
    QProgressBar *downloadProgress;


    bool seriesPictureFlowTopLevel;
    QHash<QTreeWidgetItem*, int> pictureFlowHash;
    QHash<QTreeWidgetItem*, CastAndCrewLinks*> itemLinksHash;
    QList<QString> seriesIDs;
    QString imageFolder;
    QTreeWidgetItem* expanded;

};

#endif // MAINWINDOW_H
