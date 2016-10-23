#include "squareSumUI.h"
#include "ui_squareSumUI.h"

const qint64 ITERATION_STEP=100000000;

//TODO сделать вариант на основе http://itnotesblog.ru/note.php?id=145
//Т.е. потоки не через qConcurrent, а ручные.
squareSumUI::squareSumUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::squareSumUI)
{
    ui->setupUi(this);
}

squareSumUI::~squareSumUI()
{
    delete ui;
}

/**
 * @brief squareSumUI::on_findSquares_clicked - обработчик слота нажатия на кнопку "find squares (single, qset)"
 * @details очищает контейнеры. Считывает данные из поля ввода. Запускает алгоритм поиска последовательности квадратов, поиска слагаемых и вывода в Log.
 * Алгоритм работает медленно, съедает гигабайты памяти, но меньше подвержен искажениям из-за округления. Здесь оставлен в ознакомительных целях.
 */
void squareSumUI::on_findSquares_clicked()
{
    ui->logListWidget->clear();
    m_squaresSet.clear();
    m_squareSumsHash.clear();

    //Делаем проверку на переполнение qint64 в строке ввода и на то, что ввели текст.
    if(this->getInNumber())
    {
        if(this->generateSequence())
        {
            if(this->findSumSquares(m_inNumber))
            {
                this->squareSumHashToLog();
            }
        }
    }
}

/**
 * @brief squareSumUI::squareSumHashToLog - выводит содержимое m_squareSumsHash в UI в лог.
 */
void squareSumUI::squareSumHashToLog()
{
    QHash<qint64, qint64>::const_iterator iter = m_squareSumsHash.constBegin();
    QHash<qint64,qint64>::const_iterator stop = m_squareSumsHash.constEnd();

    //делать одинарные InsertItem не рекомендуеца. Т.к. на каждое добавление вызывается метод перерисовки интерфейса.
    QStringList results;
    while (iter != stop) {
        results.append(QString::number(iter.key())+";"+QString::number(iter.value()));
        ++iter;
    }
    ui->logListWidget->insertItems(0,results);
}

/**
 * @brief squareSumUI::getInNumber - считывает значение из поля ввода.
 * @details Проверяет, не пустое ли поле ввода, введено ли число и можно ли его представить, как qint64.
 * @return true если ввели число, которое можно представить, как qint64.
 */
bool squareSumUI::getInNumber()
{
    QString inText = ui->leIn->text();
    if(inText.isEmpty())
    {
        ui->logListWidget->insertItem(0,"Введите, пожалуйста, число и повторите попытку.");
        return false;
    }
    m_inNumber = ui->leIn->text().toULongLong();

    //Делаем проверку на переполнение qint64 в строке ввода и на то, что ввели текст.
    if(inText != "0" && m_inNumber == 0)
    {
        ui->logListWidget->insertItem(0,"Введён текст или число превыщающее INT64_MAX");
        return false;
    }

    return true;
}

/**
 * @brief squareSumUI::generateSequence - генерирует последовательность чисел с шагом 2, начиная с 1.
 * @details Из школьного курса алгебры известно, что квадрат любого натурального числа N может быть
 * представлен, как сумма N натуральных чисел с шагом два.
 * Пример:
 * Дана последовательность: 1, 3, 5, 7...
 * 2^2 = 1+3 = 4;
 * 3^2 = 1 + 3 + 5 = 4 + 5 = 9.
 *
 * При этом, операции сложения выполняются быстрее, чем операции умножения/возведения в степень, т.к. они скрывают в себе множественные операции сложения.
 *
 * На числах больше int начинает съедать гигабайты оперативной памяти.
 * @return true - если прошло без существенных для нас ошибок. (Переполнение стека тут не учитывается).
 */
bool squareSumUI::generateSequence()
{
    QTime start = QTime::currentTime();
    qint64 sequence = 1;
    qint64 sum = sequence;
    qint64 border = 9223312036854775807; //уменьшенный INT64_MAX, чтобы ввести границу, когда начинаешь проверять на переполнение.
    m_squaresSet.clear();
    while(sum <= m_inNumber){
        m_squaresSet.insert(sum);
        sequence+=2;
        if(sum >= border)
        {
            //Проверка на переполнение qint64
            if(INT64_MAX-sum < sequence)
            {
                break;
            }
        }
        sum += sequence;
    }
    ui->logListWidget->insertItem(0,"generation time=" + QString::number(start.elapsed()) + " ms");
    ui->logListWidget->insertItem(0,"QSet size=" + QString::number(m_squaresSet.size()) + " ms");
    return true;
}

/**
 * @brief squareSumUI::findSumSquares - производит поиск пар слагаемых, в сумме дающих введенное число.
 * @details Проходит по QSet ранее посчитанных квадратов.
 * Забирает значение, вычитает из введённого числа, проверяет есть ли получившаяся разница в QSet.
 *
 * Опыты показали, что использование std::set и std::unordered_set приводят к снижению производительности.
 * Дело в том, что у них есть только метод find, который возвращает итератор. Здесь же у нас есть булевый метод contains.
 * @param n_inputNumber
 * @return
 */
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

/**
 * @brief squareSumUI::on_findSquaresVector_clicked -бработчик слота нажатия на кнопку "find squares (single, vector)".
 * @details очищает контейнеры. Считывает данные из поля ввода. Запускает алгоритм поиска последовательности квадратов, поиска слагаемых и вывода в Log.
 * Алгоритм работает медленно, съедает гигабайты памяти, но меньше подвержен искажениям из-за округления. Здесь оставлен в ознакомительных целях.
 *
 * Отличается от on_findSquares_clicked вызовом другого метода генерации. Оставлена такая структура для наглядности.
 */
void squareSumUI::on_findSquaresVector_clicked()
{
    try
    {
        if(this->getInNumber())
        {
            ui->logListWidget->clear();
            m_squaresVector.clear();
            m_squaresSet.clear();
            m_squareSumsHash.clear();

            if(this->generateSequenceVector())
            {
                if(this->findSumSquares(m_inNumber))
                {
                    this->squareSumHashToLog();
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
 *
 * На числах больше int начинает съедать гигабайты оперативной памяти.
 *
 * Использование QVector, даже с последующим QVector::toList()::toSet(), работает заметно быстрее, чем использование QSet.
 *
 * К сожалению, QVector падает с ошибкой на больших числах (в районе 20 000 000 000 000 000, при 17 000 000 000 000 000 работает). Сначала грешил на то, что у него ключи типа int, но падает на операции append и выполнении move.
 * Обработчик ошибки поставил на более высокий уровень, чтобы остановить всю БЛ.
 *
 * Использование std::vector обходит это. но из std::vector нельзя перейти в QSet. Использование std::set || srd::unordered_set убивает своим find весь прирост скорости за счёт вектора.
 * @param n_count - сколько элементов должно быть в последовательности.
 * @return true - если прошло без существенных для нас ошибок. (Переполнение стека тут не учитывается).
 */
bool squareSumUI::generateSequenceVector()
{
    QTime start = QTime::currentTime();
    qint64 number = 1;
    qint64 sum = number;
    qint64 border = 9223312036854775807; //уменьшенный INT64_MAX, чтобы ввести границу, когда начинаешь проверять на переполнение.
    m_squaresVector.clear();
    while(sum <= m_inNumber)
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
    };
    ui->logListWidget->insertItem(0,"vectorSize=" + QString::number(m_squaresVector.size()));
    m_squaresSet = m_squaresVector.toList().toSet();
    m_squaresVector.clear();
    ui->logListWidget->insertItem(0,"generation time=" + QString::number(start.elapsed()) + " ms");

    return true;
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
    qreal z;
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
                if(modf(qSqrt(j),&z) == 0)
                {
                    squareSumsHash.insert(x,j);
                }
            }
            else
            {
                //0*0=0 - тоже квадрат.
                squareSumsHash.insert(x,j);
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
 * @brief squareSumUI::on_threadsFindSquares_clicked - обработчик слота нажатия на кнопку "find squares (threads, computations)"
 * @details очищает контейнеры. Считывает данные из поля ввода. Рассчитывает, сколько потоков, с какими диапазонами нужно создать и запускает многопоточный поиск с использованием API высокого уровня QtConcurrent.
 * Алгоритм нетребователен к памяти, но процессор может загрузить полностью.
 */
void squareSumUI::on_threadsFindSquares_clicked()
{
    if(this->getInNumber())
    {
        ui->logListWidget->clear();
        m_squaresSet.clear();
        m_squareSumsHash.clear();

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

        QHash<qint64,qint64> total = mappedReduced(listOfPairs, findSquareSumConcur, reduce);
        qDebug()<<"jiraf" + QString::number(total.size())+" " + QString::number(start.elapsed())+" ms";
        QHash<qint64, qint64>::const_iterator iter = total.constBegin();
        QHash<qint64,qint64>::const_iterator stop = total.constEnd();

        QStringList results;
        while (iter != stop) {
            results.append(QString::number(iter.key())+";"+QString::number(iter.value()));
            ++iter;
        }
        ui->logListWidget->insertItems(0,results);
    }
}
