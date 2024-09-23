
#include "LeeCommand.h"
#include "LeeDefination.h"
#include "qdir.h"



LeeTdCommand::LeeTdCommand(QString inDir, QStringList inOldNames, QStringList inNewNames, QUndoCommand *parent)
    :QUndoCommand(parent)
{
    dir = inDir;
    m_OldNames = inOldNames;
    m_NewNames = inNewNames;

    ChangeFileName(inDir,m_OldNames,m_NewNames);
}

void LeeTdCommand::undo()
{
    LEELOG("undo..");
    if(m_OldNames.isEmpty() || m_NewNames.isEmpty()) {
        LEELOG("Print abc.....");
        return;
    }

    return ChangeFileName(dir,m_NewNames,m_OldNames);

}

void LeeTdCommand::redo()
{
    LEELOG("redo");
}

bool LeeTdCommand::ExistsName(QString inFileName)
{
    if(inFileName.isNull() || inFileName.isEmpty()){
        LEELOG("empty Dir");
        return false;
    }
    return QFile::exists(inFileName);
}

void LeeTdCommand::ChangeFileName(QString inDir, QStringList inOldNames, QStringList inNewNames)
{
    QDir nDir(inDir);
    QStringList files = nDir.entryList(QDir::Files);

#pragma omp for paranell orderred
    for(int i=0;i < files.length() ; i++){
        if(!ExistsName(inOldNames[i]) ||
            ExistsName(inNewNames[i]))
            continue;

        QString nName = inNewNames[i];
        if(QFile(inNewNames[i]).exists())
            nName += QString::number(i);
        nDir.rename(inOldNames[i],nName);
    }
}
