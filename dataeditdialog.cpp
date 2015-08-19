#include "dataeditdialog.h"

DataEditDialog::DataEditDialog(QList<QTreeWidgetItem *> items, QWidget *parent) : QDialog(parent)
{
    QTreeWidgetItem *item;
    for(int i = 0; i<items.size(); ++i){
        item = new QTreeWidgetItem(*items.at(i));
        items.append(item);
    }
}
