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
    GetMovieData(QString series, int season, int episode, bool longPlot=false, QObject *parent = 0);
    GetMovieData(QString imdbID, bool longPlot=false, QObject *parent=0);
    ~GetMovieData();
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
    QString type();

    CastAndCrewLinks *peopleLinks();
    bool success();
    bool isTV();
    int season();
    int episode();
    QString series();
    QString seriesID();
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
    QString _imdbID;
    QImage _poster;
    QString _rating;
    QString _language;
    QString _plot;
    QUrl _posterURL;
    QString _type;
    QString _seriesID;
    bool downloading;
    QNetworkReply *reply;
    QAbstractItemModel *data;
    CastAndCrewLinks *links;
    QString creditsURL;
private slots:
    void readxml();
    void setPoster();
    void readPeopleLinks();
    /** The actor names returned by omdbapi.com are not necessarily the leading roles.
     * this function scrapes the stars list from the movies imdb page.
     **/
    void getLeadingRoles();
    void closeConnection();
signals:
    void finishedDownloading(GetMovieData* instance);
    void finishedStep(int steps);

};

#endif // GETMOVIEDATA_H
