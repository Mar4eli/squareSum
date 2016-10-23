#include "squareSumUI.h"
#include "ui_squareSumUI.h"

squareSumUI::squareSumUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::squareSumUI)
{
    ui->setupUi(this);
    m_squaresSetPtr = QSharedPointer<QSet<qint64>>(new QSet<qint64>());
    m_squareSumsHashPtr = QSharedPointer<QHash<qint64,qint64>>(new QHash<qint64,qint64>());
}
// НА MSVC 64 работает быстрее
/* Постановка задачи
 * На вход поступает целое число (qint64). Необходимо найти все варианты разложения данного числа
 * на суммы квадратов двух других целых чисел (qint64).
 *
 * Нельзя использовать продвинутые алгебраические и геометрические методы.
 * Программа должна иметь три варианта реализации:
 * 1. однопоточная.
 * 2. многопоточная.
 * 3. OpenCV (не уверен, что успею за этот срок).
 *
 * Программа должна содержать в себе документацию и тесты.
 */

// TODO сделать под компилятор 64
squareSumUI::~squareSumUI()
{
    delete ui;
}

void squareSumUI::on_findSquares_clicked()
{
    ui->logListWidget->clear();
    QString inText = ui->leIn->text();
    m_inNumber = ui->leIn->text().toULongLong();
    m_squaresSet.clear();
    m_squareSumsHash.clear();

    //Делаем проверку на переполнение qint64 в строке ввода и на то, что ввели текст.
    if(inText != "0" && m_inNumber == 0)
    {
        ui->logListWidget->insertItem(0,"Введён текст или число превыщающее INT64_MAX");
    }
    else
    {
        this->generateSequence();
        this->findSumSquares(m_inNumber);

        // TODO переделать на замену значений в таблице
        QHash<qint64, qint64>::const_iterator iter = m_squareSumsHash.constBegin();
        QHash<qint64,qint64>::const_iterator stop = m_squareSumsHash.constEnd();
        while (iter != stop) {
            ui->logListWidget->insertItem(0,QString::number(iter.key())+" "+QString::number(iter.value()));
            ++iter;
        }
    }
}

/**
 * @brief squareSumUI::generateSequence - генерирует последовательность чисел с шагом 2, начиная с 1.
 * @details Из школьного курса алгебры известно, что квадрат любого натурального числа может быть
 * представлен, как сумма N натуральных чисел с шагом два, где N - натуральное число, чей квадрат мы ищем.
 * Пример:
 * Дана последовательность: 1, 3, 5, 7. 2^2 = 1+3 = 4; 3^2 = 1 + 3 + 5 = 4 + 5 = 9.
 *
 * При этом, операции сложения выполняются быстрее, чем операции умножения/возведения в степень, т.к. они скрывают в себе множественные операции сложения.
 * @param n_count - сколько элементов должно быть в последовательности.
 * @return true - если прошло без существенных для нас ошибок. (Переполнение стека тут не учитывается).
 */
bool squareSumUI::generateSequence()
{
    QTime start = QTime::currentTime();
    qint64 number = 1;
    qint64 sum = number;
    qint64 border = 9223312036854775807; //уменьшенный INT64_MAX, чтобы ввести границу, когда начинаешь проверять на переполнение.
    m_squaresSet.clear();
    while(sum <= m_inNumber){
        m_squaresSet.insert(sum);
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
    ui->logListWidget->insertItem(0,"generation time=" + QString::number(start.elapsed()) + " ms");
    ui->logListWidget->insertItem(0,"setSize=" + QString::number(m_squaresSet.size()) + " ms");
    return true;
}

bool squareSumUI::findSumSquares(qint64 n_inputNumber)
{
    QTime start = QTime::currentTime();
    m_squareSumsHash.clear();
    if(m_squaresSet.contains(n_inputNumber))
    {
        m_squareSumsHash.insert(0, n_inputNumber);
    }

    // Тесты показали, что проверка на наличие повторов не даёт заметного прироста скорости в диапазоне qint64.
    //связано это с тем, что таких пар не так много, и число лишних проверок превышает число пар.
    qint64 dif;
    qint64 entryInSet;
    foreach(entryInSet, m_squaresSet)
    {
        dif = n_inputNumber - entryInSet;
        if(m_squaresSet.contains(dif))
        {
            m_squareSumsHash.insert(entryInSet,dif);
        }
    }
    ui->logListWidget->insertItem(0,"find time="+QString::number(start.elapsed())+" ms");
    ui->logListWidget->insertItem(0,"find size="+QString::number(m_squareSumsHash.size())+" ms");

    return true;
}

void squareSumUI::on_findSquaresVector_clicked()
{
    try
    {
        ui->logListWidget->clear();
        QString inText = ui->leIn->text();
        m_inNumber = ui->leIn->text().toULongLong();
        m_squaresVector.clear();
        m_squaresSet.clear();
        m_squareSumsHash.clear();

        //Делаем проверку на переполнение qint64 в строке ввода и на то, что ввели текст.
        if(inText != "0" && m_inNumber == 0)
        {
            ui->logListWidget->insertItem(0,"Введён текст или число превыщающее INT64_MAX");
        }
        else
        {
            if(this->generateSequenceVector())
            {
                this->findSumSquares(m_inNumber);

                // TODO переделать на замену значений в таблице
                QHash<qint64, qint64>::const_iterator iter = m_squareSumsHash.constBegin();
                QHash<qint64,qint64>::const_iterator stop = m_squareSumsHash.constEnd();
                while (iter != stop) {
                    ui->logListWidget->insertItem(0,QString::number(iter.key())+" "+QString::number(iter.value()));
                    ++iter;
                }
            }
        }
    }
    catch(...)
    {
        QString tmp = "К сожалению, QVector падает при большом количестве записей qint64, причём падает на append и move. Времени досконально изучить вопрос не было. Использование std::vector и std::unordered_set || std::set показали худшие результаты.";
        ui->logListWidget->insertItem(0,tmp);
        m_squaresVector.clear();
    }
}

/**
 * @brief squareSumUI::generateSequence - генерирует последовательность чисел с шагом 2, начиная с 1.
 * @details Из школьного курса алгебры известно, что квадрат любого натурального числа может быть
 * представлен, как сумма N натуральных чисел с шагом два, где N - натуральное число, чей квадрат мы ищем.
 * Пример:
 * Дана последовательность: 1, 3, 5, 7. 2^2 = 1+3 = 4; 3^2 = 1 + 3 + 5 = 4 + 5 = 9.
 *
 * При этом, операции сложения выполняются быстрее, чем операции умножения/возведения в степень, т.к. они скрывают в себе множественные операции сложения.
 * @param n_count - сколько элементов должно быть в последовательности.
 * @return true - если прошло без существенных для нас ошибок. (Переполнение стека тут не учитывается).
 */

//Это даёт прирости в скорости, но QVector почему-то падает на числах в районе 20 000 000 000 000 000 (17 000 000 000 000 000 работает). Падает, как бы это странно не звучало, на append и операторе move.
//Не было времени детально в это влезать.
bool squareSumUI::generateSequenceVector()
{
    QTime start = QTime::currentTime();
    qint64 number = 1;
    qint64 sum = number;
    qint64 border = 9223312036854775807; //уменьшенный INT64_MAX, чтобы ввести границу, когда начинаешь проверять на переполнение.
    m_squaresVector.clear();
    qint64 intIndex=0;
    while(sum <= m_inNumber)
    {
        intIndex++;
        if(intIndex < INT_MAX)
        {
            m_squaresVector.append(sum);
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
        else
        {
            QString tmp = "К сожалению, QVector падает при большом количестве записей qint64, возхможно из-за использования int в качестве ключа. При превышении INT_MAX, происходит ошибка. Использование std::vector и std::unordered_set || std::set показали худшие результаты.";
            ui->logListWidget->insertItem(0,tmp);
            m_squaresVector.clear();
            return false;

        }
    };
    ui->logListWidget->insertItem(0,"vectorSize=" + QString::number(m_squaresVector.size()));
    m_squaresSet = m_squaresVector.toList().toSet();
    m_squaresVector.clear();
    ui->logListWidget->insertItem(0,"generation time=" + QString::number(start.elapsed()) + " ms");

    return true;
}

void textFunc(QString n_text)
{
    qDebug() << n_text << " " <<
                        "from" << QThread::currentThread();
}

void findSquareSumConcur(qint64 n_iterNumber,qint64 n_offset, qint64 n_limit, qint64 n_number)
{
    //меньше памяти, больше вычислений процессора
    QTime start = QTime::currentTime();
    QHash<qint64,qint64> m_squareSumsHash;
    qint64 max = n_offset + n_limit;
    qint64 j;
    if(n_offset == 0)
    {
        n_offset++;
    }
    qint64 i;
    for(i = n_offset; i <= max; ++i)
    {
        qint64 x=i*i;
        if(x <= n_number)
        {
            j = n_number - x;
            if(j != 0)
            {
                qint64 z = qSqrt(j);
                if(j % z == 0)
                {
                    m_squareSumsHash.insert(x,j);
                    //qDebug()<<"iterNumb="<<QString::number(n_iterNumber)<<" "<<QString::number(i)<<" and " <<QString::number(j)<<"from" << QThread::currentThread();
                    qDebug()<<QString::number(x)+";"+QString::number(j);
                }
            }
            else
            {
                m_squareSumsHash.insert(x,j);
                //qDebug()<<"iterNumb="<<QString::number(n_iterNumber)<<" "<<QString::number(i)<<" and " <<QString::number(j)<<"from" << QThread::currentThread();
                qDebug()<<QString::number(x)+";"+QString::number(j);;
            }
        }
        else
        {
            break;
        }

        //        qint64 x = qSqrt(i);
        //        if(i % x == 0)
        //        {
        //            j = n_number - i;
        //            qint64 z = qSqrt(j);
        //            if(j % z == 0)
        //            {
        //                m_squareSumsHash.insert(i,j);
        //            }
        //        }
    }
    //qDebug()<<"iterNumb="<<QString::number(n_iterNumber)<<" "<<QString::number(n_offset)<<" to " <<QString::number(max)<<"from" << QThread::currentThread();


}

//Использование std::vector обходит это. но из std::vector нельзя перейти в QSet. Использование std::set убивает своим find весь прирост скорости за счёт вектора
//тест на удаление значения из сета по итератору при проходе показал падение производительности. Логично, т.к. под erase скрывается много операций
//Испытания показали, что на диапазоне qint64 чтение файла с квадратами прирост скорости не даёт.

void squareSumUI::on_threadsFindSquares_clicked()
{
    ui->logListWidget->clear();
    QString inText = ui->leIn->text();
    m_inNumber = ui->leIn->text().toULongLong();
    m_squaresSet.clear();
    m_squareSumsHash.clear();

    //Делаем проверку на переполнение qint64 в строке ввода и на то, что ввели текст.
    if(inText != "0" && m_inNumber == 0)
    {
        ui->logListWidget->insertItem(0,"Введён текст или число превыщающее INT64_MAX");
    }
    else
    {
        QTime start = QTime::currentTime();
        QFuture<void> f1;
        qint64 iterations;
        qint64 maxNumber = qSqrt(INT64_MAX);
        if (m_inNumber < maxNumber)
        {
            iterations = m_inNumber/10000000;
        }
        else
        {
            iterations = maxNumber/10000000;
        }

        for(qint64 i=0; i <= iterations; ++i)
        {
          QtConcurrent::run(findSquareSumConcur,i,i*10000000,10000000,m_inNumber);
        }
        qint64 fullIter = iterations*10000000;
        QtConcurrent::run(findSquareSumConcur,iterations+1,fullIter,10000000,m_inNumber);



        qDebug()<<"slon "<<QString::number(start.elapsed());
    }
}


bool squareSumUI::findSumSquaresPtr(qint64 n_inputNumber)
{
    QTime start = QTime::currentTime();
    m_squareSumsHash.clear();
    if(m_squaresSetPtr->contains(n_inputNumber))
    {
        m_squareSumsHash.insert(0, n_inputNumber);
    }

    // Тесты показали, что проверка на наличие повторов не даёт заметного прироста скорости в диапазоне qint64.
    //связано это с тем, что таких пар не так много, и число лишних проверок превышает число пар.
    qint64 dif;
    qint64 entryInSet;
    foreach(entryInSet, *m_squaresSetPtr)
    {
        dif = n_inputNumber - entryInSet;
        if(m_squaresSetPtr->contains(dif))
        {
            m_squareSumsHash.insert(entryInSet,dif);
        }
    }
    ui->logListWidget->insertItem(0,"find time="+QString::number(start.elapsed())+" ms");

    return true;
}
