#ifndef LEERENAMETOOL_H
#define LEERENAMETOOL_H

#include <QWidget>
#include <QPushButton>
#include <QDir>
#include <QFile>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QFileSystemModel>
#include <QDebug>
#include <QFileDialog>
#include "LeeCommand.h"
#include "Spoiler.h"
#include "qlineedit.h"
#include "QKeyEvent"
#include <QUndoStack>
#include <QRegularExpression>

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

    //On Undo
    void OnUndo_Clicked();

    QString& NumFilter(QString filename,QString &search);

protected:
    Ui::leeRenameTool *ui;

    void ImplanteTreeView(QString directory);
    void ImplanteSearchAndReplace();

    void InitCommand(const QString inDir,const QStringList inOldNames,const QStringList inNewNames);

    QFileSystemModel* fileSystemModel;
    QString lCurrentDirName;
    QDir lDir;
    QString lPrefix,lSuffix,lnewname,lSearch,lReplace;
    QStringList lpictures,l3DFiles,lallFiles,lPng,ljpg,ljpeg;
    QStringList defaultnametiles;

    QStringList ValidNames;
    QList<QStringList> leeFilters;
    QStringList Currentnamefiles;
    QLineEdit* sline;
    QLineEdit *rline;
    Section* spoiler;
    QPushButton* ReplaceBtn;

    QString lRemoveAllDigit(QString iStr);

    QString ltestDemo();

    QString GetInputName(int inIdx,const QString inOldFilename);

    QStringList GetFileNames();

    QUndoStack* undo_stack;

    bool IsValidName(const QStringList inValidNames,const QString inNewName);

private:
   void keyPressEvent(QKeyEvent *event) override;
   QList<QStringList> processNames;
   int step=0;
};
#endif // LEERENAMETOOL_H
