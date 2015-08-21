#include "tree.h"
#include <string>
Tree::Tree(QWidget *parent)
    : QTreeWidget(parent){
    setIconSize(QSize(25,25));
    setConnections();
    timer.setSingleShot(true);
    setMouseTracking(true);
}
Tree::Tree(const QStringList columns, QList<QTreeWidgetItem*> items, QWidget *parent) : QTreeWidget(parent) {
    setIconSize(QSize(25,25));
    setConnections();
    setHeaderLabels(columns);
    setMouseTracking(true);
    timer.setSingleShot(true);
    addTopLevelItems(items);
}

Tree::Tree(const QStringList columns, QWidget *parent) : QTreeWidget(parent){
    setIconSize(QSize(25,25));
    setConnections();
    setHeaderLabels(columns);
    timer.setSingleShot(true);
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
    }
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

bool Tree::readItemsFromXML(QXmlStreamReader *reader, QString treeName){
    QString name, data;
    QTreeWidgetItem *item = new QTreeWidgetItem();
    QXmlStreamAttributes att;
    QMap<QString,int> hMap = getHeaderMap();

    CastAndCrewLinks *links;
    while(reader->name().toString() != treeName || !reader->isEndElement())
    {
        if(reader->readNext()==1)
            return false;
        if(reader->isStartElement()){
            item = new QTreeWidgetItem();
            if(reader->name() == "File"){
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
                this->addTopLevelItem(item);
            }
            else
                if(reader->name() == "Directory"){
                    addTopLevelItem(readDirectoryFromXML(reader));
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
QTreeWidgetItem* Tree::readDirectoryFromXML(QXmlStreamReader *reader){
    QXmlStreamAttributes att;
    QString col, data;

    att = reader->attributes();
    QMap<QString,int> hMap = getHeaderMap();
    QTreeWidgetItem *item = new QTreeWidgetItem();
    QTreeWidgetItem *child = new QTreeWidgetItem();
    for(int i = 0; i<att.count(); ++i)
    {
        col = att[i].name().toString();
        data = att[i].value().toString();
        if(hMap.contains(col)){
            item->setText(hMap.value(col),data);
        }
        else{

            headerItem()->setText(columnCount(),col);
            item->setText(columnCount()-1,data);
            this->hideColumn(columnCount()-1);
        }
        if(col == "Icon6154"){
            item->setIcon(0, QIcon(data));
        }
    }
    reader->readNext();

    while(!(reader->name() == "Directory" && reader->isEndElement()))
    {

        if (reader->isStartElement() && reader->name() == "File"){
            att = reader->attributes();
            child = new QTreeWidgetItem;
            for(int i = 0; i<att.count(); ++i){
                col = att[i].name().toString();
                data = att[i].value().toString();
                if(col == "Icon")
                    child->setIcon(0,QIcon(data));
                if(hMap.contains(col))
                    child->text(hMap.value(col));
                else{
                    headerItem()->setText(columnCount(),col);
                    child->setText(columnCount()-1,data);
                    this->hideColumn(columnCount()-1);
                }
            }
            item->addChild(child);
        }
        else
            if(reader->name() == "Directory" && reader->isStartElement()){
                item->addChild(readDirectoryFromXML(reader));
            }
        reader->readNext();
    }
    return item;

}
bool Tree::writeToFile(QXmlStreamWriter *writer, QString treeName){
    QMap<QString, int> hMap = getHeaderMap();
    CastAndCrewLinks *links;
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
            if(topLevelItem(i)->childCount()>0){
                writeDirectory(writer, topLevelItem(i));
            }
            else{
                writer->writeStartElement("File");
                for(int j = 0; j<columnCount(); ++j){
                    writer->writeAttribute(headerItem()->text(j),topLevelItem(i)->text(j));
                }
                if(itemLinksHash.contains(topLevelItem(i))){
                    QStringList list = topLevelItem(i)->text(getHeaderMap().value("Starring")).split(", ", QString::SkipEmptyParts);
                    links = itemLinksHash.value(topLevelItem(i));
                    writer->writeStartElement("CastLinks");
                    for(int j=0; j<list.size(); ++j){

                        writer->writeAttribute(replaceNonAlphanum(list[j]), links->castLinks.value(list[j]));
                    }
                    writer->writeEndElement();
                    list = topLevelItem(i)->text(getHeaderMap().value("Director")).split(", ", QString::SkipEmptyParts);
                    writer->writeStartElement("DirectorLinks");
                    for(int j=0; j<list.size(); ++j){

                        writer->writeAttribute(replaceNonAlphanum(list[j]), links->directorLinks.value(list[j]));
                    }
                    writer->writeEndElement();
                    list = topLevelItem(i)->text(getHeaderMap().value("Writer")).split(", ", QString::SkipEmptyParts);
                    writer->writeStartElement("WriterLinks");
                    for(int j=0; j<list.size(); ++j){
                        writer->writeAttribute(replaceNonAlphanum(list[j]), links->writerLinks.value(list[j]));
                    }
                    writer->writeEndElement();
                }

                writer->writeEndElement();

            }
        }
        writer->writeEndElement();
        return true;
    }
    return false;
}
void Tree::writeDirectory(QXmlStreamWriter *writer, QTreeWidgetItem *item){
    writer->writeStartElement("Directory");
    for(int i = 0; i<columnCount(); ++i){
        writer->writeAttribute(headerItem()->text(i),item->text(i));
    }
    for(int i = 0; i<item->childCount(); ++i){
        if(item->child(i)->childCount()>0){
            writeDirectory(writer,item->child(i));
        }
        else{
            writer->writeStartElement("File");
            for(int j=0; j<columnCount();++j){
                writer->writeAttribute(headerItem()->text(i), item->text(j));
            }
            writer->writeEndElement();
        }
    }
    writer->writeEndElement();
    return;
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
        if(QFileInfo(path).isExecutable())
            pro.startDetached(path);
        else
            if(QSysInfo::kernelType() == "linux")
                pro.startDetached("xdg-open", args);
            else
                if(QSysInfo::kernelType() == "windows")
                    pro.startDetached(path);

    }
}
QMap<QString,int> Tree::getHeaderMap(){
    QMap<QString,int> headerMap;
    for(int i = 0; i<columnCount(); ++i)
        headerMap.insert(headerItem()->text(i),i);
    return headerMap;
}
void Tree::respondToClick(QTreeWidgetItem* item, int col){
    if(item == curItem && !timer.isActive()){
        lineEdit = new QLineEdit (item->text(col), this);

        lineEdit->selectAll();
        lineEdit->setFocus();
        connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(editFinished()));
        setItemWidget(currentItem(), currentColumn(), lineEdit);
        lineEdit->resize(columnWidth(col), 30);
        lineEditIndex = indexFromItem(item, col);
    }
    else{
        timer.start(1000);
        curItem = item;
        emit itemSelectionChanged();
    }
}
void Tree::editFinished(){
    QString str = lineEdit->text();
    removeItemWidget(topLevelItem(lineEditIndex.row()),lineEditIndex.column());
    topLevelItem(lineEditIndex.row())->setText(lineEditIndex.column(), str);

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
    emit itemDoubleClicked(topLevelItem(index.row()), 0);
}
void Tree::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Up){
        if(indexOfTopLevelItem(currentItem()) > 0){
            setCurrentItem(topLevelItem(indexOfTopLevelItem(currentItem())-1));
            emit itemSelectionChanged();
            if(!hasFocus())
                setFocus();
            timer.start(1000);
            curItem = currentItem();
        }
    } else
    if(event->key() == Qt::Key_Down){
        if(indexOfTopLevelItem(currentItem()) < topLevelItemCount()-1){
            setCurrentItem(topLevelItem(indexOfTopLevelItem(currentItem())+1));
            emit itemSelectionChanged();
            if(!hasFocus())
                setFocus();
            timer.start(1000);
            curItem = currentItem();
        }
    } else //if the key is a letter, set current item to first item starting with that letter
    if(event->key()>0x40 && event->key()<0x5b){
        for(int i=0; i<topLevelItemCount(); ++i){
            if(topLevelItem(i)->text(sortColumn()).at(0) >= QChar(event->key()) || topLevelItem(i)->text(sortColumn()).at(0) >= QChar(event->key()+32)){
                setCurrentItem(topLevelItem(i));
                emit itemSelectionChanged();
                return;
            }
        }
    } else
        if(event->key()>0x20){
            for(int i=0; i<topLevelItemCount(); ++i){
                if(topLevelItem(i)->text(sortColumn()).at(0) >= QChar(event->key())){
                    setCurrentItem(topLevelItem(i));
                    emit itemSelectionChanged();
                    return;
                }
            }
        }
}
