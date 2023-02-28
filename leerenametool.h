#ifndef LEERENAMETOOL_H
#define LEERENAMETOOL_H

#include <QWidget>
#include <QPushButton>
#include <QDirModel>
#include <QFile>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QFileSystemModel>
#include <QDebug>
#include <QFileDialog>
#include "Spoiler.h"
#include "qlineedit.h"


QT_BEGIN_NAMESPACE
namespace Ui { class leeRenameTool; }
QT_END_NAMESPACE

class leeRenameTool : public QWidget
{
    Q_OBJECT

public:
    leeRenameTool(QWidget *parent = nullptr);
    ~leeRenameTool();


public slots:
    // browser
    void OnBrowserChanged(QString content);
    //clicked
    void OnBrowserClicked();
    // Main new name
    void OnNewNameChanged(QString newname);
    // Prefix change
    void OnPrefixChanged(QString newprefix);
    // Suffix changed
    void OnSuffixChanged(QString newsuffix);
    // Search changed
    void OnSearchChanged(QString newsuffix);
    // Replace changed
    void OnReplaceChanged(QString newsuffix);

    // on filter changed
    void OnFilterChanged(QString filter);
    // on Rename Clicked
    void OnDoRenameClicked(bool isClicked);
    // FBX filter
    void OnFilterFBX(int state);

    void OnDirectoryFilterLoader(QString path);

    void OnReplaceClicked();

    void ReGetValue();
    // picture filter
    void OnFilterBoxChanged(int state);

    QString& NumFilter(QString filename,QString &search);
private:
    Ui::leeRenameTool *ui;

    void ImplanteTreeView(QString directory);
    void ImplanteSearchAndReplace();

    QFileSystemModel* fileSystemModel;
    QString lCurrentDirName;
    QDir lDir;
    QString lPrefix,lSuffix,lnewname,lSearch,lReplace;
    QStringList lpictures,l3DFiles,lallFiles,lPng,ljpg,ljpeg;
    QList<QStringList> leeFilters;
    QLineEdit* sline;
    QLineEdit *rline;
    Section* spoiler;
    QPushButton* ReplaceBtn;

    QString lRemoveAllDigit(QString iStr);

    QString ltestDemo();
};
#endif // LEERENAMETOOL_H
