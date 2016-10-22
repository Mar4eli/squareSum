#include "squareSumUI.h"
#include "ui_squareSumUI.h"

squareSumUI::squareSumUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::squareSumUI)
{
    ui->setupUi(this);
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

    return true;
}

void squareSumUI::on_findSquaresVector_clicked()
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
        this->generateSequenceStdVector();
        this->findSumSquaresStd(m_inNumber);

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

//Это даёт прирости в скорости, но у QVector int в качестве ключа, соответственно, когда происходит переполнение int, вектор падает.
//Использование std::vector обходит это. но из std::vector нельзя перейти в QSet. Использование std::set убивает своим find весь прирост скорости за счёт вектора
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
    ui->logListWidget->insertItem(0,"vectorSize=" + QString::number(m_squaresVector.size()) + " ms");
    m_squaresSet = m_squaresVector.toList().toSet();
    m_squaresVector.clear();
    ui->logListWidget->insertItem(0,"generation time=" + QString::number(start.elapsed()) + " ms");

    return true;
}

bool squareSumUI::generateSequenceStdVector()
{
    QTime start = QTime::currentTime();
    qint64 number = 1;
    qint64 sum = number;
    qint64 border = 9223312036854775807; //уменьшенный INT64_MAX, чтобы ввести границу, когда начинаешь проверять на переполнение.
    m_squaresStdVector.clear();
    while(sum <= m_inNumber)
    {
        m_squaresStdVector.push_back(sum);
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
    ui->logListWidget->insertItem(0,"vectorSize=" + QString::number(m_squaresStdVector.size()) + " ms");
    std::set<qint64> squaresStdSet(m_squaresStdVector.begin(),m_squaresStdVector.end());
    m_squaresStdSet = squaresStdSet;
    m_squaresStdVector.clear();
    ui->logListWidget->insertItem(0,"generation time=" + QString::number(start.elapsed()) + " ms");

    return true;
}

bool squareSumUI::findSumSquaresStd(qint64 n_inputNumber)
{
    QTime start = QTime::currentTime();
    m_squareSumsHash.clear();
    if(m_squaresStdSet.find(n_inputNumber) != m_squaresStdSet.end())
    {
        m_squareSumsHash.insert(0, n_inputNumber);
    }

    // Тесты показали, что проверка на наличие повторов не даёт заметного прироста скорости в диапазоне qint64.
    //связано это с тем, что таких пар не так много, и число лишних проверок превышает число пар.
    qint64 dif;
    qint64 entryInSet;
    foreach(entryInSet, m_squaresStdSet)
    {
        dif = n_inputNumber - entryInSet;
        if(m_squaresStdSet.find(dif) != m_squaresStdSet.end())
        {
            m_squareSumsHash.insert(entryInSet,dif);
        }
    }
    ui->logListWidget->insertItem(0,"find time="+QString::number(start.elapsed())+" ms");

    return true;
}

//тест на удаление значения из сета по итератору при проходе показал падение производительности. Логично, т.к. под erase скрывается много операций
//QSet<qint64>::const_iterator iter = m_squaresSet.constBegin();
//QSet<qint64>::const_iterator stop = m_squaresSet.constEnd();
//while (iter != stop) {
//    dif = n_inputNumber - *iter;
//    if(m_squaresSet.contains(dif))
//    {
//        m_squareSumsHash.insert(*iter,dif);
//    }
//    iter = m_squaresSet.erase(iter);
//}

//Испытания показали, что на диапазоне qint64 чтение файла с квадратами прирост скорости не даёт.
//bool squareSumUI::saveSequenceToFile()
//{
//    QTime start = QTime::currentTime();
//    QFile *file = new QFile("sequence.txt");
//    if(file->open(QFile::WriteOnly))
//    {
//        //комбинируем строку для записи
//        qint64 entry;
//        QString resultStr;
//        foreach(entry,m_squaresSet)
//        {
//            resultStr=resultStr+QString::number(entry)+";";
//        }
//        file->write(resultStr.toUtf8());
//        file->close();
//        delete file;

//        ui->logListWidget->insertItem(0,"save time="+QString::number(start.elapsed())+" ms");
//        return true;
//    }
//    else
//    {
//        ui->logListWidget->insertItem(0,"Не могу создать файл для сохранения последовательности");
//        return false;
//    }
//}

//bool squareSumUI::loadSequence()
//{
//    QTime start = QTime::currentTime();
//    QFile *file = new QFile("sequence.txt");
//    if(file->open(QFile::ReadOnly))
//    {
//        QString fileStr = file->readAll();
//        QList<QString> listSquare = fileStr.split(';');
//        QString entry;
//        m_squaresSet.clear();
//        foreach(entry, listSquare)
//        {
//            m_squaresSet.insert(entry.toqint64());
//        }

//        file->close();
//        delete file;

//        ui->logListWidget->insertItem(0,"load time="+QString::number(start.elapsed())+" ms");
//        return true;
//    }
//    else
//    {
//        ui->logListWidget->insertItem(0,"Не могу создать файл для сохранения последовательности");
//        return false;
//    }
//}
