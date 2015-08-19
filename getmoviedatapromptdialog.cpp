#include "getmoviedatapromptdialog.h"

GetMovieDataPromptDialog::GetMovieDataPromptDialog(QList<QTreeWidgetItem*> items, QMap<QString, int> headerMap, QString _imageFolder, QWidget *parent) :
    QDialog(parent), imageFolder(_imageFolder){
    int yearCol = headerMap.value("Year");
    int titleCol = headerMap.value("Title");
    int imdbCol = headerMap.value("imdbID");
    buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,Qt::Horizontal, this);
    header = new QLabel(this);
    header->setWordWrap(true);
    QString headerstr = "The title and year of release (optional), or the ID from the movie's IMDb page (imdb.com/title/[IMDb ID]/...) will be used to gather film data and covers.";
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


    tree = new QTreeWidget(this);
    head = new QTreeWidgetItem();
    head->setText(0, QString());
    head->setText(1, "Title");
    head->setText(2, "Year");
    head->setText(3, QString());
    head->setText(4, "IMDb ID");
    head->setTextAlignment(1,Qt::AlignCenter);
    head->setTextAlignment(2,Qt::AlignCenter);
    head->setTextAlignment(4,Qt::AlignCenter);
    tree->setHeaderItem(head);
    tree->setColumnWidth(0, 50);
    tree->setColumnWidth(1, 450);
    tree->setColumnWidth(2, 100);
    tree->setColumnWidth(3, 30);
    tree->setColumnWidth(4, 125);
    for(int i=0; i<items.size(); ++i){
        lineEditStruct *les = new lineEditStruct();
        QButtonGroup *btnGroup = new QButtonGroup();

        useMovieYear = new QRadioButton(this);
        useimdbID = new QRadioButton(this);
        connect(useMovieYear, SIGNAL(toggled(bool)), this, SLOT(movieToggle(bool)));

        btnGroup->addButton(useMovieYear);
        btnGroup->addButton(useimdbID);
        movieEdit = new QLineEdit(items[i]->text(titleCol));
        yearEdit = new QLineEdit(items[i]->text(yearCol));
        imdbIDEdit = new QLineEdit(items[i]->text(imdbCol));
        les->imdbID = imdbIDEdit;
        les->movie = movieEdit;
        les->year = yearEdit;
        les->imdbID->setEnabled(false);
        _lineEdits.append(les);
        movieButtonEditMap.insert(useMovieYear, les);
        imdbButtonEditMap.insert(useimdbID, les);
        newItem = new QTreeWidgetItem();
        useMovieYear->setChecked(true);
        tree->addTopLevelItem(newItem);

        tree->setItemWidget(newItem, 0, useMovieYear);
        tree->setItemWidget(newItem, 1, movieEdit);
        tree->setItemWidget(newItem, 2, yearEdit);
        tree->setItemWidget(newItem, 3, useimdbID);
        tree->setItemWidget(newItem, 4, imdbIDEdit);

    }
    layout();
}

void GetMovieDataPromptDialog::layout(){
    vBox = new QVBoxLayout();
    hLayout = new QHBoxLayout();
    imageFolderLine = new QHBoxLayout();
    imageFolderLine->addWidget(imgFolder);
    imageFolderLine->addWidget(imageFolderEdit);
    imageFolderLine->addWidget(choose);

    hLayout->addItem(new QSpacerItem(10, 1));
    hLayout->addWidget(buttons);
    vBox->addWidget(header);
    vBox->addWidget(tree);
    vBox->addLayout(imageFolderLine);
    vBox->addLayout(hLayout);
    this->setLayout(vBox);
    this->setMinimumWidth(800);
    this->setMinimumHeight(700);
}


QList<lineEditStruct*> GetMovieDataPromptDialog::lineEdits(){
    return _lineEdits;
}
QString GetMovieDataPromptDialog::getImageFolder(){
    return imageFolderEdit->text();
}
void GetMovieDataPromptDialog::fileDialog(){
    QString newImageFolder = QFileDialog::getExistingDirectory(this, "Select Download Location for Cover Images");
    if(!newImageFolder.isEmpty())
        imageFolderEdit->setText(newImageFolder);
}
void GetMovieDataPromptDialog::movieToggle(bool checked){
    QWidget *widg = focusWidget();
    if(widg){
        if(imdbButtonEditMap.contains(widg)){
            imdbButtonEditMap.value(widg)->movie->setEnabled(false);
            imdbButtonEditMap.value(widg)->year->setEnabled(false);
            imdbButtonEditMap.value(widg)->imdbID->setEnabled(true);
        }
        else{
            movieButtonEditMap.value(widg)->movie->setEnabled(true);
            movieButtonEditMap.value(widg)->year->setEnabled(true);
            movieButtonEditMap.value(widg)->imdbID->setEnabled(false);
        }
    }
}
