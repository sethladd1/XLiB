#include "mainwindow.h"
MainWindow::MainWindow(int argc, char *argv[], QWidget *parent) :
    QMainWindow(parent)
{
    app = QFileInfo(argv[0]).absoluteFilePath(); // So I can launch a new instance of this program later
    setWindowIcon(QIcon(":Icons/2-Movies-icon2.png"));
    tabwidget = new QTabWidget(this);
    setupMovieTab();
    setupSeriesTab();
    setCentralWidget(tabwidget);
    resize(700, maximumHeight());
    setWindowTitle("[*]untitled");
    setWindowModified(false);
    setMouseTracking(true);
    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    createConnections();
    createStatusBar();
    expanded = 0;
    tree->setFocus();
    downloading = 0;
    finished = 0;
    if(argc>1){
        QTimer *t = new QTimer(this);
        openFile = argv[argc-1];
        t->setSingleShot(true);
        t->start(2000);
        connect(t, SIGNAL(timeout()), this, SLOT(callOpen()));
    }
}
void MainWindow::setupMovieTab(){
    tree = new Tree(this);
    QStringList headers;
    headers.append("Title");
    headers.append("Year");
    headers.append("Director");
    headers.append("Starring");
    headers.append("Genre");
    headers.append("Rated");
    headers.append("Writer");
    headers.append("Country");
    headers.append("Language");
    headers.append("Size");
    headers.append("imdbID");
    headers.append("Plot");
    headers.append("Path");
    headers.append("Icon6154");
    headers.append("Type6154");
    tree->setHeaderItem(new QTreeWidgetItem(headers));
    QMap<QString, int> hMap = tree->getHeaderMap();
    tree->hideColumn(hMap.value("Icon6154"));
    tree->hideColumn(hMap.value("imdbID"));
    tree->hideColumn(hMap.value("Type6154"));
    tree->hideColumn(hMap.value("Plot"));
    tree->setSortingEnabled(true);
    tree->sortByColumn(0, Qt::AscendingOrder);
    data = tree->model();

    selections = new QItemSelectionModel(data);
    tree->setSelectionModel(selections);
    tree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tree->setIconSize(QSize(30,30));

    list = new ListView(this);
    list->setSpacing(5);
    list->setViewMode(QListView::IconMode);
    list->setIconSize(QSize(100,100));
    list->setResizeMode(QListView::Adjust);
    list->setWordWrap(true);
    list->setModel(data);
    list->setSelectionModel(selections);
    list->setSelectionMode(QAbstractItemView::ExtendedSelection);

    pictureFlow = new PictureFlow(this);
    pictureFlow->setSlideSize(QSize(400, 400));
    pictureFlow->setMinimumHeight(300);
    QSplitter *vertPage = new QSplitter(Qt::Vertical, this);
    timer = new QTimer(this);
    timer->setSingleShot(true);
    sidebarTimer = new QTimer(this);
    sidebarTimer->setSingleShot(true);
    list->hide();
    vertPage->addWidget(pictureFlow);
    vertPage->addWidget(tree);
    vertPage->addWidget(list);
    page = new QSplitter(Qt::Horizontal, this);
    sidebar = new SideBar(this);
    scrollArea = new QScrollArea(this);
    scrollArea->setMaximumWidth(270);
    scrollArea->setWidget(sidebar);
    scrollArea->resize(270, scrollArea->height());
    scrollArea->setBackgroundRole(QPalette::Light);
    page->addWidget(scrollArea);
    page->addWidget(vertPage);
    tabwidget->addTab(page, "Movies");
}
void MainWindow::setupSeriesTab(){
    seriesTree = new Tree(this);
    QStringList headers;
    headers.append("Title");
    headers.append("Year");
    headers.append("Season");
    headers.append("Episode");
    headers.append("Director");
    headers.append("Starring");
    headers.append("Genre");
    headers.append("Rated");
    headers.append("Writer");
    headers.append("Country");
    headers.append("Language");
    headers.append("Size");
    headers.append("imdbID");
    headers.append("Plot");

    headers.append("Path");
    headers.append("Icon6154");
    headers.append("Type6154");
    seriesTree->setHeaderItem(new QTreeWidgetItem(headers));
    QMap<QString, int> hMap = seriesTree->getHeaderMap();
    seriesTree->hideColumn(hMap.value("Icon6154"));
    seriesTree->hideColumn(hMap.value("imdbID"));
    seriesTree->hideColumn(hMap.value("Type6154"));
    seriesTree->hideColumn(hMap.value("Plot"));
    seriesTree->setSortingEnabled(true);
    seriesTree->sortByColumn(0, Qt::AscendingOrder);
    seriesData = seriesTree->model();

    seriesSelections = new QItemSelectionModel(seriesData);
    seriesTree->setSelectionModel(seriesSelections);
    seriesTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    seriesTree->setIconSize(QSize(30,30));

    seriesList = new ListView(this);
    seriesList->setSpacing(5);
    seriesList->setViewMode(QListView::IconMode);
    seriesList->setIconSize(QSize(100,100));
    seriesList->setResizeMode(QListView::Adjust);
    seriesList->setWordWrap(true);
    seriesList->setModel(seriesData);
    seriesList->setSelectionModel(seriesSelections);
    seriesList->setSelectionMode(QAbstractItemView::ExtendedSelection);

    seriesPictureFlow = new PictureFlow(this);
    seriesPictureFlow->setSlideSize(QSize(400, 400));
    seriesPictureFlow->setMinimumHeight(300);
    QSplitter *vertPage = new QSplitter(Qt::Vertical, this);
    seriesTimer = new QTimer(this);
    seriesTimer->setSingleShot(true);
    seriesSidebarTimer = new QTimer(this);
    seriesSidebarTimer->setSingleShot(true);
    seriesList->hide();
    vertPage->addWidget(seriesPictureFlow);
    vertPage->addWidget(seriesTree);
    vertPage->addWidget(seriesList);
    seriesPage = new QSplitter(Qt::Horizontal, this);
    seriesSidebar = new SideBar(this);
    seriesScrollArea = new QScrollArea(this);
    seriesScrollArea->setMaximumWidth(270);
    seriesScrollArea->setWidget(seriesSidebar);
    seriesScrollArea->resize(270, scrollArea->height());
    seriesScrollArea->setBackgroundRole(QPalette::Light);
    seriesPage->addWidget(seriesScrollArea);
    seriesPage->addWidget(vertPage);
    tabwidget->addTab(seriesPage, "Television");
}

void MainWindow::itemExpanded(QTreeWidgetItem *item){
    seriesTree->blockSignals(true);
    seriesTree->collapseAll();
    seriesTree->expandItem(item);
    seriesTree->blockSignals(false);
    expanded = item;
    seriesTree->setCurrentItem(item->child(0));
    setupSeriesPictureFlow();
    seriesPictureFlowTopLevel = false;
}

void MainWindow::itemCollapsed(QTreeWidgetItem* item){
    if(item == expanded){
        expanded = 0;
        if(!seriesPictureFlowTopLevel){
            seriesTree->setCurrentItem(item);
            setupSeriesPictureFlow();
            seriesPictureFlowTopLevel = true;
        }
    }
}

void MainWindow::importSeriesEpisodes(){
    Ui::GetSeries ui;

    QDialog *dlg = new QDialog(this);
    ui.setupUi(dlg);
    QStringList completerList;
    int col = seriesTree->getHeaderMap().value("Title");
    for(int i=0; i<seriesTree->topLevelItemCount(); ++i){
        completerList.append(seriesTree->topLevelItem(i)->text(col));
    }
    ui.lineEdit->setCompleter(new QCompleter(completerList, this));

    if(dlg->exec()){
        QStringList paths = QFileDialog::getOpenFileNames(this, "Import Files to Library");
        QString name;
        QTreeWidgetItem *parent;
        QMap<QString, int> hMap = seriesTree->getHeaderMap();
        bool found = false;
        for(int i=0; i<seriesTree->topLevelItemCount(); ++i){
            if(seriesTree->topLevelItem(i)->text(hMap.value("Title")).trimmed()==ui.lineEdit->text().trimmed()){
                parent=seriesTree->topLevelItem(i);
                found=true;
                break;
            }
        }
        if(!found){
            parent = new QTreeWidgetItem();
            parent->setText(0, ui.lineEdit->text());
        }
        QTreeWidgetItem *treeItem;
        QIcon icon;

        QList<QTreeWidgetItem*> items;
        for (int i = 0; i< paths.size(); ++i){
            name = QFileInfo(paths[i]).completeBaseName();
            treeItem = new QTreeWidgetItem();
            icon = QFileIconProvider().icon(QFileInfo(paths[i]));

            treeItem->setText(hMap.value("Path"), paths[i]);
            treeItem->setIcon(0,icon);
            treeItem->setText(hMap.value("Title"), name);
            double d = double(QFileInfo(paths[i]).size());
            QString size;
            if(d/1000000000 >= 1)
                size = QString().number(d/1000000000,'f',1) +"GB";
            else
                if(d/1000000 >= 1)
                    size = QString().number(d/1000000,'f',1) +"MB";
                else
                    if(d/1000 >= 1)
                        size = QString().number(d/1000,'f',1) +"KB";
                    else
                        size = QString().number(d,'f',1) +"B";
            treeItem->setText(hMap.value("Size"), size);
            items.append(treeItem);

        }
        if(items.size()){
            parent->addChildren(items);
            getSeriesData(items);
            seriesTree->addTopLevelItem(parent);
            setWindowModified(true);
            seriesTree->setCurrentItem(parent);
            seriesTree->blockSignals(true);
            seriesTree->collapseAll();
            seriesTree->blockSignals(false);
            setupSeriesPictureFlow();
        }
    }
}

void MainWindow::setupSeriesPictureFlow(){
    seriesPictureFlow->clear();
    QMap<QString, int> hMap = seriesTree->getHeaderMap();
    QTreeWidgetItem *item;
    if(seriesTree->currentItem() == 0)
        return;
    pictureFlowHash.clear();
    int picWidth = 0, picHeight = 0, scaledToHeight = .6*seriesPictureFlow->height(), count=0;
    if(seriesTree->currentItem()->childCount()){ //match to toplevel items
        for (int i =0; i<seriesTree->topLevelItemCount(); ++i){
            item = seriesTree->topLevelItem(i);
            if(!item->text(hMap.value("Icon6154")).isEmpty()){
                QPixmap pix(item->text(hMap.value("Icon6154")));
                pix = pix.scaledToHeight(scaledToHeight);
                picHeight+=pix.height();
                picWidth+=pix.width();
                ++count;
                seriesPictureFlow->setSlideCount(i+1);
                seriesPictureFlow->setSlide(i,pix);
                seriesPictureFlow->setSlideCaption(i,item->text(hMap.value("Title")));
            }
            else{
                seriesPictureFlow->setSlideCount(i+1);
                seriesPictureFlow->setSlide(i,QFileIconProvider().icon(QFileInfo(item->text(hMap.value("Path")))).pixmap(pictureFlow->slideSize()));
                seriesPictureFlow->setSlideCaption(i,item->text(hMap.value("Title")));
            }
            pictureFlowHash.insert(item, i);
        }
        seriesPictureFlow->setCurrentSlide(seriesTree->indexOfTopLevelItem(seriesTree->currentItem()));
        seriesPictureFlowTopLevel = true;
    }
    else{ //match to current items' siblings
        QTreeWidgetItem* parent = seriesTree->currentItem()->parent();
        int index;
        for (int i =0; i<parent->childCount(); ++i){
            item = parent->child(i);
            if(item == seriesTree->currentItem())
                index = i;
            if(!item->text(hMap.value("Icon6154")).isEmpty()){
                QPixmap pix(item->text(hMap.value("Icon6154")));
                pix = pix.scaledToHeight(scaledToHeight);
                picHeight+=pix.height();
                picWidth+=pix.width();
                ++count;
                seriesPictureFlow->setSlideCount(i+1);
                seriesPictureFlow->setSlide(i,pix);
                seriesPictureFlow->setSlideCaption(i,item->text(hMap.value("Title")));
            }
            else{
                seriesPictureFlow->setSlideCount(i+1);
                seriesPictureFlow->setSlide(i,QFileIconProvider().icon(QFileInfo(item->text(hMap.value("Path")))).pixmap(pictureFlow->slideSize()));
                seriesPictureFlow->setSlideCaption(i,item->text(hMap.value("Title")));
            }
            pictureFlowHash.insert(item, i);
        }
        seriesPictureFlow->setCurrentSlide(index);
    }
    if(seriesTree->currentItem() != 0)
        seriesSidebar->populate(seriesTree->currentItem(), seriesTree->getHeaderMap(), seriesTree->getLinks().value(seriesTree->currentItem()));
}
void MainWindow::setupPictureFlow(){
    pictureFlow->clear();
    int picWidth = 0, picHeight = 0, scaledToHeight = .6*pictureFlow->height(), count=0;
    QMap<QString, int> hMap = tree->getHeaderMap();
    QTreeWidgetItem *item;
    for (int i =0; i<tree->topLevelItemCount(); ++i){
        item = tree->topLevelItem(i);
        if(!item->text(hMap.value("Icon6154")).isEmpty()){
            QPixmap pix(item->text(hMap.value("Icon6154")));
            pix = pix.scaledToHeight(scaledToHeight);
            picHeight+=pix.height();
            picWidth+=pix.width();
            ++count;
            pictureFlow->setSlideCount(i+1);
            pictureFlow->setSlide(i, pix);
            pictureFlow->setSlideCaption(i,item->text(hMap.value("Title")));
        }
        else{
            pictureFlow->setSlideCount(i+1);
            pictureFlow->setSlide(i,QFileIconProvider().icon(QFileInfo(item->text(hMap.value("Path")))).pixmap(pictureFlow->slideSize()));
            pictureFlow->setSlideCaption(i,item->text(hMap.value("Title")));
        }
    }
    if(tree->currentItem() != 0){
        pictureFlow->setCurrentSlide(tree->indexOfTopLevelItem(tree->currentItem()));
        sidebar->populate(tree->currentItem(), tree->getHeaderMap(), tree->getLinks().value(tree->currentItem()));
    }
}

void MainWindow::updateCoverFlow(QTreeWidgetItem *item, int col){

    if(pictureFlowHash.contains(item)){
        int i = pictureFlowHash.value(item);
        if(seriesPictureFlow->slideCount() > i){
            setupSeriesPictureFlow();
        }
    } else{
        int i = tree->indexOfTopLevelItem(item);
        if(i<0) return;
        if(pictureFlow->slideCount() > i){
            if(item == tree->currentItem())
                sidebar->populate(item, tree->getHeaderMap(), tree->getLinks().value(item));
            QMap<QString, int> hMap = tree->getHeaderMap();
            if(col == 0 || col == hMap.value("Icon6154")){
                if(!item->text(hMap.value("Icon6154")).isEmpty()){
                    pictureFlow->setSlide(i,QPixmap(item->text(hMap.value("Icon6154"))));
                    pictureFlow->setSlideCaption(i,item->text(hMap.value("Title")));
                }
                else{
                    pictureFlow->setSlide(i,QFileIconProvider().icon(QFileInfo(item->text(hMap.value("Path")))).pixmap(pictureFlow->slideSize()));
                    pictureFlow->setSlideCaption(i,item->text(hMap.value("Title")));
                }
            }
        }
    }
}
void MainWindow::getSeriesData(QList<QTreeWidgetItem*> items){
    QHash<QString, QList<QTreeWidgetItem*> > seriesEpisodeHash;
    QMap<QString, int> hMap = seriesTree->getHeaderMap();
    QString series;
    QList<QTreeWidgetItem*> episodes;
    QStringList parents;
    bool showErrDlg = false;
    if(items.isEmpty())
        items = seriesTree->selectedItems();
    for(int i=0; i<items.size(); ++i){
        if(items[i]->childCount())
            items.removeAt(i);
        else{
            series = items[i]->parent()->text(hMap.value("Title"));
            if(!parents.contains(series))
                parents.append(series);
            if(seriesEpisodeHash.contains(series)){
                seriesEpisodeHash[series].append(items[i]);
            }
            else{
                episodes.clear();
                episodes.append(items[i]);
                seriesEpisodeHash.insert(series, episodes);
            }
        }
    }
    seriesDataPrompt = new GetSeriesDataPrompt(seriesEpisodeHash, hMap, imageFolder, this);
    if(seriesDataPrompt->exec()){
        if(downloading==0){
            downloading = items.size();
            finished = 0;
            downloadProgress->setMaximum(downloading*4 + parents.size()*4);
            downloadProgress->setValue(0);
        }
        else{
            downloading += items.size();
            downloadProgress->setMaximum(downloading*4 + parents.size()*4);
            statusBar()->show();
        }
        statusBar()->show();
        GetMovieData *movieData;
        QList<QPair<QTreeWidgetItem*, seriesLineEditStruct*> >getDataFor = seriesDataPrompt->lineEdits();
        imageFolder = seriesDataPrompt->getImageFolder();
        if(!QDir(imageFolder).exists())
            QDir().mkpath(seriesDataPrompt->getImageFolder());
        for(int i = 0; i<getDataFor.size(); ++i){
            if(getDataFor.at(i).second->imdbID->isEnabled() && !getDataFor.at(i).second->imdbID->text().isEmpty()){
                movieData = new GetMovieData(getDataFor.at(i).second->imdbID->text(), true, this);
                filmDataItemMap.insert(movieData, getDataFor.at(i).first);
                connect(movieData, SIGNAL(finishedDownloading(GetMovieData*)), this, SLOT(filmDataDownloaded(GetMovieData*)));
                connect(movieData, SIGNAL(finishedStep(int)), this, SLOT(updateProgressBar(int)));
                connect(cancelAction, SIGNAL(triggered()), movieData, SLOT(closeConnection()));
            }
            else
            {
                bool *ok = new bool;
                *ok = true;
                int season = getDataFor.at(i).second->season->text().toInt(ok,10);
                int episode = getDataFor.at(i).second->episode->text().toInt(ok,10);
                if(*ok){
                    movieData = new GetMovieData(getDataFor[i].first->parent()->text(hMap.value("Title")), season, episode, true, this);
                    filmDataItemMap.insert(movieData, getDataFor.at(i).first);
                    connect(movieData, SIGNAL(finishedDownloading(GetMovieData*)), this, SLOT(filmDataDownloaded(GetMovieData*)));
                    connect(movieData, SIGNAL(finishedStep(int)), this, SLOT(updateProgressBar(int)));
                    connect(cancelAction, SIGNAL(triggered()), movieData, SLOT(closeConnection()));
                }
                else
                    showErrDlg = true;
            }

        }
    }
    if(showErrDlg){
        QMessageBox::information(this,"Invalid Input", "Data search was not for some episodes due to invalid input.", QMessageBox::Ok);
    }
}
void MainWindow::treeSelectionChanged(){
    sidebarTimer->start(100);
    if(tabwidget->currentIndex()==0){

        if(pictureFlow->currentSlide() != tree->indexOfTopLevelItem(tree->currentItem())){
            pictureFlow->setCurrentSlide(tree->indexOfTopLevelItem(tree->currentItem()));
        }
    }
    else{
        QTreeWidgetItem *item = seriesTree->currentItem();
        if(item->childCount()){
            if(expanded != 0){
                if(expanded->isExpanded())
                    seriesTree->collapseItem(expanded);
                expanded = 0;
                setupSeriesPictureFlow();
            }
        }
        seriesPictureFlow->setCurrentSlide(pictureFlowHash[item]);
    }
}


void MainWindow::listSelectionChanged(QModelIndex index){
    sidebarTimer->start(100);
    if(pictureFlow->currentSlide() != index.row()){
        pictureFlow->setCurrentSlide(index.row());
    }
}

void MainWindow::currentSlideChanged(int index){
    timer->start(1000);

    if(tabwidget->currentIndex() == 0){

        if(!tree->selectedItems().contains(tree->topLevelItem(index))){
            tree->setCurrentItem(tree->topLevelItem(index));
        }
    } else{
        QTreeWidgetItem* pfItem = pictureFlowHash.key(seriesPictureFlow->currentSlide());
        if(!seriesTree->selectedItems().contains(pfItem)){
            seriesTree->setCurrentItem(pfItem);
        }
    }
}
void MainWindow::sidebarTimerExpired(){
    if(tabwidget->currentIndex() == 0){
        if(tree->currentItem() != tree->topLevelItem(pictureFlow->currentSlide()))
            tree->setCurrentItem(tree->topLevelItem(pictureFlow->currentSlide()));
        sidebar->populate(tree->currentItem(), tree->getHeaderMap(), tree->getLinks().value(tree->currentItem()));
    } else{
        QTreeWidgetItem* item = pictureFlowHash.key(seriesPictureFlow->currentSlide());
        if(seriesTree->currentItem() != item)
            seriesTree->setCurrentItem(item);
        seriesSidebar->populate(item, seriesTree->getHeaderMap(), seriesTree->getLinks().value(seriesTree->currentItem()));
    }
}
void MainWindow::timerExpired(){
    if(tabwidget->currentIndex()==0){
        tree->setCurrentItem(tree->topLevelItem(pictureFlow->currentSlide()));
        sidebar->populate(tree->currentItem(), tree->getHeaderMap(), tree->getLinks().value(tree->currentItem()));
    }
    else{
        seriesTree->setCurrentItem(pictureFlowHash.key(seriesPictureFlow->currentSlide()));
        seriesSidebar->populate(seriesTree->currentItem(), seriesTree->getHeaderMap(), seriesTree->getLinks().value(seriesTree->currentItem()));
    }
}
MainWindow::~MainWindow(){
    delete selections;
    int childCount = children().size();
    for(int i=0; i<childCount; ++i){

        delete children().at(0);
    }
}

void MainWindow::callOpen(){
    if(QFileInfo(openFile).exists()){
        openLibrary(QFileInfo(openFile).absoluteFilePath());
    }
}

void MainWindow::createToolBars(){

    QList<QAction*> actions;
    QAction *getData = new QAction(QIcon(":Icons/MovieInfo2"), tr("Get Data"), this);
    QAction *addMovies = new QAction(QIcon(":Icons/AddMovies"), tr("Import Files"), this);
    QAction *playMovie = new QAction(QIcon(":Icons/Play.png"), tr("Play"), this);
    QAction *openLib = new QAction(QIcon(":Icons/openFolder1.png"), tr("Open Library"), this);
    QAction *newLib = new QAction(QIcon(":Icons/Document-new-icon.png"), tr("New Library"), this);
    QAction *saveLib = new QAction(QIcon(":Icons/Devices-media-floppy-icon.png"), tr("Save Changes"), this);
    openLib->setMenu(openMenu);
    connect(getData, SIGNAL(triggered()), this, SLOT(callGetData()));
    connect(addMovies, SIGNAL(triggered()), this, SLOT(importFiles()));
    connect(playMovie, SIGNAL(triggered()), this, SLOT(play()));
    connect(openLib, SIGNAL(triggered()), this, SLOT(getLibraryFile()));
    connect(newLib, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(saveLib, SIGNAL(triggered()), this, SLOT(save()));
    libraryToolbar = new QToolBar(this);
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    actions.append(libraryToolbar->addSeparator());
    actions.append(getData);
    actions.append(libraryToolbar->addSeparator());
    actions.append(addMovies);
    actions.append(libraryToolbar->addSeparator());
    actions.append(playMovie);
    actions.append(libraryToolbar->addSeparator());
    libraryToolbar->setIconSize(QSize(40,40));
    libraryToolbar->addActions(actions);
    actions.clear();
    fileToolbar = new QToolBar(this);
    actions.append(openLib);
    actions.append(fileToolbar->addSeparator());
    actions.append(newLib);
    actions.append(fileToolbar->addSeparator());
    actions.append(saveLib);
    actions.append(fileToolbar->addSeparator());
    fileToolbar->addActions(actions);
    fileToolbar->setIconSize(QSize(40,40));
    fileToolbar->addActions(actions);
    fileToolbar->setForegroundRole(QPalette::BrightText);
    this->addToolBar(libraryToolbar);
    this->addToolBar(fileToolbar);
    canceldownload = new QToolButton(this);
    canceldownload->setDefaultAction(cancelAction);

}

void MainWindow::createMenus(){
    QList<QAction*> actions;
    fileMenu = menuBar()->addMenu(tr("&File"));
    actions.append(newAction);
    actions.append(openAction);
    fileMenu->addActions(actions);
    actions.clear();
    openMenu = new QMenu(this);
    actions.append(here);
    actions.append(inNewWindow);
    openMenu->addActions(actions);
    openAction->setMenu(openMenu);
    fileMenu->addSeparator();
    actions.clear();
    actions.append(saveAction);
    actions.append(saveAsAction);
    fileMenu->addActions(actions);
    editMenu = menuBar()->addMenu(tr("&Edit"));
    actions.clear();
    //    actions.append(preferencesAction);
    actions.append(columnAction);
    editMenu->addActions(actions);
    viewMenu = menuBar()->addMenu(tr("&View"));
    actions.clear();
    actions.append(listViewAction);
    actions.append(iconViewAction);
    actions.append(pictureFlowAction);
    viewMenu->addActions(actions);
}

void MainWindow::createStatusBar(){
    downloadProgress = new QProgressBar(this);
    statusBar()->addWidget(downloadProgress);
    statusBar()->addWidget(canceldownload);
    statusBar()->hide();
}

void MainWindow::createActions(){
    playAction = new QAction("Play", this);
    playAction->setShortcut(QKeySequence::InsertParagraphSeparator);
    importAction = new QAction("Import Files", this);
    columnAction = new QAction("Edit Columns", this);
    openAction = new QAction("Open Library", this);
    edit = new QAction("Edit", this);
    here = new QAction("Here", this);
    inNewWindow =new QAction("In New Window", this);
    saveAction = new QAction("Save", this);
    saveAsAction = new QAction("Save as", this);
    newAction = new QAction("New Library", this);
    getFilmDataAction = new QAction("Get Data and Covers", this);
    selectCoverAction = new QAction("Select Cover Image", this);
    listViewAction = new QAction("List View", this);
    listViewAction->setCheckable(true);
    listViewAction->setChecked(true);
    listViewAction->setIcon(QIcon(":Icons/Data-List-icon.png"));
    iconViewAction = new QAction("Icon View", this);
    iconViewAction->setIcon(QIcon(":Icons/layout-grid2.png"));
    iconViewAction->setCheckable(true);
    iconViewAction->setChecked(false);
    pictureFlowAction = new QAction("Covers", this);
    pictureFlowAction->setCheckable(true);
    pictureFlowAction->setIcon(QIcon(":/Icons/pictureflowicon.png"));
    pictureFlowAction->setChecked(true);
    deleteAction = new QAction("Remove Entry", this);
    deleteAction->setShortcut(QKeySequence::Delete);
    cancelAction = new QAction(QIcon(":Icons/button-close.png"), "Cancel Download", this);
    //    preferencesAction = new QAction("&Preferences", this);
}

void MainWindow::createConnections(){
    connect(importAction, SIGNAL(triggered()), this, SLOT(importFiles()));
    connect(columnAction, SIGNAL(triggered()), this, SLOT(columns()));
    connect(here, SIGNAL(triggered()), this, SLOT(getLibraryFile()));
    connect(inNewWindow, SIGNAL(triggered()), this, SLOT(openInNewWindow()));
    connect(saveAction,SIGNAL(triggered()),this,SLOT(save()));
    connect(saveAsAction,SIGNAL(triggered()),this,SLOT(saveAs()));
    connect(list, SIGNAL(doubleClicked(QModelIndex)), tree, SLOT(activate(QModelIndex)));
    connect(tree, SIGNAL(itemSelectionChanged()), this, SLOT(treeSelectionChanged()));
    connect(seriesTree, SIGNAL(itemSelectionChanged()), this, SLOT(treeSelectionChanged()));
    connect(seriesTree, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(itemExpanded(QTreeWidgetItem*)));
    connect(seriesTree, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(itemCollapsed(QTreeWidgetItem*)));
    connect(pictureFlow, SIGNAL(slideChanged(int)), this, SLOT(currentSlideChanged(int)));
    connect(seriesPictureFlow, SIGNAL(slideChanged(int)), this, SLOT(currentSlideChanged(int)));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(tree, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(updateCoverFlow(QTreeWidgetItem*,int)));
    connect(list, SIGNAL(clicked(QModelIndex)), this, SLOT(listSelectionChanged(QModelIndex)));
    connect(list, SIGNAL(selectionChanged(QModelIndex)), this, SLOT(listSelectionChanged(QModelIndex)));
    connect(tree->header(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(dataSorted(int, Qt::SortOrder)));
    connect(seriesTree, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(updateCoverFlow(QTreeWidgetItem*,int)));
    connect(seriesList, SIGNAL(clicked(QModelIndex)), this, SLOT(listSelectionChanged(QModelIndex)));
    connect(seriesList, SIGNAL(selectionChanged(QModelIndex)), this, SLOT(listSelectionChanged(QModelIndex)));
    connect(seriesTree->header(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(dataSorted(int, Qt::SortOrder)));
    connect(playAction, SIGNAL(triggered()), this, SLOT(play()));
    connect(this, SIGNAL(playMovie(QTreeWidgetItem*,int)), tree, SLOT(openFile(QTreeWidgetItem*,int)));
    connect(this, SIGNAL(playEpisode(QTreeWidgetItem*,int)), seriesTree, SLOT(openFile(QTreeWidgetItem*,int)));
    connect(selectCoverAction, SIGNAL(triggered()), this, SLOT(selectCover()));
    connect(getFilmDataAction, SIGNAL(triggered()), this, SLOT(callGetData()));
    connect(listViewAction, SIGNAL(toggled(bool)), this, SLOT(listViewToggled(bool)));
    connect(pictureFlowAction, SIGNAL(toggled(bool)), this, SLOT(coverFlowViewToggled(bool)));
    connect(iconViewAction, SIGNAL(toggled(bool)), this, SLOT(iconViewToggled(bool)));
    connect(pictureFlow, SIGNAL(itemActivated(int)), this, SLOT(coverItemActivated(int)));
    connect(seriesPictureFlow, SIGNAL(itemActivated(int)), this, SLOT(seriesCoverItemActivated(int)));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(removeMovie()));
    connect(timer, SIGNAL(timeout()), this, SLOT(timerExpired()));
    connect(sidebarTimer, SIGNAL(timeout()), this, SLOT(sidebarTimerExpired()));
    connect(edit, SIGNAL(triggered()), this, SLOT(callEdit()));
    //    connect(preferencesAction, SIGNAL(triggered()), this, SLOT(options()));
}
void MainWindow::callEdit(){
    if(tabwidget->currentIndex() == 0)
        tree->editItem(tree->currentItem(), tree->currentColumn());
    else
        seriesTree->editItem(seriesTree->currentItem(), seriesTree->currentColumn());
}
void MainWindow::options(){
    //    Ui_Options options;
    //    QDialog *dlg = new QDialog(this);

    //    options.setupUi(dlg);
    //    if(dlg->exec()){
    //        QFile settings;
    //        QDir dir;
    //        dir.setPath(QDir().homePath()+"/.XLibrary");
    //        if(!dir.exists()){
    //            dir.mkdir(dir.path());
    //        }
    //        settings.setFileName(dir.absolutePath() + "Settings");
    //        settings.open(QFile::WriteOnly);
    //        if(options.autoMove->isChecked()){
    //            settings.write("Auto Move : True");
    //        }
    //    }
}

void MainWindow::createContextMenu(){

    tree->addAction(playAction);
    tree->addAction(getFilmDataAction);
    tree->addAction(edit);
    tree->addAction(selectCoverAction);
    tree->addAction(deleteAction);
    tree->setContextMenuPolicy(Qt::ActionsContextMenu);

    pictureFlow->addAction(playAction);
    pictureFlow->addAction(getFilmDataAction);
    pictureFlow->addAction(selectCoverAction);
    pictureFlow->addAction(deleteAction);
    pictureFlow->setContextMenuPolicy(Qt::ActionsContextMenu);

    seriesTree->addAction(playAction);
    seriesTree->addAction(getFilmDataAction);
    seriesTree->addAction(edit);
    seriesTree->addAction(selectCoverAction);
    seriesTree->addAction(deleteAction);
    seriesTree->setContextMenuPolicy(Qt::ActionsContextMenu);

    seriesPictureFlow->addAction(playAction);
    seriesPictureFlow->addAction(getFilmDataAction);
    seriesPictureFlow->addAction(selectCoverAction);
    seriesPictureFlow->addAction(deleteAction);
    seriesPictureFlow->setContextMenuPolicy(Qt::ActionsContextMenu);
}

bool MainWindow::save(){
    if(curFile.isEmpty())
        return saveAs();
    else
        return saveLibrary(curFile);
}

bool MainWindow::saveAs(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Library"), ".",tr("XLibrary files (*.xlib)"));
    if(fileName.isEmpty())
        return false;
    else
        if(!fileName.endsWith(".xlib"))
            fileName.append(".xlib");
    return saveLibrary(fileName);
}
void MainWindow::callGetData(){
    if(tabwidget->currentIndex() == 1){
        if(seriesTree->selectedItems().size() > 0)
            getSeriesData(seriesTree->selectedItems());
        else
            if(seriesPictureFlow->slideCount()>0){
                QList<QTreeWidgetItem*> items;
                QTreeWidgetItem* item = pictureFlowHash.key(seriesPictureFlow->currentSlide());
                if(item){
                    items.append(item);
                    getFilmData(items);
                }
            }
        return;
    }
    if(tree->selectedItems().size() > 0)
        getFilmData(tree->selectedItems());
    else
        if(pictureFlow->slideCount()>0){
            QList<QTreeWidgetItem*> items;
            items.append(tree->topLevelItem(pictureFlow->currentSlide()));
            getFilmData(items);
        }
}
void MainWindow::selectCover(){
    QString newCover = QFileDialog::getOpenFileName(this, "Select Cover Image", QString(), "Images (*.bmp *.gif *.jpeg *.jpg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)");
    QTreeWidgetItem* item = tree->topLevelItem(pictureFlow->currentSlide());
    pictureFlow->setSlide(pictureFlow->currentSlide(), QImage(newCover));
    item->setText(tree->getHeaderMap().value("Icon6154"), newCover);
    QIcon icon;
    icon.addFile(newCover);
    item->setIcon(0, icon);
    sidebar->populate(tree->currentItem(), tree->getHeaderMap(), tree->getLinks().value(tree->currentItem()));
    setWindowModified(true);
}

void MainWindow::removeMovie(){
    if(tabwidget->currentIndex()==0){
        QList<QTreeWidgetItem*> removeItems = tree->selectedItems();
        int prev = tree->indexOfTopLevelItem(removeItems[0])-1;
        int next = prev+1;
        tree->removeItems(removeItems);
        setupPictureFlow();
        if(prev >= 0){
            tree->setCurrentItem(tree->topLevelItem(prev));
            sidebar->populate(tree->topLevelItem(prev), tree->getHeaderMap(), tree->getLinks().value(tree->topLevelItem(prev)));
            pictureFlow->setCurrentSlide(prev);
        }
        else
            if(next<tree->topLevelItemCount()){
                tree->setCurrentItem(tree->topLevelItem(next));
                sidebar->populate(tree->topLevelItem(next), tree->getHeaderMap(), tree->getLinks().value(tree->topLevelItem(next)));
                pictureFlow->setCurrentSlide(next);
            }
            else
                sidebar->clear();

    }
    else{
        QList<QTreeWidgetItem*> removeItems = seriesTree->selectedItems();
        int prev = seriesTree->indexOfTopLevelItem(removeItems[0])-1;
        int next = prev+1;
        seriesTree->removeItems(removeItems);
        setupSeriesPictureFlow();
        if(prev >= 0){
            seriesTree->setCurrentItem(seriesTree->topLevelItem(prev));
            seriesSidebar->populate(seriesTree->topLevelItem(prev), seriesTree->getHeaderMap(), seriesTree->getLinks().value(seriesTree->topLevelItem(prev)));
            seriesPictureFlow->setCurrentSlide(prev);
        }
        else
            if(next<seriesTree->topLevelItemCount()){
                seriesTree->setCurrentItem(seriesTree->topLevelItem(next));
                seriesSidebar->populate(seriesTree->topLevelItem(next), seriesTree->getHeaderMap(), seriesTree->getLinks().value(seriesTree->topLevelItem(next)));
                seriesPictureFlow->setCurrentSlide(next);
            }
            else
                seriesSidebar->clear();
    }
}

bool MainWindow::saveLibrary(const QString &fileName)
{
    QFile *file = new QFile(fileName, this);
    if(file->open(QFile::OpenMode(QFile::WriteOnly))){
        QXmlStreamWriter *writer = new QXmlStreamWriter(file);
        writer->writeStartDocument();
        writer->writeStartElement("Library");
        writeSettings(writer);
        tree->writeToFile(writer, "MovieLibrary");
        seriesTree->writeToFile(writer, "TVLibrary");
        writer->writeEndDocument();
        file->close();
        curFile = fileName;
        setWindowTitle("[*]" + QFileInfo(curFile).fileName());
        setWindowModified(false);
        delete writer;
        return true;
    }
    return false;
}

void MainWindow::writeSettings(QXmlStreamWriter *writer){
    int i = tabwidget->currentIndex();
    tabwidget->setCurrentIndex(0);
    writer->writeStartElement("Settings");
    if(tree->isVisible())
        writer->writeAttribute("Tree","Show");
    else
        writer->writeAttribute("Tree", "Hide");
    if(pictureFlow->isVisible())
        writer->writeAttribute("PictureFlow", "Show");
    else
        writer->writeAttribute("PictureFlow","Hide");
    if(list->isVisible())
        writer->writeAttribute("IconList", "Show");
    else
        writer->writeAttribute("IconList", "Hide");
    tabwidget->setCurrentIndex(1);
    if(seriesTree->isVisible())
        writer->writeAttribute("SeriesTree","Show");
    else
        writer->writeAttribute("SeriesTree", "Hide");
    if(seriesPictureFlow->isVisible())
        writer->writeAttribute("SeriesPictureFlow", "Show");
    else
        writer->writeAttribute("SeriesPictureFlow","Hide");
    if(seriesList->isVisible())
        writer->writeAttribute("SeriesIconList", "Show");
    else
        writer->writeAttribute("SeriesIconList", "Hide");
    writer->writeAttribute("DownloadLocation", imageFolder);
    writer->writeEndElement();
    tabwidget->setCurrentIndex(i);

}
void MainWindow::importFiles(){
    if(tabwidget->currentIndex() == 1){
        importSeriesEpisodes();
        return;
    }
    QStringList paths = QFileDialog::getOpenFileNames(this, "Import Files to Library");
    QString name;
    QTreeWidgetItem *treeItem;
    QIcon icon;
    QMap<QString, int> hMap = tree->getHeaderMap();
    QList<QTreeWidgetItem*> items;
    for (int i = 0; i< paths.size(); ++i){
        name = QFileInfo(paths[i]).completeBaseName();
        treeItem = new QTreeWidgetItem();
        icon = QFileIconProvider().icon(QFileInfo(paths[i]));

        treeItem->setText(hMap.value("Path"), paths[i]);
        treeItem->setIcon(0,icon);
        treeItem->setText(hMap.value("Title"), name);
        double d = double(QFileInfo(paths[i]).size());
        QString size;
        if(d/1000000000 >= 1)
            size = QString().number(d/1000000000,'f',1) +"GB";
        else
            if(d/1000000 >= 1)
                size = QString().number(d/1000000,'f',1) +"MB";
            else
                if(d/1000 >= 1)
                    size = QString().number(d/1000,'f',1) +"KB";
                else
                    size = QString().number(d,'f',1) +"B";
        treeItem->setText(hMap.value("Size"), size);
        items.append(treeItem);
    }
    if(items.size()){
        setWindowModified(true);
        getFilmData(items);
        tree->addTopLevelItems(items);
        tree->setCurrentItem(treeItem);
        setupPictureFlow();
    }
}

void MainWindow::newFile(){
    QProcess pro;
    pro.startDetached(app);
}

void MainWindow::getLibraryFile(){
    if(okToContinue()){
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Library"),".", tr("XLibrary Files (*.xlib)"));
        if(!fileName.isEmpty())
            openLibrary(fileName);
    }
}

void MainWindow::openLibrary(const QString &fileName){
    QXmlStreamReader *reader = new QXmlStreamReader(new QFile(fileName));
    if(!reader->device()->open(QFile::ReadOnly)){
        QMessageBox::information(this,"Access Denied", "Unable to open file " + fileName, QMessageBox::Ok);
    }
    if(reader->device()->isReadable()){
        tree->clear();
        pictureFlow->clear();
        seriesTree->clear();
        seriesPictureFlow->clear();
        if(!reader->readNextStartElement()){
            QMessageBox::information(this,"Incorrect File Format", "This is not the right type of file", QMessageBox::Ok);
            return;
        }
        if(reader->name()=="Library"){
            if(!reader->readNextStartElement()){
                QMessageBox::information(this,"Incorrect File Format", "This is not the right type of file", QMessageBox::Ok);
                return;
            }
            if(reader->name() == "Settings"){
                QXmlStreamAttributes att = reader->attributes();
                for(int i = 0; i<att.count(); ++i){
                    if(att[i].name().toString()=="Tree"){
                        if(att[i].value().toString() == "Show")
                            tree->show();
                        else
                            if(att[i].value().toString() == "Hide")
                                tree->hide();
                    }
                    else{
                        if(att[i].name().toString()=="PictureFlow"){
                            if(att[i].value().toString() == "Show")
                                pictureFlow->show();
                            else
                                if(att[i].value().toString() == "Hide")
                                    pictureFlow->hide();
                        }
                        else{
                            if(att[i].name().toString()=="IconList"){
                                if(att[i].value().toString() == "Show")
                                    list->show();
                                else
                                    if(att[i].value().toString() == "Hide")
                                        list->hide();
                            }
                            else{
                                if(att[i].name().toString() == "DownloadLocation")
                                    imageFolder = att[i].value().toString();
                            }
                            if(att[i].name().toString()=="SeriesTree"){
                                if(att[i].value().toString() == "Show")
                                    seriesTree->show();
                                else
                                    if(att[i].value().toString() == "Hide")
                                        seriesTree->hide();
                            }
                            else{
                                if(att[i].name().toString()=="SeriesPictureFlow"){
                                    if(att[i].value().toString() == "Show")
                                        seriesPictureFlow->show();
                                    else
                                        if(att[i].value().toString() == "Hide")
                                            seriesPictureFlow->hide();
                                }
                                else{
                                    if(att[i].name().toString()=="SeriesIconList"){
                                        if(att[i].value().toString() == "Show")
                                            seriesList->show();
                                        else
                                            if(att[i].value().toString() == "Hide")
                                                seriesList->hide();
                                    }
                                }
                            }
                        }
                    }

                }
            }
            else{
                QMessageBox::information(this,"Incorrect File Format", "This is not the right type of file", QMessageBox::Ok);
                return;
            }
        }
        else{
            QMessageBox::information(this,"Incorrect File Format", "This is not the right type of file", QMessageBox::Ok);
            return;
        }
        if(!tree->readItemsFromXML(reader, "MovieLibrary"))
            QMessageBox::information(this,"XML Read Error", "There was an error in the XML text. Error String: "+reader->errorString(), QMessageBox::Ok);
        reader->readNext();
        if(!(reader->name().toString() == "Library" && reader->isEndElement())){
            if(!seriesTree->readItemsFromXML(reader, "TVLibrary"))
                QMessageBox::information(this,"XML Read Error", "There was an error in the XML text. Error String: "+reader->errorString(), QMessageBox::Ok);
        }

        curFile = fileName;
        setWindowTitle("[*]" + QFileInfo(fileName).completeBaseName());
        setWindowModified(false);
        if(tree->topLevelItemCount()>0){
            tree->setCurrentItem(tree->topLevelItem(0));
            pictureFlow->setCurrentSlide(0);
            sidebar->populate(tree->topLevelItem(0), tree->getHeaderMap(), tree->getLinks().value(tree->topLevelItem(0)));
        }
        if(seriesTree->topLevelItemCount()>0){
            seriesTree->setCurrentItem(seriesTree->topLevelItem(0));
            seriesPictureFlow->setCurrentSlide(0);
            seriesSidebar->populate(seriesTree->topLevelItem(0), seriesTree->getHeaderMap(), seriesTree->getLinks().value(seriesTree->topLevelItem(0)));
        }
        setupPictureFlow();
        setupSeriesPictureFlow();
    }
    else{
        QMessageBox::information(this,"Access Denied", "Unable to read from file " + fileName, QMessageBox::Ok);
    }
    seriesTree->collapseAll();
}

void MainWindow::openInNewWindow(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Library"),".", tr("XLibrary Files (*.xlib)"));

    if(!fileName.isEmpty()){
        QProcess pro;
        QStringList args;
        args.append(fileName);
        pro.startDetached(app, args);
    }
}

bool MainWindow::okToContinue()
{
    if (isWindowModified()) {
        int r = QMessageBox::warning(this, tr("Movie Library"),
                                     tr("This library has been modified.\n"
                                        "Do you want to save your changes?"),
                                     QMessageBox::Yes | QMessageBox::Default,
                                     QMessageBox::No,
                                     QMessageBox::Cancel | QMessageBox::Escape);
        if (r == QMessageBox::Yes) {
            return save();
        }

        if (r == QMessageBox::Cancel) {
            return false;
        }
        if(r == QMessageBox::No)
            return true;
    }
    return true;
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(okToContinue()){
        event->accept();
    }
    else
        event->ignore();
}

void MainWindow::dataSorted(int col, Qt::SortOrder){
    if(tabwidget->currentIndex() == 0)
        setupPictureFlow();
    else
        setupSeriesPictureFlow();
}

void MainWindow::columns(){
    if(tabwidget->currentIndex() == 0){
        ColumnEditDialog *columnDialog = new ColumnEditDialog(tree, this);
        if(columnDialog->exec())
        {
            QList<QCheckBox*> cb = columnDialog->getExistingColumns();
            QList<QLineEdit*> le = columnDialog->getNewColumns();
            QMap<QString, int> hMap = tree->getHeaderMap();
            bool winmodified = false;
            for(int i=0; i<cb.size(); ++i){
                if(cb.at(i)->isChecked()){
                    if(tree->isColumnHidden(hMap.value(cb.at(i)->text()))){
                        tree->setColumnHidden(hMap.value(cb.at(i)->text()), false);
                        winmodified = true;
                    }
                }
                else{
                    if(!tree->isColumnHidden(hMap.value(cb.at(i)->text()))){
                        tree->setColumnHidden(hMap.value(cb.at(i)->text()), true);
                        winmodified =true;
                    }
                }
            }
            for(int i=0; i<le.size(); ++i){
                if(!le[i]->text().isEmpty()){
                    if(!hMap.contains(le[i]->text()))
                        tree->addHeader(le[i]->text());
                    winmodified = true;
                }
            }
            if(winmodified)
                setWindowModified(true);
        }
    }
    else{
        ColumnEditDialog *columnDialog = new ColumnEditDialog(seriesTree, this);
        if(columnDialog->exec())
        {
            QList<QCheckBox*> cb = columnDialog->getExistingColumns();
            QList<QLineEdit*> le = columnDialog->getNewColumns();
            QMap<QString, int> hMap = seriesTree->getHeaderMap();
            bool winmodified = false;
            for(int i=0; i<cb.size(); ++i){
                if(cb.at(i)->isChecked()){
                    if(seriesTree->isColumnHidden(hMap.value(cb.at(i)->text()))){
                        seriesTree->setColumnHidden(hMap.value(cb.at(i)->text()), false);
                        winmodified = true;
                    }
                }
                else{
                    if(!seriesTree->isColumnHidden(hMap.value(cb.at(i)->text()))){
                        seriesTree->setColumnHidden(hMap.value(cb.at(i)->text()), true);
                        winmodified =true;
                    }
                }
            }
            for(int i=0; i<le.size(); ++i){
                if(!le[i]->text().isEmpty()){
                    if(!hMap.contains(le[i]->text()))
                        seriesTree->addHeader(le[i]->text());
                    winmodified = true;
                }
            }
            if(winmodified)
                setWindowModified(true);
        }
    }
}

void MainWindow::getFilmData(QList<QTreeWidgetItem*> items){
    if(items.isEmpty())
        items = tree->selectedItems();
    QMap<QString, int> hMap = tree->getHeaderMap();
    movieDataPrompt = new GetMovieDataPromptDialog(items, hMap, imageFolder, this);

    if(movieDataPrompt->exec()){
        if(downloading == 0){
            downloading = items.size();
            finished = 0;

            downloadProgress->setMaximum(downloading*4);
            downloadProgress->setValue(0);
            statusBar()->show();
        }
        else{
            downloading += items.size();
            downloadProgress->setMaximum(downloading*4);
            statusBar()->show();
        }
        GetMovieData *movieData;
        QList<lineEditStruct*> getDataFor = movieDataPrompt->lineEdits();
        imageFolder = movieDataPrompt->getImageFolder();
        if(!QDir(imageFolder).exists())
            QDir().mkpath(movieDataPrompt->getImageFolder());
        for(int i = 0; i<getDataFor.size(); ++i){
            if(getDataFor.at(i)->imdbID->isEnabled())
                movieData = new GetMovieData(getDataFor.at(i)->imdbID->text(), true, this);
            else
                movieData = new GetMovieData(getDataFor.at(i)->movie->text(), getDataFor.at(i)->year->text(), true, this);
            filmDataItemMap.insert(movieData, items.at(i));
            connect(movieData, SIGNAL(finishedDownloading(GetMovieData*)), this, SLOT(filmDataDownloaded(GetMovieData*)));
            connect(movieData, SIGNAL(finishedStep(int)), this, SLOT(updateProgressBar(int)));
            connect(cancelAction, SIGNAL(triggered()), movieData, SLOT(closeConnection()));
        }
    }
}

void MainWindow::filmDataDownloaded(GetMovieData *movieData){
    QMap<QString, int> hMap;
    QTreeWidgetItem *item = filmDataItemMap.value(movieData);
    if(movieData->success()){
        setWindowModified(true);
        if(movieData->type() == "episode" || movieData->type() == "series"){
            hMap = seriesTree->getHeaderMap();
            if(movieData->season() >= 0)
                item->setText(hMap.value("Season"), QString().number(movieData->season()));
            if(movieData->episode() >= 0)
                item->setText(hMap.value("Episode"), QString().number(movieData->episode()));
            item->setText(hMap.value("Title"), movieData->movie());
            item->setText(hMap.value("Year"), movieData->year());
            item->setText(hMap.value("Director"), movieData->director());
            item->setText(hMap.value("Rated"), movieData->rating());
            item->setText(hMap.value("Genre"), movieData->genres());
            item->setText(hMap.value("Starring"), movieData->starring());
            item->setText(hMap.value("Writer"), movieData->writers());
            item->setText(hMap.value("Country"), movieData->country());
            item->setText(hMap.value("Language"), movieData->language());
            item->setText(hMap.value("imdbID"), movieData->imdbID());
            item->setText(hMap.value("Plot"), movieData->plot());
            if(!movieData->seriesID().isEmpty() && item->parent() != 0){
                if(!seriesIDs.contains(movieData->seriesID())){
                    ++downloading;
                    seriesIDs.append(movieData->seriesID());
                    GetMovieData *getData = new GetMovieData(movieData->seriesID(), true, this);
                    filmDataItemMap.insert(getData, item->parent());
                    connect(getData, SIGNAL(finishedDownloading(GetMovieData*)), this, SLOT(filmDataDownloaded(GetMovieData*)));
                    connect(getData, SIGNAL(finishedStep(int)), this, SLOT(updateProgressBar(int)));
                    connect(cancelAction, SIGNAL(triggered()), movieData, SLOT(closeConnection()));
                }

            }
            seriesTree->setLinks(item, movieData->peopleLinks());
            if(!movieData->poster().isNull()){
                QString iconPath = imageFolder + "/" + movieData->movie() + ".jpg";
                movieData->poster().save(iconPath);
                item->setText(hMap.value("Icon6154"), iconPath);
                QIcon icon;
                icon.addFile(iconPath);
                item->setIcon(0, icon);
            }
        }
        else if(movieData->type() == "movie"){
            hMap = tree->getHeaderMap();
            item->setText(hMap.value("Title"), movieData->movie());
            item->setText(hMap.value("Year"), movieData->year());
            item->setText(hMap.value("Director"), movieData->director());
            item->setText(hMap.value("Rated"), movieData->rating());
            item->setText(hMap.value("Genre"), movieData->genres());
            item->setText(hMap.value("Starring"), movieData->starring());
            item->setText(hMap.value("Writer"), movieData->writers());
            item->setText(hMap.value("Country"), movieData->country());
            item->setText(hMap.value("Language"), movieData->language());
            item->setText(hMap.value("imdbID"), movieData->imdbID());
            item->setText(hMap.value("Plot"), movieData->plot());
            tree->setLinks(item, movieData->peopleLinks());
            if(!movieData->poster().isNull()){
                QString iconPath = imageFolder + "/" + movieData->movie() + ".jpg";
                movieData->poster().save(iconPath);
                item->setText(hMap.value("Icon6154"), iconPath);
                QIcon icon;
                icon.addFile(iconPath);
                item->setIcon(0, icon);
            }
        }
    }
    else
        unsuccessfulDataDL.append(item->text(hMap.value("Title")));
    ++finished;
    if(finished<downloading){
        return;
    }
    statusBar()->hide();
    if(unsuccessfulDataDL.size() == 0){
        finished = 0;
        downloading = 0;
        setupPictureFlow();
        setupSeriesPictureFlow();
        return;
    }
    QString label;
    label.append("Unable to find data for:\n");
    for(int i = 0; i<unsuccessfulDataDL.size(); ++i){
        label.append(unsuccessfulDataDL.at(i) + "\n");
    }
    QMessageBox::information(this,"Data Not Found", label, QMessageBox::Ok);
    unsuccessfulDataDL.clear();
    finished = 0;
    downloading = 0;
    setupPictureFlow();
    setupSeriesPictureFlow();
}
void MainWindow::updateProgressBar(int steps){
    downloadProgress->setValue(downloadProgress->value()+steps);
}

void MainWindow::listViewToggled(bool checked){
    if(checked){
        tree->show();
        seriesTree->show();
        iconViewAction->setChecked(false);
    }
    else{
        tree->hide();
        seriesTree->hide();
        iconViewAction->setChecked(true);
    }
}
void MainWindow::iconViewToggled(bool checked){
    if(checked)
    {
        list->show();
        //        seriesList->show();
        listViewAction->setChecked(false);
    }
    else
    {
        list->hide();
        //        seriesList->hide();
        listViewAction->setChecked(true);
    }

}
void MainWindow::coverFlowViewToggled(bool checked){
    if(checked){
        pictureFlow->show();
        seriesPictureFlow->show();
    }
    else{
        pictureFlow->hide();
        seriesPictureFlow->hide();
    }
}

void MainWindow::play(){
    if(tabwidget->currentIndex()==0)
        emit playMovie(tree->currentItem(),0);
    else
        emit playEpisode(seriesTree->currentItem(), 0);
}
void MainWindow::seriesCoverItemActivated(int index){
    if(pictureFlowHash.key(index) == 0) return;
    if(seriesPictureFlowTopLevel)
        seriesTree->expandItem(seriesTree->topLevelItem(index));
    else
        emit playEpisode(seriesTree->currentItem(), 0);
}

void MainWindow::coverItemActivated(int index){
    if(tree->topLevelItemCount()>index)
        emit playMovie(tree->topLevelItem(index),0);
}
