#ifndef GETMOVIEDATAPROMPTDIALOG_H
#define GETMOVIEDATAPROMPTDIALOG_H
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

struct lineEditStruct{

    QLineEdit *movie;
    QLineEdit *year;
    QLineEdit *imdbID;
};

class GetMovieDataPromptDialog : public QDialog
{
    Q_OBJECT

public:
    GetMovieDataPromptDialog(QList<QTreeWidgetItem*> items, int titleCol, int yearCol, QString _imageFolder, QWidget *parent=0);
    GetMovieDataPromptDialog(QList<QPair<QString, QString> > movie_year, QString _imageFolder="", QWidget *parent=0);
    QList<lineEditStruct*> lineEdits();
    QString getImageFolder();


private:
    void layout();
    QVBoxLayout *vBox;
    QHBoxLayout *hLayout;
    QHBoxLayout *imageFolderLine;
    QPushButton *choose;
    QLineEdit *imageFolderEdit;
    QDialogButtonBox *buttons;
    QLabel *header;
    QLineEdit *movieEdit;
    QLineEdit *yearEdit;
    QLineEdit *imdbIDEdit;
    QTreeWidget *tree;
    QTreeWidgetItem *head;
    QTreeWidgetItem *newItem;
    QLabel *imgFolder;

    QString imageFolder;
    QList<lineEditStruct*> _lineEdits;
    QMap<QWidget*, lineEditStruct*> movieButtonEditMap;
    QMap<QWidget*, lineEditStruct*> imdbButtonEditMap;
    QRadioButton *useMovieYear;
    QRadioButton *useimdbID;
private slots:
    void fileDialog();
    void movieToggle(bool checked);
};


#endif // GETDATAPROMPTDIALOG_H
