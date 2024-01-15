
#include "LeeCommand.h"
#include "qdebug.h"



LeeTdCommand::LeeTdCommand(QPushButton *m_Button, QStringList inOldNames, QStringList inNewNames, QUndoCommand *parent=nullptr)
    :QUndoCommand(parent)
{
    LeeButton = m_Button;
    m_OldNames = inOldNames;
    m_NewNames = inNewNames;
}

void LeeTdCommand::undo()
{
    if(m_OldNames.isEmpty() || m_NewNames.isEmpty()) return;

    for(int i=0;i < m_NewNames.length() ; i++){

    }
}

void LeeTdCommand::redo()
{
    qDebug() << "redo" << Qt::endl;
}
