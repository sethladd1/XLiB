#ifndef TREE_H
#define TREE_H
#include <QtCore>
#include <QTreeWidget>
#include <QtGui>
#include <QSysInfo>
#include <QMessageBox>
#include <QLineEdit>
#include <QFileDialog>
#include "castandcrewlinks.h"
class Tree : public QTreeWidget
{
    Q_OBJECT

public:
    Tree(const QStringList columns, QList<QTreeWidgetItem*> items, QWidget *parent = 0);
    Tree(const QStringList columns, QWidget *parent = 0);
    Tree(QWidget *parent = 0);
    bool readItemsFromXML(QXmlStreamReader *reader, QString treeName);
    QMap<QString,int> getHeaderMap();
    bool writeToFile(QXmlStreamWriter *writer, QString treeName);
    void addHeader(QString header);
    void addTopLevelItem(QTreeWidgetItem *item);
    void addTopLevelItems(QList<QTreeWidgetItem *> &items);
    QHash<QTreeWidgetItem*, CastAndCrewLinks*> getLinks();
    void setLinks(QTreeWidgetItem* item, CastAndCrewLinks* link);
    void removeItems(QList<QTreeWidgetItem*> items);
    QList<QTreeWidgetItem*> topLevelItems();
private:

    bool readDirectory(QXmlStreamReader *reader, QTreeWidgetItem* item);
    bool readFile(QXmlStreamReader *reader, QTreeWidgetItem* item);
    void writeDirectory(QTreeWidgetItem *item, QXmlStreamWriter *writer);

    void createContextMenu();
    void setConnections();
    /**
     * returns str with most non-alphanumeric characters replaced with "_" (something accepted by xml
     * reader for names)
     */
    QString replaceNonAlphanum(const QString str) const;
    /**
     * searches list for a string matching str, after replaceNonAlphanum call
     */
    int locate(QStringList list, QString str) const;
    void writeFile(QTreeWidgetItem *item, QXmlStreamWriter* writer);

    QLineEdit *lineEdit;
    int lineEditColumn;
    QTreeWidgetItem* curItem;
    QTimer timer;
    void keyPressEvent(QKeyEvent *event);
    QHash<QTreeWidgetItem*, CastAndCrewLinks*> itemLinksHash;
    QList<QTreeWidgetItem*> _topLevelItems;
    QTreeWidgetItem* lineEditItem;

private slots:
    void respondToClick(QTreeWidgetItem* item, int col);
    void editFinished();

public slots:
    void editItem(QTreeWidgetItem *item, int column);
    void openFile(QTreeWidgetItem *item, int column);
    void activate(QModelIndex index);
};

#endif // TREE_H
