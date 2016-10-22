#ifndef FINDSQUARESUMWORKER_H
#define FINDSQUARESUMWORKER_H

#include <QThread>
#include <QSharedPointer>
#include <QSet>
#include <QHash>
#include <QTime>

class findSquareSumWorker : public QThread
{
    Q_OBJECT

    void run() Q_DECL_OVERRIDE {
        this->findSquareSum();
    }
signals:
    void resultReady(int);

public:
    findSquareSumWorker(QSharedPointer<QHash<long long, long long>> n_findSquareSumPtr,QSharedPointer<QSet<long long>> n_sequenceSetPtr, long long n_inNumber, long long n_offset = 0, long long n_limit = 0);

public slots:
    QSharedPointer<QHash<qint64, qint64>> getResultHash();

private:
    QSharedPointer<QSet<qint64>> m_sequenceSetPtr;
    QSharedPointer<QHash<qint64,qint64>> m_findSquareSumPtr;
    qint64 m_inNumber;
    qint64 m_offset;
    qint64 m_limit;
    void findSquareSum();

};

#endif // FINDSQUARESUMWORKER_H
