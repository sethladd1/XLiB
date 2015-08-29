#include "sidebar.h"

SideBar::SideBar(QWidget *parent) :QWidget(parent)
{
    setMaximumWidth(250);
    setBackgroundRole(QPalette::Light);
    starring = new QLabel(this);
    plot = new QLabel(this);
    title = new QLabel(this);
    picture = new QLabel(this);
    director = new QLabel(this);
    writer = new QLabel(this);
    director->setWordWrap(true);

    imdbPage = new QLabel(this);
    starring->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    starring->setWordWrap(true);

    plot->setTextInteractionFlags(Qt::TextSelectableByMouse);
    title->setTextInteractionFlags(Qt::TextSelectableByMouse);
    director->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    imdbPage->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    writer->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    writer->setWordWrap(true);
    title->setFont(QFont("Times", 13, QFont::Bold));
    title->setWordWrap(true);
    title->setAlignment(Qt::AlignHCenter);
    vLayout = new QVBoxLayout(this);
    vLayout->addWidget(title);
    vLayout->addWidget(picture);
    vLayout->addWidget(plot);
    vLayout->addWidget(starring);
    vLayout->addWidget(director);
    vLayout->addWidget(writer);
    vLayout->addWidget(imdbPage);

    imdbPage->setTextFormat(Qt::RichText);
    writer->setTextFormat(Qt::RichText);
    director->setTextFormat(Qt::RichText);
    starring->setTextFormat(Qt::RichText);
    imdbPage->setAlignment(Qt::AlignHCenter);
    plot->setAlignment(Qt::AlignHCenter);
    plot->setWordWrap(true);
    connect(imdbPage, SIGNAL(linkActivated(QString)), this, SLOT(linkActivated(QString)));
    connect(writer, SIGNAL(linkActivated(QString)), this, SLOT(linkActivated(QString)));
    connect(director, SIGNAL(linkActivated(QString)), this, SLOT(linkActivated(QString)));
    connect(starring, SIGNAL(linkActivated(QString)), this, SLOT(linkActivated(QString)));
    connect(writer, SIGNAL(linkHovered(QString)), this, SLOT(writerLinkHovered(QString)));
    connect(director, SIGNAL(linkHovered(QString)), this, SLOT(directorLinkHovered(QString)));
    connect(starring, SIGNAL(linkHovered(QString)), this, SLOT(starringLinkHovered(QString)));
}
void SideBar::populate(QTreeWidgetItem *item, QMap<QString, int> headerMap, CastAndCrewLinks *links){

    if(item == 0) return;
    QString titlestr = item->text(headerMap.value("Title"));
    if(!item->text(headerMap.value("Year")).isEmpty())
        titlestr += " ("+item->text(headerMap.value("Year")) + ")";
    title->setText(titlestr);
    plot->setText("\n" + item->text(headerMap.value("Plot")) + "\n");
    QPixmap pix(200,200);
    if(!item->text(headerMap.value("Icon6154")).isEmpty())
        pix.load(item->text(headerMap.value("Icon6154")));
    else
        pix = QFileIconProvider().icon(QFileInfo(item->text(headerMap.value("Path")))).pixmap(200,200);
    pix = pix.scaled(200,200,Qt::KeepAspectRatio);
    picture->setPixmap(pix);
    picture->setAlignment(Qt::AlignCenter);
    QString castStr;
    QStringList castStrList;
    if(!item->text(headerMap.value("Starring")).isEmpty()){
        castStr = "Starring: ";
        castStrList = item->text(headerMap.value("Starring")).split(", ", QString::SkipEmptyParts);
        for(int i=0; i<castStrList.size(); ++i){
            if(links !=0 && !links->castLinks.value(castStrList[i]).isEmpty()){
                castStr.append("<a href=\""+links->castLinks.value(castStrList[i])+"\">"+castStrList[i]+"</a>");
            }
            else
                castStr.append(castStrList[i]);
            if(i<castStrList.size()-1)
                castStr.append(", ");
        }
    }
    castStr.append("\n");
    starring->setText(castStr);
    QString directorStr;
    QStringList directorStrList;
    if(!item->text(headerMap.value("Director")).isEmpty()){
        directorStr = "Director: ";
        directorStrList = item->text(headerMap.value("Director")).split(", ", QString::SkipEmptyParts);
        for(int i=0; i<directorStrList.size(); ++i){
            if(links !=0 && !links->directorLinks.value(directorStrList[i]).isEmpty()){
                directorStr.append("<a href=\""+links->directorLinks.value(directorStrList[i])+"\">"+directorStrList[i]+"</a>");
            }
            else
                directorStr.append(directorStrList[i]);
            if(i<directorStrList.size()-1)
                directorStr.append(", ");
        }
    }
    directorStr.append("\n");
    director->setText(directorStr);
    QString writerStr;
    QStringList writerStrList;
    if(!item->text(headerMap.value("Writer")).isEmpty()){
        writerStr = "Writer: ";
        writerStrList = item->text(headerMap.value("Writer")).split(", ", QString::SkipEmptyParts);
        for(int i=0; i<writerStrList.size(); ++i){
            if(links !=0 && !links->writerLinks.value(writerStrList[i]).isEmpty()){
                writerStr.append("<a href=\""+links->writerLinks.value(writerStrList[i])+"\">"+writerStrList[i]+"</a>");
            }
            else
                writerStr.append(writerStrList[i]);
            if(i<writerStrList.size()-1)
                writerStr.append(", ");
        }
    }
    writerStr.append("\n");
    writer->setText(writerStr);
    //    director->setAlignment(Qt::AlignHCenter);
    if(!item->text(headerMap.value("imdbID")).isEmpty()){
        imdbURL = "http://www.imdb.com/title/" + item->text(headerMap.value("imdbID"))+"/";
        imdbPage->setText("<a href=\""+imdbURL + "\" >Go to IMDb page</a>");
        imdbPage->setToolTip(imdbURL);
    }
    else{
        imdbPage->clear();
    }

    int height = vLayout->heightForWidth(250);
    setMinimumHeight(height);
    resize(maximumWidth(),height);
}

SideBar::~SideBar(){
    int childCount = children().size();
    for(int i=0; i<childCount; ++i){
        delete children().at(0);
    }
}

void SideBar::clear(){
    imdbPage->clear();
    picture->clear();
    title->clear();
    plot->clear();
    director->clear();
    starring->clear();
}

void SideBar::linkActivated(QString link){

    if(!link.isEmpty()){
        QProcess pro;
        QStringList args;
        args.append(link);
        if(QSysInfo::kernelType() == "linux")
            pro.startDetached("xdg-open", args);
        else
            if(QSysInfo::productType() == "windows")
                pro.startDetached("cmd /Q /C \"start " + link +"\"");
    }
}
void SideBar::starringLinkHovered(QString link){
    starring->setToolTip(link);
}

void SideBar::writerLinkHovered(QString link){
    writer->setToolTip(link);
}
void SideBar::directorLinkHovered(QString link){
    director->setToolTip(link);
}
