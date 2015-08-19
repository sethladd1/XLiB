#include "mainwindow.h"

MainWindow::MainWindow(int argc, char *argv[], QWidget *parent) :
    QMainWindow(parent)
{

    app = QFileInfo(argv[0]).absoluteFilePath();
    setWindowIcon(QIcon(":Icons/2-Movies-icon2.png"));
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
    
    QItemSelectionModel *selections = new QItemSelectionModel(data);
    tree->setSelectionModel(selections);
    tree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tree->setIconSize(QSize(30,30));

    list = new QListView();
    list->setSpacing(5);
    list->setViewMode(QListView::IconMode);
    list->setIconSize(QSize(100,100));
    list->setResizeMode(QListView::Adjust);
    list->setWordWrap(true);
    list->setModel(data);
    list->setSelectionModel(selections);
    list->setSelectionMode(QAbstractItemView::ExtendedSelection);

    pictureFlow = new PictureFlow(this);
    pictureFlow->setSlideSize(QSize(230, 300));
    page = new QSplitter(Qt::Vertical, this);
    timer = new QTimer(this);
    timer->setSingleShot(true);
    sidebarTimer = new QTimer(this);
    sidebarTimer->setSingleShot(true);

    list->hide();
    page->addWidget(pictureFlow);
    page->addWidget(tree);
    page->addWidget(list);
    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    createConnections();

    QSplitter *horPage = new QSplitter(Qt::Horizontal, this);
    sidebar = new SideBar(this);
    scrollArea = new QScrollArea(this);
    scrollArea->setMaximumWidth(270);
    scrollArea->setWidget(sidebar);
    scrollArea->resize(270, scrollArea->height());
    scrollArea->setBackgroundRole(QPalette::Light);
    horPage->addWidget(scrollArea);
    horPage->addWidget(page);
    setCentralWidget(horPage);

    pictureFlow->setMinimumHeight(300);

    resize(700, maximumHeight());
    setWindowTitle("[*]untitled");
    setWindowModified(false);
    setMouseTracking(true);

    if(argc>1){
        QTimer *t = new QTimer(this);
        openFile = argv[argc-1];
        t->setSingleShot(true);
        t->start(2000);
        connect(t, SIGNAL(timeout()), this, SLOT(callOpen()));
    }
}
void MainWindow::callOpen(){

    if(QFileInfo(openFile).exists()){
        openLibrary(QFileInfo(openFile).absoluteFilePath());
    }
}

void MainWindow::createToolBars(){

    QList<QAction*> actions;
    QAction *getData = new QAction(QIcon(":Icons/MovieInfo2"), tr("Get Movie Data"), this);
    QAction *addMovies = new QAction(QIcon(":Icons/AddMovies"), tr("Import Movies"), this);
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
    this->addToolBar(libraryToolbar);
    this->addToolBar(fileToolbar);
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

    viewMenu = menuBar()->addMenu(tr("&View"));
    actions.clear();
    actions.append(listViewAction);
    actions.append(iconViewAction);
    actions.append(pictureFlowAction);
    actions.append(columnAction);
    viewMenu->addActions(actions);
}

void MainWindow::createActions(){
    playAction = new QAction("Play", this);
    playAction->setShortcut(QKeySequence::InsertParagraphSeparator);
    importAction = new QAction(QIcon(":Icons/add2"), "Import Movies", this);
    columnAction = new QAction("Edit Columns", this);
    openAction = new QAction("Open Library", this);
    here = new QAction("Here", this);
    inNewWindow =new QAction("In New Window", this);
    saveAction = new QAction("Save", this);
    saveAsAction = new QAction("Save as", this);
    newAction = new QAction("New Library", this);
    getFilmDataAction = new QAction("Get Film Data", this);
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
    connect(pictureFlow, SIGNAL(slideChanged(int)), this, SLOT(currentSlideChanged(int)));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(tree, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(updateCoverFlow(QTreeWidgetItem*,int)));
    connect(list, SIGNAL(pressed(QModelIndex)), this, SLOT(listSelectionChanged(QModelIndex)));
    connect(tree->header(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(dataSorted(int, Qt::SortOrder)));
    connect(playAction, SIGNAL(triggered()), this, SLOT(play()));
    connect(this, SIGNAL(playMovie(QTreeWidgetItem*,int)), tree, SLOT(openFile(QTreeWidgetItem*,int)));
    connect(selectCoverAction, SIGNAL(triggered()), this, SLOT(selectCover()));
    connect(getFilmDataAction, SIGNAL(triggered()), this, SLOT(callGetData()));
    connect(listViewAction, SIGNAL(toggled(bool)), this, SLOT(listViewToggled(bool)));
    connect(pictureFlowAction, SIGNAL(toggled(bool)), this, SLOT(coverFlowViewToggled(bool)));
    connect(iconViewAction, SIGNAL(toggled(bool)), this, SLOT(iconViewToggled(bool)));
    connect(pictureFlow, SIGNAL(itemActivated(int)), this, SLOT(coverItemActivated(int)));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(removeMovie()));
    connect(timer, SIGNAL(timeout()), this, SLOT(timerExpired()));
    connect(sidebarTimer, SIGNAL(timeout()), this, SLOT(sidebarTimerExpired()));

}
void MainWindow::createContextMenu(){

    tree->addAction(playAction);
    tree->addAction(getFilmDataAction);
    tree->addAction(selectCoverAction);
    tree->addAction(deleteAction);
    tree->setContextMenuPolicy(Qt::ActionsContextMenu);
    pictureFlow->addAction(playAction);
    pictureFlow->addAction(getFilmDataAction);
    pictureFlow->addAction(selectCoverAction);
    pictureFlow->addAction(deleteAction);
    pictureFlow->setContextMenuPolicy(Qt::ActionsContextMenu);
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
}

void MainWindow::removeMovie(){

    QList<QTreeWidgetItem*> removeItems = tree->selectedItems();
    int prev = tree->indexOfTopLevelItem(removeItems[0])-1;
    int next = prev+1;
    for(int i = 0; i<removeItems.size(); ++i){
        delete removeItems[i];
    }
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

bool MainWindow::saveLibrary(const QString &fileName)
{
    QFile *file = new QFile(fileName);
    if(file->open(QFile::OpenMode(QFile::WriteOnly))){
        QXmlStreamWriter *writer = new QXmlStreamWriter(file);
        writer->writeStartDocument();
        writer->writeStartElement("Library");
        writeSettings(writer);
        tree->writeToFile(writer, "MovieLibrary");
        writer->writeEndDocument();
        file->close();
        curFile = fileName;
        setWindowTitle("[*]" + QFileInfo(curFile).fileName());
        setWindowModified(false);
        return true;
    }
    return false;

}

void MainWindow::writeSettings(QXmlStreamWriter *writer){

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
    writer->writeAttribute("DownloadLocation", imageFolder);
    writer->writeEndElement();

}
void MainWindow::importFiles(){

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
        setupPictureFlow();
        setWindowModified(true);
        getFilmData(items);
        tree->addTopLevelItems(items);
        tree->setCurrentItem(treeItem);
        sidebar->populate(treeItem, hMap, tree->getLinks().value(treeItem));
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
        QMessageBox::information(this,"Access Denied", "Unable to open file " + fileName, QMessageBox::Ok);;
    }
    if(reader->device()->isReadable()){
        tree->clear();
        pictureFlow->clear();
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
            QMessageBox::information(this,"XML Read Error", "There was an error in the XML text. Error String: "+reader->errorString(), QMessageBox::Ok);;
        setupPictureFlow();
        curFile = fileName;
        setWindowTitle("[*]" + QFileInfo(fileName).completeBaseName());
        setWindowModified(false);
        if(tree->topLevelItemCount()>0){
            tree->setCurrentItem(tree->topLevelItem(0));
            pictureFlow->setCurrentSlide(0);
            sidebar->populate(tree->topLevelItem(0), tree->getHeaderMap(), tree->getLinks().value(tree->topLevelItem(0)));
        }
    }
    else{
        QMessageBox::information(this,"Access Denied", "Unable to read from file " + fileName, QMessageBox::Ok);
    }
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

void MainWindow::setupPictureFlow(){
    pictureFlow->clear();
    QMap<QString, int> hMap = tree->getHeaderMap();
    for (int i =0; i<tree->topLevelItemCount(); ++i){
        if(!tree->topLevelItem(i)->text(hMap.value("Icon6154")).isEmpty()){
            pictureFlow->setSlideCount(i+1);
            pictureFlow->setSlide(i,QPixmap(tree->topLevelItem(i)->text(hMap.value("Icon6154"))));
            pictureFlow->setSlideCaption(i,tree->topLevelItem(i)->text(hMap.value("Title")));
        }
        else{
            pictureFlow->setSlideCount(i+1);
            pictureFlow->setSlide(i,QFileIconProvider().icon(QFileInfo(tree->topLevelItem(i)->text(hMap.value("Path")))).pixmap(pictureFlow->slideSize()));
            pictureFlow->setSlideCaption(i,tree->topLevelItem(i)->text(hMap.value("Title")));
        }
    }
    if(tree->currentItem() != 0){
        pictureFlow->setCurrentSlide(tree->indexOfTopLevelItem(tree->currentItem()));
        sidebar->populate(tree->currentItem(), tree->getHeaderMap(), tree->getLinks().value(tree->currentItem()));
    }
}

void MainWindow::updateCoverFlow(QTreeWidgetItem *item, int col){
    int i = tree->indexOfTopLevelItem(item);

    if(pictureFlow->slideCount() > i){
        if(item == tree->currentItem())
            sidebar->populate(item, tree->getHeaderMap(), tree->getLinks().value(item));
        QMap<QString, int> hMap = tree->getHeaderMap();
        if(col == 0 || col == hMap.value("Icon6154")){
            if(!tree->topLevelItem(i)->text(hMap.value("Icon6154")).isEmpty()){
                pictureFlow->setSlide(i,QPixmap(tree->topLevelItem(i)->text(hMap.value("Icon6154"))));
                pictureFlow->setSlideCaption(i,tree->topLevelItem(i)->text(hMap.value("Title")));
            }
            else{
                pictureFlow->setSlide(i,QFileIconProvider().icon(QFileInfo(tree->topLevelItem(i)->text(hMap.value("Path")))).pixmap(pictureFlow->slideSize()));
                pictureFlow->setSlideCaption(i,tree->topLevelItem(i)->text(hMap.value("Title")));
            }
        }
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
void MainWindow::treeSelectionChanged(){
    sidebarTimer->start(100);
    if(pictureFlow->currentSlide() != tree->indexOfTopLevelItem(tree->currentItem())){
        //        sidebar->populate(tree->currentItem(), tree->getHeaderMap());
        pictureFlow->setCurrentSlide(tree->indexOfTopLevelItem(tree->currentItem()));
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
    if(!tree->selectedItems().contains(tree->topLevelItem(index))){
        tree->setCurrentItem(tree->topLevelItem(index));
    }
}

void MainWindow::timerExpired(){
    if(!tree->selectedItems().contains(tree->topLevelItem(pictureFlow->currentSlide()))){
        tree->setCurrentItem(tree->topLevelItem(pictureFlow->currentSlide()));
    }
    sidebar->populate(tree->currentItem(), tree->getHeaderMap(), tree->getLinks().value(tree->currentItem()));
}
void MainWindow::sidebarTimerExpired(){

    sidebar->populate(tree->currentItem(), tree->getHeaderMap(), tree->getLinks().value(tree->currentItem()));
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(okToContinue()){
        event->accept();
    }
    else
        event->ignore();
}
void MainWindow::dataSorted(int col, Qt::SortOrder){
    setupPictureFlow();
}

void MainWindow::columns(){
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
                if(!tree->getHeaderMap().contains(le[i]->text()))
                    tree->addHeader(le[i]->text());
                winmodified = true;
            }
        }
        if(winmodified)
            setWindowModified(true);
    }
}

void MainWindow::getFilmData(QList<QTreeWidgetItem*> items){
    if(items.isEmpty())
        items = tree->selectedItems();
    QMap<QString, int> hMap = tree->getHeaderMap();
    movieDataPrompt = new GetMovieDataPromptDialog(items, hMap, imageFolder, this);

    if(movieDataPrompt->exec()){
        downloading = items.size();
        finished = 0;
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
        }
    }
}

void MainWindow::filmDataDownloaded(GetMovieData *movieData){
    QMap<QString, int> hMap = tree->getHeaderMap();
    QTreeWidgetItem *item = filmDataItemMap.value(movieData);
    if(movieData->success()){
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
        setWindowModified(true);
    }
    else
        unsuccessfulDataDL.append(item->text(hMap.value("Title")));
    ++finished;
    if(finished<downloading)
        return;
    if(unsuccessfulDataDL.size() == 0){
        finished = 0;
        downloading = 0;
        setupPictureFlow();
        return;
    }
    QString label;
    label.append("Unable to find data for:\n");
    for(int i = 0; i<unsuccessfulDataDL.size(); ++i){
        label.append(unsuccessfulDataDL.at(i) + "\n");
    }
    QMessageBox::information(this,"Data Not Found", label, QMessageBox::Ok);
    unsuccessfulDataDL.clear();
    setupPictureFlow();
}
void MainWindow::listViewToggled(bool checked){
    if(checked){
        tree->show();
        iconViewAction->setChecked(false);
    }
    else{
        tree->hide();
        iconViewAction->setChecked(true);
    }
}
void MainWindow::iconViewToggled(bool checked){
    if(checked)
    {
        list->show();
        listViewAction->setChecked(false);
    }
    else
    {
        list->hide();
        listViewAction->setChecked(true);
    }

}
void MainWindow::coverFlowViewToggled(bool checked){
    if(checked)
        pictureFlow->show();
    else
        pictureFlow->hide();
}

void MainWindow::play(){
    emit playMovie(tree->currentItem(),0);
}
void MainWindow::coverItemActivated(int index){
    if(tree->topLevelItemCount()>index)
        emit playMovie(tree->topLevelItem(index),0);
}
