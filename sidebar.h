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
class Link :public QLabel
{

public:
    Link(QString &text, QWidget *parent=0, Qt::WindowFlags f=0);
    Link(QWidget *parent=0, Qt::WindowFlags f=0);
    void setURL(QString url);
private:
    QString url;
    void mousePressEvent(QMouseEvent *ev);

};

class SideBar : public QWidget
{
public:
    SideBar(QWidget *parent=0);
    void populate(QTreeWidgetItem *item, QMap<QString, int> headerMap);
    void clear();
private:
    QScrollArea *area;
    QLabel *picture;
    QLabel *plot;
    QLabel *title;
    QLabel *director;
    QLabel *starring;
    Link *imdbPage;
    QString imdbURL;
    QVBoxLayout *vLayout;



};

#endif // SIDEBAR_H
