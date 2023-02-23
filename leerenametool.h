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
    // on filter changed
    void OnFilterChanged(QString filter);
    //reload treview
    void OnReloadTreeView(bool isClicked);
    // on Rename Clicked
    void OnDoRenameClicked(bool isClicked);
    // picture filter
    void OnFilterImagePicture(int state);
    // FBX filter
    void OnFilterFBX(int state);

    void OnDirectoryFilterLoader(QString path);
private:
    Ui::leeRenameTool *ui;

    void ImplanteTreeView(QString directory);

    QFileSystemModel* fileSystemModel;
    QString lCurrentDirName;
    QDir lDir;
    QString lPrefix;
    QString lSuffix;
    QString lnewname;
    QStringList lpictures;
    QStringList l3DFiles;
    QStringList lallFiles;

};
#endif // LEERENAMETOOL_H
