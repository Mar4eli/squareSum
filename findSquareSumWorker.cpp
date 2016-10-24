#include "findSquareSumWorker.h"

const qint64 ITERATION_STEP=100000000;

findSquareSumWorker::findSquareSumWorker(long long n_inNumber) : m_inNumber(n_inNumber), QThread()
{
}

QHash<qint64, qint64> findSquareSumWorker::getResultHash()
{
    return m_findSquareSumPtr;
}

/**
 * @brief findSquareSumConcur - потоко безопасный метод поиска слагаемых.
 * @details Данный метод работает с изолированным набором данных, использует мало памяти и при массовом запуске в потоках может занять весь процессор.
 *
 * В теле описан один твик производительности, позволивший увеличить её на 2,5% на тестовой машине.
 * @param n_offsetInNumberPair - парный объект, содержащий в себе: offset - начальное натуральное число для поиска квадратов, и inNumber - число, которое мы должны получить в сумме.
 * @return QHash<qint64,qint64>, содержащий все найденные пары значений. Практика показала, что длина такого хеша редко превышает 0 элементов, поэтому передача по указателю не дала бы заметного прироста производительности.
 */
QHash<qint64,qint64> findSquareSumConcur(QPair<qint64,qint64> n_offsetInNumberPair)
{
    //меньше памяти, больше вычислений процессора
    QHash<qint64,qint64> squareSumsHash;
    qint64 offset = n_offsetInNumberPair.first;
    qint64 inNumber = n_offsetInNumberPair.second;

    qint64 max = offset + ITERATION_STEP;

    if(offset == 0)
    {
        offset++;
    }
    qint64 i,j;
    qreal z,secondSquareRoot;
    //находим квадрат предыдущего числа из последовательности. Можно было сделать -1 до, но оставил для наглядности.
    qint64 x=(offset-1)*(offset-1);

    //квадрат любого натурального числа N может быть представлен,
    //как сумма N элементов последовательности натуральных чисел, начинающейся с 1, и с шагом 2.
    //1,3,5,7...
    qint64 sequence = 1 + (offset-1)*2;
    for(i = offset; i <= max; ++i)
    {
        //используя последовательность и сложение вместо возведения в степень, мы экономим такты.
        //так как процессор разбивает операцию умножения, на операции сложения.
        //5*5 = 5 + 5 + 5 + 5 + 5
        //на моём домашнем компьютере int64_max выполняется примерно 15100 мс при умножение
        // и 14700-14800 при сложении. экономия около 2,5%
        x+=sequence;
        sequence+=2;

        //проверяем, меньше ли полученное число искомого числа.
        if(x <= inNumber)
        {
            //находим второе слагаемое
            j = inNumber - x;
            if(j != 0)
            {
                //проверяем, является ли оно квадратом. т.е. пустой ли остаток от взятия корня.
                secondSquareRoot = qSqrt(j);
                if(modf(secondSquareRoot,&z) == 0)
                {
                    squareSumsHash.insert(i,secondSquareRoot);
                }
            }
            else
            {
                //0*0=0 - тоже квадрат.
                squareSumsHash.insert(i,j);
            }
        }
        else
        {
            break;
        }
    }

    return squareSumsHash;
}

/**
 * @brief reduce - функция объединения результатов работы потока поиска слагаемых.
 * @details Делает банальное объединение хешей.
 * @param[in] result - параметр, в который сохраняется суммарный результат потоков.
 * @param[in] w - результат из конкретного потока.
 */
void reduce(QHash<qint64,qint64> &result, const QHash<qint64,qint64> &w)
{
    result.unite(w);
}

/**
 * @brief findSquareSum - запускает многопоточный вариант поиска сумм квадратов.
 * @details Выделен в отдельный класс и поток, чтобы не замораживать интерфейс.
 * Рассчитывает, сколько потоков, с какими диапазонами нужно создать и запускает многопоточный поиск с использованием API высокого уровня QtConcurrent.
 * Алгоритм нетребователен к памяти, но процессор может загрузить полностью.
 */
void findSquareSumWorker::findSquareSum()
{
    qDebug()<<"hi";
    QTime start = QTime::currentTime();
    qint64 iterations,lastLimit;

    //Граничное условие. Дальше этого числа не стоит пытаться возвести его в квадрат, т.к. выпадешь с переполнением и задачу не решишь.
    //Из условия задачи следует, что максимальное значение квадрата одного из слагаемых меньше, либо равно заданному числу. Т.е. 20^2 = 20^2 + 0.
    //В таком случае, нам не надо исследовать числа большие, чем sqrt(заданное_число) c округлением в меньшую сторону.
    qint64 max_right_value = qSqrt(m_inNumber);
    iterations = max_right_value/ITERATION_STEP;
    lastLimit = max_right_value-(iterations*ITERATION_STEP);

    QList<QPair<qint64,qint64>> listOfPairs;
    for(qint64 i=0; i <= iterations; ++i)
    {
        listOfPairs.append(QPair<qint64,qint64>(i*ITERATION_STEP,m_inNumber));
    }
    listOfPairs.append(QPair<qint64,qint64>((iterations+1)*ITERATION_STEP,m_inNumber));

    QHash<qint64,qint64> total = QtConcurrent::mappedReduced(listOfPairs, findSquareSumConcur, reduce);
    //qDebug()<<"jiraf" + QString::number(total.size())+" " + QString::number(start.elapsed())+" ms";
    QHash<qint64, qint64>::const_iterator iter = total.constBegin();
    QHash<qint64,qint64>::const_iterator stop = total.constEnd();

    QStringList results;
    while (iter != stop) {
        results.append(QString::number(iter.key())+";"+QString::number(iter.value()));
        ++iter;
    }
    emit resultReady(start.elapsed(),results);
}
