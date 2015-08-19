#ifndef GETMOVIEDATA_H
#define GETMOVIEDATA_H
#include <QtCore>
#include <QtNetwork>
#include <QImage>
#include "castandcrewlinks.h"
class GetMovieData : public QObject
{
    Q_OBJECT
public:
    GetMovieData(QString title, QString year="", bool longPlot=false, QObject *parent = 0);
    GetMovieData(QString series, int season, int episode, QString year="", bool longPlot=false, QObject *parent = 0);
    GetMovieData(QString imdbID, bool longPlot=false, QObject *parent=0);
    QString year();
    QString starring();
    QImage poster();
    QString movie();
    QString released();
    QString genres();
    QString rating();
    QString director();
    QString writers();
    QString language();
    QString country();
    QString posterURL();
    QString plot();
    QString imdbID();

    CastAndCrewLinks *peopleLinks();
    bool success();
    bool isTV();
    int season();
    int episode();
    QString series();
private:
    QString base;
    bool _success;
    QString _movie;
    QString _year;
    QString _starring;
    QString _released;
    QString  _genres;
    QString _director;
    QString _writers;
    QString _country;
    bool TV;
    int _episode;
    int _season;
    QString _series;
//    QString awards;
//    QString metaScore;
//    QString imdbRating;
    QString _imdbID;
    QImage _poster;
    QString _rating;
    QString _language;
    QString _plot;
    QUrl _posterURL;
    QNetworkReply *reply;
    QAbstractItemModel *data;
    CastAndCrewLinks *links;
    QString creditsURL;
private slots:
    void readxml();
    void setPoster();
    void readPeopleLinks();
signals:
    void finishedDownloading(GetMovieData* instance);

};

#endif // GETMOVIEDATA_H
