#include "columneditdialog.h"

ColumnEditDialog::ColumnEditDialog(Tree *tree, QWidget *parent) : QDialog(parent), tree(tree)
{
    hBox = new QHBoxLayout();
    vBox = new QVBoxLayout();
    grid1 = new QGridLayout();
    grid2 = new QGridLayout();
    main = new QGridLayout();
    buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Vertical, this);
    newColumn();
    int col = 0, row = 0;
    grid1->addLayout(vBox, 0, 0);
    grid1->addItem(new QSpacerItem(15,1),0,2,1,1);
    grid1->addWidget(buttons,0,3,1,3);
    for(int i = 0; i<tree->columnCount(); ++i){
        if(tree->headerItem()->text(i) != "Type6154" && tree->headerItem()->text(i) != "Icon6154" && tree->headerItem()->text(i) != "imdbID"){
            QCheckBox *checkbox = new QCheckBox(tree->headerItem()->text(i), this);
            checkbox->setChecked(!tree->isColumnHidden(i));
            checkboxes.append(checkbox);
            headers.append(QPair<QString, bool>(tree->headerItem()->text(i), tree->isColumnHidden(i)));
            grid2->addWidget(checkbox, row, col);
            ++row;
            if(row == 4){
                row = 0;
                col++;
            }
        }
    }
    main->addLayout(grid1, 0, 0);
    main->addLayout(grid2, 1, 0);
    this->setLayout(main);
    this->setFixedSize(this->sizeHint());

    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
}
ColumnEditDialog::~ColumnEditDialog(){
    int childCount = children().size();
    for(int i=0; i<childCount; ++i){
        delete children().at(0);
    }
}

void ColumnEditDialog::newColumn(){

    QHBoxLayout *box = new QHBoxLayout();
    QLabel *addColumn = new QLabel("New Column", this);
    QLineEdit *edit = new QLineEdit(this);
    addColumn->setBuddy(edit);
    QPushButton *more = new QPushButton("...", this);
    more->setFixedWidth(30);
    box->addWidget(addColumn);
    box->addWidget(edit);
    box->addWidget(more);
    vBox->addLayout(box);
    textFields.append(edit);
    connect(more, SIGNAL(pressed()), this, SLOT(newColumn()));
}

QList<QLineEdit* > ColumnEditDialog::getNewColumns()
{
    for(int i=0; i<textFields.size(); ++i){
        if(textFields[i]->text().isEmpty())
            textFields.removeAt(i);
    }
    return textFields;
}

QList<QCheckBox*> ColumnEditDialog::getExistingColumns(){
    return checkboxes;
}

