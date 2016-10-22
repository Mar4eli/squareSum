#include "generationWorker.h"

generationWorker::generationWorker(QSharedPointer<QSet<long long int>> n_set, long long int n_inNumber) : QThread()
{
    m_sharedSet = n_set;
    m_inNumber = n_inNumber;
}

bool generationWorker::generateSequence()
{
    QTime start = QTime::currentTime();
    qint64 number = 1;
    qint64 sum = number;
    qint64 border = 9223312036854775807; //уменьшенный INT64_MAX, чтобы ввести границу, когда начинаешь проверять на переполнение.
    m_sharedSet->clear();
    while(sum <= m_inNumber){
        m_sharedSet->insert(sum);
        number+=2;
        if(sum >= border)
        {
            //Проверка на переполнение qint64
            if(INT64_MAX-sum < number)
            {
                break;
            }
        }
        sum += number;
    }
    emit resultReady(start.elapsed());
    return true;
}
