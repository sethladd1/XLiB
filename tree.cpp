#include "tree.h"
#include <string>
Tree::Tree(QWidget *parent)
    : QTreeWidget(parent){
    setIconSize(QSize(25,25));
    setConnections();
    setMouseTracking(true);
}
Tree::Tree(const QStringList columns, QList<QTreeWidgetItem*> items, QWidget *parent) : QTreeWidget(parent) {
    setIconSize(QSize(25,25));
    setConnections();
    setHeaderLabels(columns);
    setMouseTracking(true);
    addTopLevelItems(items);
}

Tree::Tree(const QStringList columns, QWidget *parent) : QTreeWidget(parent){
    setIconSize(QSize(25,25));
    setConnections();
    setHeaderLabels(columns);
    setMouseTracking(true);
}

void Tree::addTopLevelItem(QTreeWidgetItem *item){
    QString str;

    for(int i = 0; i < columnCount(); ++i){
        str = item->text(i);
        QStringList list = str.split(" ");
        str.clear();
        for(int j=1; j<list.size(); ++j){
            if(j%15 == 0 && j != list.size()-1){
                list.insert(j, "\n");
            }
            str.append(list.at(j-1));
            if(list.at(j-1) != "\n")
                str.append(" ");
        }
        if(list.size())
            str.append(list[list.size()-1]);
        item->setToolTip(i, str);
    }
    for(int c = 0; c<item->childCount(); ++c){
        for(int i = 0; i < columnCount(); ++i){
            str = item->child(c)->text(i);
            QStringList list = str.split(" ");
            str.clear();
            for(int j=1; j<list.size(); ++j){
                if(j%15 == 0 && j != list.size()-1){
                    list.insert(j, "\n");
                }
                str.append(list.at(j-1));
                if(list.at(j-1) != "\n")
                    str.append(" ");
            }
            if(list.size())
                str.append(list[list.size()-1]);
            item->child(c)->setToolTip(i, str);
        }
    }
    _topLevelItems.append(item);
    QTreeWidget::addTopLevelItem(item);

}
void Tree::addTopLevelItems(QList<QTreeWidgetItem *> &items)
{
    QString str;
    for(int l=0; l<items.size(); ++l){
        for(int i = 0; i < columnCount(); ++i){
            str = items[l]->text(i);
            QStringList list = str.split(" ");
            str.clear();
            for(int j=1; j<list.size(); ++j){
                if(j%15 == 0 && j != list.size()-1){
                    list.insert(j, "\n");
                }
                str.append(list.at(j-1));
                if(list.at(j-1) != "\n")
                    str.append(" ");
            }
            if(list.size())
                str.append(list[list.size()-1]);
            items[l]->setToolTip(i, str);
        }
        for(int c = 0; c<items[l]->childCount(); ++c){
            for(int i = 0; i < columnCount(); ++i){
                str = items[l]->child(c)->text(i);
                QStringList list = str.split(" ");
                str.clear();
                for(int j=1; j<list.size(); ++j){
                    if(j%15 == 0 && j != list.size()-1){
                        list.insert(j, "\n");
                    }
                    str.append(list.at(j-1));
                    if(list.at(j-1) != "\n")
                        str.append(" ");
                }
                if(list.size())
                    str.append(list[list.size()-1]);
                items[l]->child(c)->setToolTip(i, str);
            }
        }
    }
    _topLevelItems.append(items);
    QTreeWidget::addTopLevelItems(items);
}
QHash<QTreeWidgetItem*, CastAndCrewLinks*> Tree::getLinks(){
    return itemLinksHash;
}
void Tree::setLinks(QTreeWidgetItem *item, CastAndCrewLinks *link){
    itemLinksHash.insert(item, link);
}

void Tree::setConnections(){
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(respondToClick(QTreeWidgetItem*, int)));
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(openFile(QTreeWidgetItem*,int)));
}
bool Tree::readFile(QXmlStreamReader *reader, QTreeWidgetItem* item){
    QString name, data;
    QXmlStreamAttributes att;
    QMap<QString,int> hMap = getHeaderMap();
    CastAndCrewLinks *links;
    att = reader->attributes();
    for(int i = 0; i<att.count(); ++i)
    {
        name = att[i].name().toString();
        data = att[i].value().toString();
        if(hMap.contains(name)){
            item->setText(hMap.value(name),data);
        }
        else{
            headerItem()->setText(columnCount(),name);
            item->setText(columnCount()-1,data);
            hideColumn(columnCount()-1);
        }
        if(name == "Icon6154"){
            if(QFileInfo(data).exists())
                item->setIcon(0, QIcon(data));
        }
    }
    links = new CastAndCrewLinks();
    links->addCastLinks(item->text(hMap.value("Starring")).split(", "));
    links->addWriterLinks(item->text(hMap.value("Writer")).split(", "));
    links->addDirectorLinks(item->text(hMap.value("Director")).split(", "));
    if(reader->readNext() == 1){
        return false;
    }
    QStringList sList;
    while(!(reader->isEndElement() && reader->name() == "File")){
        if(reader->isStartElement()){
            if(reader->name()=="CastLinks"){
                att = reader->attributes();
                sList = item->text(hMap.value("Starring")).split(", ", QString::SkipEmptyParts);
                for(int i = 0; i<att.count(); ++i)
                {
                    name = att[i].name().toString();
                    data = att[i].value().toString();
                    int index = locate(sList, name);
                    if(index>=0)
                        links->castLinks.insert(sList[index], data);
                }
            }
            else if(reader->name()=="DirectorLinks"){
                att = reader->attributes();
                sList = item->text(hMap.value("Director")).split(", ", QString::SkipEmptyParts);
                for(int i = 0; i<att.count(); ++i)
                {
                    name = att[i].name().toString();
                    data = att[i].value().toString();
                    int index = locate(sList, name);
                    if(index>=0)
                        links->directorLinks.insert(sList[index], data);
                }
            }
            else if(reader->name()=="WriterLinks"){
                att = reader->attributes();
                sList = item->text(hMap.value("Writer")).split(", ", QString::SkipEmptyParts);
                for(int i = 0; i<att.count(); ++i)
                {
                    name = att[i].name().toString();
                    data = att[i].value().toString();
                    int index = locate(sList, name);
                    if(index>=0)
                        links->writerLinks.insert(sList[index], data);
                }
            }
            if(reader->readNext() == 1){
                return false;
            }
        }
        else
            if(reader->readNext() == 1){
                return false;
            }
    }
    itemLinksHash.insert(item, links);
    return true;
}

bool Tree::readItemsFromXML(QXmlStreamReader *reader, QString treeName){
    QString name, data;
    QTreeWidgetItem *item = new QTreeWidgetItem();
    QXmlStreamAttributes att;
    QMap<QString,int> hMap = getHeaderMap();
    while(reader->name().toString() != treeName || !reader->isEndElement())
    {
        if(reader->readNext()==1)
            return false;
        if(reader->isStartElement()){
            item = new QTreeWidgetItem();
            if(reader->name() == "File"){
                if(!readFile(reader, item))
                    return false;
                this->addTopLevelItem(item);
            }
            else
                if(reader->name() == "Directory"){
                    if(!readDirectory(reader, item))
                        return false;
                    this->addTopLevelItem(item);
                }
                else{
                    if(reader->name()=="Header"){
                        int j=0;
                        while(reader->name() != "Header" || !reader->isEndElement()){
                            if(reader->readNext() == 1){
                                return false;
                            }
                            else if(reader->isStartElement() && reader->name() == "Column"){
                                att = reader->attributes();
                                for(int i = 0; i<att.size(); ++i)
                                {
                                    name = att[i].name().toString();
                                    data = att[i].value().toString();
                                    if(name == "Name")
                                        headerItem()->setText(j, data);
                                    else if(name=="Width"){
                                        if(data.toInt())
                                            setColumnWidth(j, data.toInt());
                                    }
                                    else if(name=="Visible"){
                                        if(data=="true")
                                            showColumn(j);
                                        else
                                            hideColumn(j);
                                    }
                                }
                                ++j;
                            }
                        }
                        hMap = getHeaderMap();
                    }
                }
        }

    }
    return true;
}
/* not tested or used, for future versions*/
bool Tree::readDirectory(QXmlStreamReader *reader, QTreeWidgetItem* item){
    QString name, data;
    QXmlStreamAttributes att;
    QMap<QString,int> hMap = getHeaderMap();
    CastAndCrewLinks *links;
    att = reader->attributes();
    QTreeWidgetItem *child = new QTreeWidgetItem();
    for(int i = 0; i<att.count(); ++i)
    {
        name = att[i].name().toString();
        data = att[i].value().toString();
        if(hMap.contains(name)){
            item->setText(hMap.value(name),data);
        }
        else{
            headerItem()->setText(columnCount(),name);
            item->setText(columnCount()-1,data);
            hideColumn(columnCount()-1);
        }
        if(name == "Icon6154"){
            if(QFileInfo(data).exists())
                item->setIcon(0, QIcon(data));
        }
    }
    links = new CastAndCrewLinks();
    links->addCastLinks(item->text(hMap.value("Starring")).split(", "));
    links->addWriterLinks(item->text(hMap.value("Writer")).split(", "));
    links->addDirectorLinks(item->text(hMap.value("Director")).split(", "));
    if(reader->readNext() == 1){
        return false;
    }
    QStringList sList;
    while(!(reader->isStartElement() && reader->name() == "File") && !(reader->isEndElement() && reader->name() == "Directory")){
        if(reader->isStartElement()){
            if(reader->name()=="CastLinks"){
                att = reader->attributes();
                sList = item->text(hMap.value("Starring")).split(", ", QString::SkipEmptyParts);
                for(int i = 0; i<att.count(); ++i)
                {
                    name = att[i].name().toString();
                    data = att[i].value().toString();
                    int index = locate(sList, name);
                    if(index>=0)
                        links->castLinks.insert(sList[index], data);
                }
            }
            else if(reader->name()=="DirectorLinks"){
                att = reader->attributes();
                sList = item->text(hMap.value("Director")).split(", ", QString::SkipEmptyParts);
                for(int i = 0; i<att.count(); ++i)
                {
                    name = att[i].name().toString();
                    data = att[i].value().toString();
                    int index = locate(sList, name);
                    if(index>=0)
                        links->directorLinks.insert(sList[index], data);
                }
            }
            else if(reader->name()=="WriterLinks"){
                att = reader->attributes();
                sList = item->text(hMap.value("Writer")).split(", ", QString::SkipEmptyParts);
                for(int i = 0; i<att.count(); ++i)
                {
                    name = att[i].name().toString();
                    data = att[i].value().toString();
                    int index = locate(sList, name);
                    if(index>=0)
                        links->writerLinks.insert(sList[index], data);
                }
            }
            if(reader->readNext() == 1){
                return false;
            }
        }
        else
            if(reader->readNext() == 1){
                return false;
            }
        itemLinksHash.insert(item, links);
    }
    while(!(reader->isEndElement() && reader->name() == "Directory")){
        child = new QTreeWidgetItem();
        if(reader->name().toString() == "File" && reader->isStartElement()){
            if(!readFile(reader, child))
                return false;
        }
        if(reader->readNext() == 1)
            return false;
        item->addChild(child);
    }
    return true;

}
bool Tree::writeToFile(QXmlStreamWriter *writer, QString treeName){
    QMap<QString, int> hMap = getHeaderMap();
    CastAndCrewLinks *links;
    QTreeWidgetItem* item;
    QTreeWidgetItem *child;
    if(writer->device()->isOpen()&&writer->device()->isWritable()){
        writer->writeStartElement(treeName);
        writer->writeStartElement("Header");
        for(int i=0;i<columnCount();++i){
            writer->writeStartElement("Column");
            writer->writeAttribute("Name", headerItem()->text(i));
            writer->writeAttribute("Width", QString().number(columnWidth(i)));
            QString visible = isColumnHidden(i) ? "false" : "true";
            writer->writeAttribute("Visible", visible);
            writer->writeEndElement();
        }
        writer->writeEndElement();
        for(int i = 0; i<topLevelItemCount();++i){
            item = topLevelItem(i);
            if(item->childCount()>0){
                writeDirectory(item, writer);
            }
            else{
                writeFile(item, writer);
            }
        }
        writer->writeEndElement();
        return true;
    }
    return false;
}

void Tree::writeFile(QTreeWidgetItem *item, QXmlStreamWriter *writer){
    QMap<QString, int> hMap = getHeaderMap();
    CastAndCrewLinks *links;
    writer->writeStartElement("File");
    for(int j = 0; j<columnCount(); ++j){
        writer->writeAttribute(headerItem()->text(j),item->text(j));
    }
    if(itemLinksHash.contains(item)){
        QStringList list = item->text(hMap.value("Starring")).split(", ", QString::SkipEmptyParts);
        links = itemLinksHash.value(item);
        writer->writeStartElement("CastLinks");
        for(int j=0; j<list.size(); ++j){
            writer->writeAttribute(replaceNonAlphanum(list[j]), links->castLinks.value(list[j]));
        }
        writer->writeEndElement();
        list = item->text(hMap.value("Director")).split(", ", QString::SkipEmptyParts);
        writer->writeStartElement("DirectorLinks");
        for(int j=0; j<list.size(); ++j){
            writer->writeAttribute(replaceNonAlphanum(list[j]), links->directorLinks.value(list[j]));
        }
        writer->writeEndElement();
        list = item->text(hMap.value("Writer")).split(", ", QString::SkipEmptyParts);
        writer->writeStartElement("WriterLinks");
        for(int j=0; j<list.size(); ++j){
            writer->writeAttribute(replaceNonAlphanum(list[j]), links->writerLinks.value(list[j]));
        }
        writer->writeEndElement();
    }
    writer->writeEndElement();
}

void Tree::writeDirectory(QTreeWidgetItem *item, QXmlStreamWriter *writer){
    QMap<QString, int> hMap = getHeaderMap();
    CastAndCrewLinks *links;
    writer->writeStartElement("Directory");
    for(int j = 0; j<columnCount(); ++j){
        writer->writeAttribute(headerItem()->text(j),item->text(j));
    }
    if(itemLinksHash.contains(item)){
        QStringList list = item->text(hMap.value("Starring")).split(", ", QString::SkipEmptyParts);
        links = itemLinksHash.value(item);
        writer->writeStartElement("CastLinks");
        for(int j=0; j<list.size(); ++j){
            writer->writeAttribute(replaceNonAlphanum(list[j]), links->castLinks.value(list[j]));
        }
        writer->writeEndElement();
        list = item->text(hMap.value("Director")).split(", ", QString::SkipEmptyParts);
        writer->writeStartElement("DirectorLinks");
        for(int j=0; j<list.size(); ++j){
            writer->writeAttribute(replaceNonAlphanum(list[j]), links->directorLinks.value(list[j]));
        }
        writer->writeEndElement();
        list = item->text(hMap.value("Writer")).split(", ", QString::SkipEmptyParts);
        writer->writeStartElement("WriterLinks");
        for(int j=0; j<list.size(); ++j){
            writer->writeAttribute(replaceNonAlphanum(list[j]), links->writerLinks.value(list[j]));
        }
        writer->writeEndElement();
    }
    for(int c=0; c<item->childCount();++c){
        writeFile(item->child(c), writer);
    }
    writer->writeEndElement();
}
QString Tree::replaceNonAlphanum(QString str) const{
    QString newString;
    QString okChars = "_.-";
    for(int i=0; i<str.size(); ++i){
        if(str[i].isLetterOrNumber() || okChars.contains(str[i])){
            newString.append(str[i]);
        }
        else
            newString.append("_");
    }
    return newString;
}
int Tree::locate(QStringList list, QString str) const{
    for(int i=0; i<list.size(); ++i){
        if(replaceNonAlphanum(list[i])==str){
            return i;
        }
    }
    return -1;
}

void Tree::openFile(QTreeWidgetItem *item, int column){
    if(item !=0){
        QProcess pro;
        QStringList args;
        QMap<QString, int> hMap = getHeaderMap();
        int col = hMap.value("Path");
        QString path = item->text(col);
        QString name = item->text(hMap.value("Title"));
        if(!QFileInfo(path).exists()){
            if(QMessageBox::information(this,tr("File Not Found"), path + tr(" does not exist. Locate File?"), QMessageBox::Yes, QMessageBox::No)== QMessageBox::Yes){
                path = QFileDialog::getOpenFileName(this, "Locate " + name);
                item->setText(col,path);
            }
            else
                return;
        }
        args.append(path);
        if(QSysInfo::kernelType() == "linux")
            pro.startDetached("xdg-open", args);
        else
            if(QSysInfo::productType() == "windows")
                pro.startDetached("cmd /Q /C \"start " + path +"\"");

    }
}
QMap<QString,int> Tree::getHeaderMap(){
    QMap<QString,int> headerMap;
    for(int i = 0; i<columnCount(); ++i)
        headerMap.insert(headerItem()->text(i),i);
    return headerMap;
}
void Tree::editItem(QTreeWidgetItem *item, int column){
    lineEdit = new QLineEdit(item->text(column), parentWidget());
    setItemWidget(item, column, lineEdit);
    lineEdit->selectAll();
    lineEdit->setFocus();
    connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(editFinished()));

    lineEdit->resize(columnWidth(column), 30);
    lineEditColumn = column;
    lineEditItem = item;
}

void Tree::respondToClick(QTreeWidgetItem* item, int col){
    emit itemSelectionChanged();

}
void Tree::editFinished(){
    QString str = lineEdit->text();
    removeItemWidget(lineEditItem,lineEditColumn);
    lineEditItem->setText(lineEditColumn, str);
    lineEdit->hide();

}
void Tree::addHeader(QString header){
    setColumnCount(columnCount()+1);
    headerItem()->setText(columnCount()-1,header);
}
void Tree::removeItems(QList<QTreeWidgetItem *> items){
    for(int i = 0; i<items.size(); ++i){
        itemLinksHash.remove(items[i]);
        delete items[i];
    }
}

void Tree::activate(QModelIndex index){
    if(topLevelItem(index.row()))
        emit itemDoubleClicked(topLevelItem(index.row()), 0);
}
void Tree::keyPressEvent(QKeyEvent *event){
    QTreeWidgetItem* item = currentItem();
    QTreeWidgetItem* par = item->parent();
    if(event->key() == Qt::Key_Up){
        if(par==0){
            if(indexOfTopLevelItem(currentItem()) > 0){
                setCurrentItem(topLevelItem(indexOfTopLevelItem(currentItem())-1));
                emit itemSelectionChanged();
                if(!hasFocus())
                    setFocus();
            }
        }
        else{
            int index = par->indexOfChild(item);
            if(index > 0){
                setCurrentItem(par->child(index-1));
            }
            else{
                setCurrentItem(par);
            }
            if(!hasFocus())
                setFocus();
            emit itemSelectionChanged();
        }
    }
    else
        if(event->key() == Qt::Key_Down){
            if(par==0){
                int index = indexOfTopLevelItem(item);
                if(index < topLevelItemCount()-1){
                    setCurrentItem(topLevelItem(index+1));
                    emit itemSelectionChanged();
                    if(!hasFocus())
                        setFocus();
                }
            }
            else{
                int index = par->indexOfChild(item);
                if(index < par->childCount()-1){
                    setCurrentItem(par->child(index+1));
                    emit itemSelectionChanged();
                    if(!hasFocus())
                        setFocus();
                }
                else{
                    if(indexOfTopLevelItem(par) < topLevelItemCount()-1){
                        setCurrentItem(topLevelItem(indexOfTopLevelItem(par)+1));
                        emit itemSelectionChanged();
                        if(!hasFocus())
                            setFocus();
                    }
                }
            }
        }
        else
            if(event->key() == Qt::Key_Right){
                if(hasFocus())
                    if(item->childCount())
                        setCurrentItem(item->child(0));
            }
            else //if the key is a letter, set current item to first item starting with that letter
                if(event->key()>0x40 && event->key()<0x5b){
                    for(int i=0; i<topLevelItemCount(); ++i){
                        if(topLevelItem(i)->text(sortColumn()).at(0) >= QChar(event->key()) || topLevelItem(i)->text(sortColumn()).at(0) >= QChar(event->key()+32)){
                            setCurrentItem(topLevelItem(i));
                            emit itemSelectionChanged();
                            return;
                        }
                    }
                }
                else
                    // if key is non-alpha printable character, set current item to first item starting with that character
                    if(event->key()>0x21 &&event->key()<0x7e){
                        for(int i=0; i<topLevelItemCount(); ++i){
                            if(topLevelItem(i)->text(sortColumn()).at(0) >= QChar(event->key())){
                                setCurrentItem(topLevelItem(i));
                                emit itemSelectionChanged();
                                return;
                            }
                        }
                    }
}
