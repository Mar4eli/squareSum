#include "squareSumUI.h"
#include "ui_squareSumUI.h"

squareSumUI::squareSumUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::squareSumUI)
{
    ui->setupUi(this);
}
/* Постановка задачи
 * На вход поступает целое число (int). Необходимо найти все варианты разложения данного числа
 * на суммы квадратов двух других целых чисел (int).
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

void squareSumUI::on_genSquares_clicked()
{
    ui->logListWidget->clear();
    QString inText = ui->leIn->text();
    m_inNumber = ui->leIn->text().toInt();
    //Делаем проверку на переполнение INT в строке ввода и на то, что ввели текст.
    if(inText!= 0 && m_inNumber == 0)
    {
        ui->logListWidget->insertItem(0,"Введён текст или число превыщающее 2 147 483 647");
    }
    else
    {
        this->generateSequence();
        this->findSumSquares(m_inNumber);

        // TODO переделать на замену значений в таблице
        QHash<int, int>::const_iterator iter = m_squareSumsHash.constBegin();
        QHash<int,int>::const_iterator stop = m_squareSumsHash.constEnd();
        while (iter != stop) {
            ui->logListWidget->insertItem(0,QString::number(iter.key())+" "+QString::number(iter.value()));
            ++iter;
        }

    }
}

/* TODO на 32 битной системе проблем с скоростью нет, поэтому чтение из файла не нужно. Можно, конечно, проверить, но быстрее 10ms будет сложно уложиться*/
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
    int number = 1;
    int sum = number;
    int border = INT_MAX - 1000000;
    m_squaresSet.clear();
    m_squaresSet.insert(sum);
    do{
        number+=2;
        if(sum >= border)
        {
            //Проверка на переполнение int
            double tmp = (double)sum+(double)number;
            if(tmp > INT_MAX)
            {
                break;
            }
            else
            {
                sum += number;
            }
        }
        else
        {
            sum += number;
        }
        m_squaresSet.insert(sum);

    }while(sum < m_inNumber);
    ui->logListWidget->insertItem(0,"generation time=" + QString::number(start.elapsed()) + " ms");
    return true;
}

bool squareSumUI::findSumSquares(int n_inputNumber)
{
    QTime start = QTime::currentTime();
    m_squareSumsHash.clear();
    if(m_squaresSet.contains(n_inputNumber))
    {
        m_squareSumsHash.insert(0, n_inputNumber);
    }

    // Тесты показали, что проверка на наличие повторов не даёт заметного прироста скорости в диапазоне int.
    int entryInSet;
    int dif;
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

//Испытания показали, что на диапазоне INT чтение файла с квадратами прирост скорости не даёт.
//bool squareSumUI::saveSequenceToFile()
//{
//    QTime start = QTime::currentTime();
//    QFile *file = new QFile("sequence.txt");
//    if(file->open(QFile::WriteOnly))
//    {
//        //комбинируем строку для записи
//        int entry;
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
//            m_squaresSet.insert(entry.toInt());
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
