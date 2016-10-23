#ifndef FINDSQUARESUMWORKER_H
#define FINDSQUARESUMWORKER_H

#include <QThread>
#include <QHash>
#include <QTime>
#include <QtConcurrent/QtConcurrent>

class findSquareSumWorker : public QThread
{
    Q_OBJECT

    void run() Q_DECL_OVERRIDE {
        this->findSquareSum();
    }
signals:
    void resultReady(int,QStringList);

public:
    findSquareSumWorker(long long n_inNumber);

public slots:
    QHash<qint64, qint64> getResultHash();

private:
    QHash<qint64,qint64> m_findSquareSumPtr;
    qint64 m_inNumber;

    void findSquareSum();
};

#endif // FINDSQUARESUMWORKER_H
