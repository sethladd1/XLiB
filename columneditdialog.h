#ifndef COLUMNEDITDIALOG_H
#define COLUMNEDITDIALOG_H

#include <QObject>

#include <QtGui>
#include <QCheckBox>
#include <QLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include "tree.h"
class ColumnEditDialog : public QDialog
{
    Q_OBJECT
public:
    ColumnEditDialog(Tree *tree, QWidget *parent = 0);
    QList<QLineEdit* > getNewColumns();
    QList<QCheckBox*> getExistingColumns();
private slots:
    void newColumn();
private:
    QHBoxLayout *hBox;
    QVBoxLayout *vBox;
    QGridLayout *grid1;
    QGridLayout *grid2;
    QGridLayout *main;
    QDialogButtonBox *buttons;
    QList<QCheckBox*> checkboxes;
    QList<QLineEdit*> textFields;
    QTreeWidget *treeWidget;
    Tree *tree;
    QList<QPair<QString, bool> > headers;

};

#endif // COLUMNEDITDIALOG_H
