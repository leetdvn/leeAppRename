#ifndef LEECOMMAND_H
#define LEECOMMAND_H

#include "qpushbutton.h"
#include <QUndoCommand>

class LeeTdCommand : public QUndoCommand
{

public:
    LeeTdCommand(QPushButton* m_Button,QStringList inOldNames,QStringList inNewNames,QUndoCommand* parent);

protected:
    void undo() override;
    void redo() override;

    QPushButton* LeeButton;
    QStringList m_OldNames,m_NewNames;

};

#endif // LEECOMMAND_H
