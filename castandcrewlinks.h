#ifndef CASTANDCREWLINKS_H
#define CASTANDCREWLINKS_H
#include <QtCore>
struct CastAndCrewLinks{
    QHash<QString, QString> castLinks;
    QHash<QString, QString> directorLinks;
    QHash<QString, QString> writerLinks;
    void addCastLinks(QStringList names, QStringList links = QStringList());
    void addWriterLinks(QStringList names, QStringList links = QStringList());
    void addDirectorLinks(QStringList names, QStringList links = QStringList());
};

#endif // CASTANDCREWLINKS_H
