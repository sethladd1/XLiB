#include "getmoviedata.h"

GetMovieData::GetMovieData(QString title, QString year, bool longPlot, QObject *parent)
    : QObject(parent), _year(year), _season(0), _episode(0)
{
    base = "http://www.omdbapi.com/?";
    title = title.trimmed();
    TV = false;
    title.replace(" ", "+");
    QString query = base + "t=" + title + "&y=" + _year;
    query += longPlot ? "&plot=full&r=xml" : "&plot=short&r=xml";
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    
    reply = manager->get(QNetworkRequest(QUrl(query)));
    
    connect(reply, SIGNAL(finished()), this, SLOT(readxml()));
}

GetMovieData::GetMovieData(QString series, int season, int episode, QString year, bool longPlot, QObject *parent)
    : QObject(parent), _series(series), _year(year), _season(season), _episode(episode)
{
    base = "http://www.omdbapi.com/?";
    TV = true;
    series = series.trimmed();
    series.replace(" ", "+");
    QString query = base + "t=" + series + "&season=" + QString().number(season) + "&episode=" + QString().number(episode) + "&y=" + _year;
    query += longPlot ? "&plot=full&r=xml" : "&plot=short&r=xml";
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    reply = manager->get(QNetworkRequest(QUrl(query)));
    connect(reply, SIGNAL(finished()), this, SLOT(readxml()));
}

GetMovieData::GetMovieData(QString imdbID, bool longPlot, QObject *parent) : QObject(parent){
    base = "http://www.omdbapi.com/?";
    imdbID = imdbID.trimmed();
    _imdbID = imdbID;
    QString query = base + "i=" + _imdbID;
    query += longPlot ? "&plot=full&r=xml" : "&plot=short&r=xml";
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    reply = manager->get(QNetworkRequest(QUrl(query)));
    connect(reply, SIGNAL(finished()), this, SLOT(readxml()));
}

QString GetMovieData::series(){
    return _series;
}

bool GetMovieData::isTV(){
    return TV;
}

int GetMovieData::season(){
    return _season;
}

int GetMovieData::episode(){
    return _episode;
}

QString GetMovieData::year(){
    return _year;
}

QString GetMovieData::writers(){
    return _writers;
}

QString GetMovieData::starring(){
    return _starring;
}

QImage GetMovieData::poster(){
    return _poster;
}

QString GetMovieData::movie(){
    return _movie;
}

QString GetMovieData::released(){
    return _released;
}

QString GetMovieData::genres(){
    return _genres;
}

QString GetMovieData::rating(){
    return _rating;
}

QString GetMovieData::language(){
    return _language;
}

QString GetMovieData::country(){
    return _country;
}

QString GetMovieData::posterURL(){
    return _posterURL.toString();
}

QString GetMovieData::plot(){
    return _plot;
}

bool GetMovieData::success(){
    return _success;
}

QString GetMovieData::director(){
    return _director;
}
QString GetMovieData::imdbID(){
    return _imdbID;
}
CastAndCrewLinks* GetMovieData::peopleLinks(){
    return links;
}

void GetMovieData::readxml(){
    QXmlStreamReader reader(reply);
    QXmlStreamAttributes att;
    reader.readNextStartElement();
    
    if(reader.name().toString() == "root"){
        att = reader.attributes();
        for(int i = 0; i<att.count(); ++i){
            if(att.at(i).name().toString() == "response"){
                _success = (att.at(i).value().toString() == "True");
                if(!_success){
                    emit finishedDownloading(this);
                    return;
                }
            }
        }
    }
    while(reader.readNextStartElement()){
        if(reader.name().toString() == "movie"){
            att = reader.attributes();
            for(int i = 0; i<att.count(); ++i){
                if(att.at(i).name().toString() == "title"){
                    _movie = att.at(i).value().toString();
                }
                if(att.at(i).name().toString() == "year"){
                    _year = att.at(i).value().toString();
                }
                if(att.at(i).name().toString() == "rated"){
                    _rating = att.at(i).value().toString();
                }
                if(att.at(i).name().toString() == "genre"){
                    _genres = att.at(i).value().toString();
                }
                if(att.at(i).name().toString() == "released"){
                    _released = att.at(i).value().toString();
                }
                if(att.at(i).name().toString() == "director"){
                    _director = att.at(i).value().toString();
                }
                if(att.at(i).name().toString() == "writer"){
                    _writers = att.at(i).value().toString();
                }
                if(att.at(i).name().toString() == "plot"){
                    _plot = att.at(i).value().toString();
                    _plot.replace("&quot;", "\"");
                }
                if(att.at(i).name().toString() == "language"){
                    _language = att.at(i).value().toString();
                }
                if(att.at(i).name().toString() == "actors"){
                    _starring = att.at(i).value().toString();
                }
                if(att.at(i).name().toString() == "poster"){
                    _posterURL = att.at(i).value().toString();
                }
                if(att.at(i).name().toString() == "country"){
                    _country = att.at(i).value().toString();
                }
                if(att.at(i).name().toString() == "imdbID"){
                    _imdbID = att.at(i).value().toString();
                    creditsURL = "http://www.imdb.com/title/"+_imdbID + "/fullcredits";
                }
                if(att.at(i).name().toString() == "type"){
                    if(att.at(i).value().toString()=="episode")
                        TV=true;
                    else
                        TV=false;
                    
                }
            }
            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            reply = manager->get(QNetworkRequest(_posterURL));
            connect(reply, SIGNAL(finished()), this, SLOT(setPoster()));
        }
    }
}

void GetMovieData::setPoster(){
    _poster = _poster.fromData(reply->readAll());
    QString url = "http://www.imdb.com/title/"+_imdbID+"/";
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    reply = manager->get(QNetworkRequest(QUrl(url)));
    connect(reply, SIGNAL(finished()), this, SLOT(getLeadingRoles()));
}
void GetMovieData::getLeadingRoles(){
    QRegExp regExp;
    QString html(reply->readAll());
    QString name;
    bool foundNew = false;
    html.remove("\n");
    QStringList actorsList = _starring.split(", ", QString::SkipEmptyParts);
    QString foundActors;
    int start = html.indexOf("Stars:");
    int size = html.indexOf("See full cast and crew")-start;
    regExp.setPattern("itemprop=\"name\">.{3,30}<");
    if(start>0 && size>0){
        html = html.mid(start, size);
        start = html.indexOf(regExp);
        while(start>0){
            start+=16;
            size = html.indexOf("</", start)-start;
            name = html.mid(start, size);
            if(!actorsList.contains(name.trimmed(), Qt::CaseSensitive)){
                foundActors.append(name.trimmed()+", ");
                foundNew = true;
            }
            start = html.indexOf(regExp,start);
        }
        if(foundNew)
            _starring.insert(0, foundActors);
    }
    
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    reply = manager->get(QNetworkRequest(QUrl(creditsURL)));
    connect(reply, SIGNAL(finished()), this, SLOT(readPeopleLinks())); 
}

void GetMovieData::readPeopleLinks(){
    QStringList directorsList = _director.split(", ", QString::SkipEmptyParts);
    QStringList writersList =  _writers.split(", ", QString::SkipEmptyParts);
    QStringList actorsList = _starring.split(", ", QString::SkipEmptyParts);
    QRegExp regExp;
    QString html(reply->readAll());
    html.remove("\n");
    QString pattern;
    QString link;
    QString person;
    links = new CastAndCrewLinks;
    for(int i = 0; i<directorsList.size(); ++i){
        person = directorsList[i];
        if(person.contains("(")){
            person.truncate(person.indexOf("("));
            person = person.trimmed();
        }
        pattern = "href=\"/name/nm[0-9]{7,7}.{0,100}" + person;
        regExp.setPattern(pattern);
        int index = regExp.indexIn(html);
        if(index>0){
            link = "http://www.imdb.com" + html.mid(index+6, 16);
            links->directorLinks.insert(directorsList[i], link);
        }
    }
    for(int i = 0; i<actorsList.size(); ++i){
        person = actorsList[i];    
        if(person.contains("(")){
            person.truncate(person.indexOf("("));
            person = person.trimmed();
        }
        pattern = "href=\"/name/nm[0-9]{7,7}.{0,100}" + person;
        regExp.setPattern(pattern);
        int index = regExp.indexIn(html);
        if(index>0){
            link = "http://www.imdb.com" + html.mid(index+6, 16);
            links->castLinks.insert(actorsList[i], link);
        }
    }
    for(int i = 0; i<writersList.size(); ++i){
        person = writersList[i];
        if(person.contains("(")){
            person.truncate(person.indexOf("("));
            person = person.trimmed();
        }
        pattern = "href=\"/name/nm[0-9]{7,7}.{0,100}" + person;
        regExp.setPattern(pattern);
        int index = regExp.indexIn(html);
        if(index>0){
            link = "http://www.imdb.com" + html.mid(index+6, 16);
            links->writerLinks.insert(writersList[i], link);
        }
    }
    emit finishedDownloading(this);
}
