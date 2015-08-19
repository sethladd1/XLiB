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
    director->setWordWrap(true);

    imdbPage = new Link(this);
    starring->setTextInteractionFlags(Qt::TextSelectableByMouse);
    starring->setWordWrap(true);

    plot->setTextInteractionFlags(Qt::TextSelectableByMouse);
    title->setTextInteractionFlags(Qt::TextSelectableByMouse);
    director->setTextInteractionFlags(Qt::TextSelectableByMouse);
    imdbPage->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    title->setForegroundRole(QPalette::ButtonText);
    title->setFont(QFont("Times", 13, QFont::Bold));

    vLayout = new QVBoxLayout(this);
    vLayout->addWidget(title);
    vLayout->addWidget(picture);
    vLayout->addWidget(plot);
    vLayout->addWidget(starring);
    vLayout->addWidget(director);
    vLayout->addWidget(imdbPage);
}
void SideBar::populate(QTreeWidgetItem *item, QMap<QString, int> headerMap){
    int plotHeight = 0;
    int titleHeight = 0;
    int castHeight = 0;
    int directorHeight = 0;

    QString titlestr = item->text(headerMap.value("Title"));
    if(!item->text(headerMap.value("Year")).isEmpty())
        titlestr += " ("+item->text(headerMap.value("Year")) + ")";
    title->setText(titlestr);
    title->setWordWrap(true);
    title->setAlignment(Qt::AlignHCenter);
    titleHeight = title->heightForWidth(maximumWidth());

    plot->setText("\n" + item->text(headerMap.value("Plot")) + "\n");
    plot->setWordWrap(true);
    plotHeight = plot->heightForWidth(maximumWidth());
    QString castStr;
    if(!item->text(headerMap.value("Starring")).isEmpty())
        castStr = "Starring: " + item->text(headerMap.value("Starring")) + "\n";
    starring->setText(castStr);
    castHeight = starring->heightForWidth(maximumWidth());

    QPixmap pix(200,200);
    if(!item->text(headerMap.value("Icon6154")).isEmpty())
        pix.load(item->text(headerMap.value("Icon6154")));
    else
        pix = QFileIconProvider().icon(QFileInfo(item->text(headerMap.value("Path")))).pixmap(200,200);
    pix = pix.scaled(200,200,Qt::KeepAspectRatio);
    picture->setPixmap(pix);
    picture->setAlignment(Qt::AlignCenter);
    QString directorStr;
    if(!item->text(headerMap.value("Director")).isEmpty())
        directorStr = "Director: " + item->text(headerMap.value("Director")) + "\n";
    director->setText(directorStr);

    directorHeight = director->heightForWidth(maximumWidth());
    if(!item->text(headerMap.value("imdbID")).isEmpty()){
        imdbPage->setEnabled(true);
        imdbURL = "http://www.imdb.com/title/" + item->text(headerMap.value("imdbID"));
        imdbPage->setURL(imdbURL);
        imdbPage->setText("Go to IMDb page");

    }
    else{
        imdbPage->clear();
        imdbPage->setURL(QString());
        imdbPage->setDisabled(true);
    }

    int height = titleHeight + castHeight + plotHeight + directorHeight + pix.height() + 120;

    setMinimumHeight(height);
    resize(maximumWidth(),height);
    repaint();
}
void SideBar::clear(){
    imdbPage->clear();
    imdbPage->setURL(QString());
    imdbPage->setDisabled(true);
    picture->clear();
    title->clear();
    plot->clear();
    director->clear();
    starring->clear();
}

Link::Link(QString &text, QWidget *parent, Qt::WindowFlags f) : QLabel(text, parent, f){
    setForegroundRole(QPalette::Link);
    setCursor(QCursor(Qt::PointingHandCursor));
}
Link::Link(QWidget *parent, Qt::WindowFlags f) : QLabel(parent, f){
    setForegroundRole(QPalette::Link);
    setCursor(QCursor(Qt::PointingHandCursor));
}
void Link::setURL(QString url){
    Link::url = url;
}
void Link::mousePressEvent(QMouseEvent *ev){
    if(!url.isEmpty()){
        QProcess pro;
        QStringList args;
        args.append(url);
        if(QSysInfo::kernelType() == "linux")
            pro.startDetached("xdg-open", args);
        else
            if(QSysInfo::kernelType() == "windows")
                pro.startDetached("start", args);
    }

}

