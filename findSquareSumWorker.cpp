#include "findSquareSumWorker.h"

findSquareSumWorker::findSquareSumWorker(QSharedPointer<QHash<long long, long long>> n_findSquareSumPtr,QSharedPointer<QSet<long long>> n_sequenceSetPtr, long long n_inNumber, long long n_offset, long long n_limit) : QThread()
{
    m_sequenceSet = *n_sequenceSetPtr;
    m_findSquareSumPtr = n_findSquareSumPtr;
    m_inNumber = n_inNumber;
    m_offset = n_offset;
    m_limit = n_limit;
}

//void findSquareSumWorker::findSquareSum()
//{
//    QTime start = QTime::currentTime();
//    m_findSquareSumPtr->clear();
//    if(m_sequenceSetPtr->contains(m_inNumber))
//    {
//        m_findSquareSumPtr->insert(0, m_inNumber);
//    }

//    // Тесты показали, что проверка на наличие повторов не даёт заметного прироста скорости в диапазоне qint64.
//    //связано это с тем, что таких пар не так много, и число лишних проверок превышает число пар.
//    qint64 dif;
//    qint64 entryInSet;
//    foreach(entryInSet, *m_sequenceSetPtr)
//    {
//        dif = m_inNumber - entryInSet;
//        if(m_sequenceSetPtr->contains(dif))
//        {
//            m_findSquareSumPtr->insert(entryInSet,dif);
//        }
//    }
//    emit this->resultReady(start.elapsed());
//}

void findSquareSumWorker::findSquareSum()
{
    QTime start = QTime::currentTime();
    m_findSquareSumPtr->clear();
//    if(m_sequenceSetPtr->contains(m_inNumber))
//    {
//        m_findSquareSumPtr->insert(0, m_inNumber);
//    }

    // Тесты показали, что проверка на наличие повторов не даёт заметного прироста скорости в диапазоне qint64.
    //связано это с тем, что таких пар не так много, и число лишних проверок превышает число пар.
    qint64 dif;
    qint64 entryInSet;
    QSet<qint64>::const_iterator iter = m_sequenceSet.constBegin()+m_offset;
    QSet<qint64>::const_iterator stop = iter+m_limit;

    while (iter != stop) {
        dif = m_inNumber - *iter;
        if(m_sequenceSet.contains(dif))
        {
            m_findSquareSumPtr->insert(*iter,dif);
        }
        ++iter;
    }

    emit this->resultReady(start.elapsed());
}

QSharedPointer<QHash<qint64, qint64> > findSquareSumWorker::getResultHash()
{
    return m_findSquareSumPtr;
}
