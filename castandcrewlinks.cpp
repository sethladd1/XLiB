#include "castandcrewlinks.h"

void CastAndCrewLinks::addCastLinks(QStringList names, QStringList links){
    for(int i=0; i<names.size(); ++i){
        if(links.size()>i){

                castLinks.insert(names[i],links[i]);
        }
        else{

                castLinks.insert(names[i],"");
        }
    }
}
void CastAndCrewLinks::addWriterLinks(QStringList names, QStringList links){
    for(int i=0; i<names.size(); ++i){
        if(links.size()>i){

                writerLinks.insert(names[i],links[i]);
        }
        else{

                writerLinks.insert(names[i],"");
        }
    }
}
void CastAndCrewLinks::addDirectorLinks(QStringList names, QStringList links){
    for(int i=0; i<names.size(); ++i){

        if(links.size()>i){

                directorLinks.insert(names[i],links[i]);
        }
        else{
                directorLinks.insert(names[i],"");
        }
    }
}
