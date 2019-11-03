#ifndef REDOUNDOMANAGER_H
#define REDOUNDOMANAGER_H

#include <QQueue>

#include "history.h"

class RedoUndoManager
{
public:
    RedoUndoManager();

    void addHistory(History history);
    void clearHistory();

    QImage undo();
    QImage redo();

private:
    QQueue<History> histories;
    QQueue<History> redoQueue;
};

#endif // REDOUNDOMANAGER_H
