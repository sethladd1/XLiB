#ifndef DATAEDITDIALOG_H
#define DATAEDITDIALOG_H
#include <QObject>
#include <QtCore>
#include <QTreeWidget>
#include <QtGui>
class DataEditDialog : public QDialog
{
    Q_OBJECT
public:
    DataEditDialog(QList<QTreeWidgetItem*> items, QWidget *parent=0);
private:
    QList<QTreeWidgetItem*> items;
    void setupGui(QTreeWidgetItem item) const;
private slots:
    void nextTriggered();
    void prevTriggered();

};

#endif // DATAEDITDIALOG_H
