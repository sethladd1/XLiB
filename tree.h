#ifndef TREE_H
#define TREE_H
#include <QtCore>
#include <QTreeWidget>
#include <QtGui>
#include <QSysInfo>
#include <QMessageBox>
#include <QLineEdit>
#include <QFileDialog>
class Tree : public QTreeWidget
{

    Q_OBJECT

public:
    Tree(const QStringList columns, QList<QTreeWidgetItem*> items, QWidget *parent = 0);
    Tree(const QStringList columns, QWidget *parent = 0);
    Tree(QWidget *parent = 0);
    void readItemsFromXML(QXmlStreamReader *reader, QString treeName);
    QMap<QString,int> getHeaderMap();
    bool writeToFile(QXmlStreamWriter *writer, QString treeName);
    void addHeader(QString header);
    void addTopLevelItem(QTreeWidgetItem *item);
    void addTopLevelItems(const QList<QTreeWidgetItem *> &items);
private:
    QList<QString> visibleHeaders;
    QTreeWidgetItem* readDirectoryFromXML(QXmlStreamReader *reader);
    void writeDirectory(QXmlStreamWriter *writer, QTreeWidgetItem *item);
    QList<QTreeWidgetItem> clipBoard;
    void createContextMenu();
    void setConnections();
    QLineEdit *lineEdit;
    QModelIndex lineEditIndex;
    QTreeWidgetItem* curItem;
    QTimer timer;
    void keyPressEvent(QKeyEvent *event);
private slots:
    void respondToClick(QTreeWidgetItem* item, int col);
    void editFinished();

public slots:
    void openFile(QTreeWidgetItem *item, int column);
    void activate(QModelIndex index);

};

#endif // TREE_H
