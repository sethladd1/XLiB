#include "getseriesdataprompt.h"

GetSeriesDataPrompt::GetSeriesDataPrompt(QHash<QString, QList<QTreeWidgetItem*> > items, QMap<QString, int> headerMap, QString _imageFolder, QWidget *parent) :
    QDialog(parent), imageFolder(_imageFolder){
    int seasonCol = headerMap.value("Season");
    int episodeCol = headerMap.value("Episode");
    int imdbCol = headerMap.value("imdbID");
    buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,Qt::Horizontal, this);
    header = new QLabel(this);
    header->setWordWrap(true);
    QString headerstr = "The season number and episode number, or the ID from the episode's IMDb page (imdb.com/title/[IMDb ID]/...) can be used to gather film data and covers.";
    headerstr += " You can edit these items now before searching or cancel to not download data.";
    header->setText(headerstr);
    imgFolder = new QLabel("Location for Downloaded Images", this);
    if(imageFolder.isEmpty()){
        imageFolder = QDir().homePath() + "/Pictures/MovieCovers";
    }
    imageFolderEdit = new QLineEdit(imageFolder, this);
    choose = new QPushButton("Choose", this);
    connect(choose, SIGNAL(clicked()), this, SLOT(fileDialog()));
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *w = new QWidget(this);
    QGridLayout *grid = new QGridLayout(w);
    QGridLayout *hgrid = new QGridLayout();
    int row =0;
    for(QHash<QString, QList<QTreeWidgetItem*> >::iterator i=items.begin(); i != items.end(); ++i){
        grid->addWidget(hFrame(), row, 0, 1, 12);
        ++row;
        series = new QLabel(i.key(), this);
        series->setAlignment(Qt::AlignHCenter);
        grid->addWidget(series, row, 1, 1, 12);
        ++row;
        grid->addWidget(hFrame(), row, 0, 1, 12);
        ++row;
        file = new QLabel( "File", this);
        file->setAlignment(Qt::AlignHCenter);
        grid->addWidget(file, row, 1);
        blank = new QLabel(this);
        blank->setAlignment(Qt::AlignHCenter);
        grid->addWidget(vFrame(), row, 2);
        grid->addWidget(blank, row, 3);
        season = new QLabel("Season", this);
        season->setAlignment(Qt::AlignHCenter);
        grid->addWidget(vFrame(), row, 4);
        grid->addWidget(season, row, 5);
        episode = new QLabel("Episode", this);
        episode->setAlignment(Qt::AlignHCenter);
        grid->addWidget(vFrame(), row, 6);
        grid->addWidget(episode, row, 7);
        blank = new QLabel(this);
        blank->setAlignment(Qt::AlignHCenter);
        grid->addWidget(vFrame(), row, 8);
        grid->addWidget(blank, row, 9);
        imdbID = new QLabel("IMDb ID", this);
        imdbID->setAlignment(Qt::AlignHCenter);
        grid->addWidget(vFrame(), row, 10);
        grid->addWidget(imdbID, row, 11);
        ++row;
        grid->addWidget(hFrame(), row, 0, 1, 12);
        ++row;
        for(int j=0; j<i.value().size(); ++j){
            seriesLineEditStruct *les = new seriesLineEditStruct();
            QButtonGroup *btnGroup = new QButtonGroup(this);
            useSSE = new QRadioButton(this);
            useimdbID = new QRadioButton(this);
            useSSE->setBackgroundRole(QPalette::Light);
            useimdbID->setBackgroundRole(QPalette::Light);
            connect(useSSE, SIGNAL(toggled(bool)), this, SLOT(seriesToggled(bool)));

            btnGroup->addButton(useSSE);
            btnGroup->addButton(useimdbID);
            les->imdbID = new QLineEdit(i.value()[j]->text(imdbCol));
            les->episode = new QLineEdit(i.value()[j]->text(episodeCol));
            les->season = new QLineEdit(i.value()[j]->text(seasonCol));
            les->imdbID->setEnabled(false);
            les->episode->setMaximumWidth(episode->width());
            les->season->setMaximumWidth(season->width());
            itemLineEdits.append(QPair<QTreeWidgetItem*, seriesLineEditStruct*>(i.value()[j], les));
            seriesButtonEditMap.insert(useSSE, les);
            imdbButtonEditMap.insert(useimdbID, les);
            useSSE->setChecked(true);
            fileName = new QLabel(i.value()[j]->text(headerMap.value("Title")), this);
            fileName->setWordWrap(true);
            fileName->setAlignment(Qt::AlignHCenter);
            fileName->setBackgroundRole(QPalette::Light);
            grid->addWidget(fileName, row, 1);
            grid->addWidget(useSSE, row, 3);
            grid->addWidget(les->season, row, 5);
            grid->addWidget(les->episode, row, 7);
            grid->addWidget(useimdbID, row, 9);
            grid->addWidget(les->imdbID, row, 11);
            ++row;
        }

    }

    grid->addWidget(vFrame(), 0, 0, grid->rowCount(), 1);
    grid->addWidget(vFrame(), 0, 13, grid->rowCount(), 1);
    ++row;
    grid->addWidget(hFrame(), row, 0, 1, 12);
    grid->setHorizontalSpacing(0);
    grid->setVerticalSpacing(0);
    scrollArea->setWidget(w);
    QVBoxLayout *main = new QVBoxLayout(this);
    main->addWidget(header);
    main->addLayout(hgrid);
    main->addWidget(scrollArea);

    imageFolderLine = new QHBoxLayout();
    imageFolderLine->addWidget(imgFolder);
    imageFolderLine->addWidget(imageFolderEdit);
    imageFolderLine->addWidget(choose);
    main->addLayout(imageFolderLine);
    hLayout = new QHBoxLayout();
    hLayout->addItem(new QSpacerItem(10, 1));
    hLayout->addWidget(buttons);
    main->addLayout(hLayout);
    resize(w->width()+50, 500);
}
GetSeriesDataPrompt::~GetSeriesDataPrompt(){
    int childCount = children().size();
    for(int i=0; i<childCount; ++i){
        delete children().at(0);
    }
}
void GetSeriesDataPrompt::seriesToggled(bool checked){
    QWidget *widg = focusWidget();
    if(widg){
        if(imdbButtonEditMap.contains(widg)){
            imdbButtonEditMap.value(widg)->season->setEnabled(false);
            imdbButtonEditMap.value(widg)->episode->setEnabled(false);
            imdbButtonEditMap.value(widg)->imdbID->setEnabled(true);
        }
        else{
            seriesButtonEditMap.value(widg)->season->setEnabled(true);
            seriesButtonEditMap.value(widg)->episode->setEnabled(true);
            seriesButtonEditMap.value(widg)->imdbID->setEnabled(false);
        }
    }
}
void GetSeriesDataPrompt::fileDialog(){
    QString newImageFolder = QFileDialog::getExistingDirectory(this, "Select Download Location for Cover Images");
    if(!newImageFolder.isEmpty())
        imageFolderEdit->setText(newImageFolder);
}
QFrame* GetSeriesDataPrompt::vFrame(){
    f = new QFrame(this);
    f->setFrameStyle(QFrame::VLine);
    f->setLineWidth(2);
    return f;
}
QFrame* GetSeriesDataPrompt::hFrame(){
    f = new QFrame(this);
    f->setFrameStyle(QFrame::HLine);
    f->setLineWidth(2);
    return f;
}
QList<QPair<QTreeWidgetItem *, seriesLineEditStruct *> > GetSeriesDataPrompt::lineEdits(){
    return itemLineEdits;
}
QString GetSeriesDataPrompt::getImageFolder(){
    return imageFolderEdit->text();
}
