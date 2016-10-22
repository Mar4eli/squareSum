#ifndef FINDSQUARESUMWORKER_H
#define FINDSQUARESUMWORKER_H

#include <QThread>
#include <QSharedPointer>
#include <QSet>
#include <QHash>

class findSquareSumWorker : public QThread
{
public:
    findSquareSumWorker(QSharedPointer<QSet<qint64>> n_sequenceSetPtr, QSharedPointer<QHash<qint64,qint64>> n_findSquareSumPtr, qint64 n_inNumber);

private:
    QSharedPointer<QSet<qint64>> m_sequenceSetPtr;
    QSharedPointer<QHash<qint64,qint64>> m_findSquareSumPtr;
    qint64 n_inNumber;
};

#endif // FINDSQUARESUMWORKER_H
