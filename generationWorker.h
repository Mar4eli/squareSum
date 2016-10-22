#ifndef GENERATIONWORKER_H
#define GENERATIONWORKER_H

#include <QThread>
#include <QSharedPointer>
#include <QSet>
#include <QTime>
class generationWorker: public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE {
        this->generateSequence();
    }

signals:
    void resultReady(int);
public:
    generationWorker(QSharedPointer<QSet<long long> > n_set, long long n_inNumber);
private:
    bool generateSequence();
    QSharedPointer<QSet<qint64>> m_sharedSet;
    qint64 m_inNumber;
};

#endif // GENERATIONWORKER_H
