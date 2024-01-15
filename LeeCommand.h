#ifndef LEECOMMAND_H
#define LEECOMMAND_H

#include "qpushbutton.h"
#include <QUndoCommand>

class LeeTdCommand : public QUndoCommand
{

public:
    LeeTdCommand(QString inDir,QStringList inOldNames,QStringList inNewNames,QUndoCommand* parent=nullptr);

protected:
    void undo() override;
    void redo() override;

    bool ExistsName(QString inFileName);
    QString dir;
    QPushButton* LeeButton;
    QStringList m_OldNames,m_NewNames;

    void ChangeFileName(QString inDir,QStringList inOldNames,QStringList inNewNames);
};

#endif // LEECOMMAND_H
