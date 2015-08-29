#ifndef GETSERIESDATAPROMPT_H
#define GETSERIESDATAPROMPT_H
#include <QtCore>
#include <QtGui>
#include <QTreeWidget>
#include <QWidget>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QDialog>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QRadioButton>
#include <QButtonGroup>
#include <QScrollArea>
struct seriesLineEditStruct{

    QLineEdit *episode;
    QLineEdit *season;
    QLineEdit *imdbID;
};
class GetSeriesDataPrompt : public QDialog
{
    Q_OBJECT
public:
    GetSeriesDataPrompt(QHash<QString, QList<QTreeWidgetItem*> > items, QMap<QString, int> headerMap, QString _imageFolder=QString(), QWidget *parent=0);
    ~GetSeriesDataPrompt();
    QList<QPair<QTreeWidgetItem*, seriesLineEditStruct*> > lineEdits();
    QString getImageFolder();
private:
    QFrame *hFrame();
    QFrame *vFrame();

    QHBoxLayout *hLayout;
    QHBoxLayout *imageFolderLine;
    QPushButton *choose;
    QLineEdit *imageFolderEdit;
    QDialogButtonBox *buttons;
    QLabel *header;
    QLineEdit *episodeEdit;
    QLineEdit *seasonEdit;
    QLineEdit *imdbIDEdit;
    QTreeWidgetItem *head;
    QTreeWidgetItem *newItem;
    QLabel *imgFolder;

    QString imageFolder;
    QList<seriesLineEditStruct*> _lineEdits;
    QMap<QWidget*, seriesLineEditStruct*> seriesButtonEditMap;
    QMap<QWidget*, seriesLineEditStruct*> imdbButtonEditMap;
    QRadioButton *useSSE, *useimdbID;
    QLabel *series, *file, *fileName, *season, *episode, *imdbID, *blank;
    QFrame *f;
    QList<QPair<QTreeWidgetItem*, seriesLineEditStruct*> > itemLineEdits;
private slots:
    void seriesToggled(bool checked);
    void fileDialog();
};

#endif // GETSERIESDATA_H
