#ifndef SIDEBAR_H
#define SIDEBAR_H
#include <QtCore>
#include <QObject>
#include <QtGui>
#include <QWidget>
#include <QScrollArea>
#include <QScrollBar>
#include <QLayout>
#include <QLabel>
#include <QPainter>
#include <QPicture>
#include <QTreeWidgetItem>
#include <QFileIconProvider>
#include "castandcrewlinks.h"

class SideBar : public QWidget
{
    Q_OBJECT
public:
    SideBar(QWidget *parent=0);
    void populate(QTreeWidgetItem *item, QMap<QString, int> headerMap, CastAndCrewLinks *links);
    void clear();
private:
    QScrollArea *area;
    QLabel *picture;
    QLabel *plot;
    QLabel *title;
    QLabel *director;
    QLabel *starring;
    QLabel *imdbPage;
    QLabel *writer;
    QString imdbURL;
    QVBoxLayout *vLayout;
private slots:
    void linkActivated(QString link);
    void starringLinkHovered(QString link);
    void writerLinkHovered(QString link);
    void directorLinkHovered(QString link);

};

#endif // SIDEBAR_H
