#include "redoundomanager.h"

#define MAX_HISTORY 50

RedoUndoManager::RedoUndoManager()
{
    histories = QQueue<History>();
    redoQueue = QQueue<History>();
}

void RedoUndoManager::addHistory(History history)
{
    redoQueue.clear();

    histories.enqueue(history);

    if(histories.size() > MAX_HISTORY)
        histories.pop_back();
}

void RedoUndoManager::clearHistory()
{
    histories.clear();
    redoQueue.clear();
}

QImage RedoUndoManager::undo()
{
    History history = histories.dequeue();
    redoQueue.enqueue(history);

    return history.getPixelMap();
}

QImage RedoUndoManager::redo()
{
    History history = redoQueue.dequeue();
    histories.enqueue(history);

    return history.getPixelMap();
}
