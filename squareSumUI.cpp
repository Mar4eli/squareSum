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


squareSumUI::~squareSumUI()
{
    delete ui;
}

void squareSumUI::on_genSquares_clicked()
{
    ui->logListWidget->clear();
    this->generateSequence(100);
    this->findSumSquares(ui->leIn->text().toInt());

    // TODO переделать на замену значений в таблице
    QHash<int, int>::const_iterator iter = m_squareSumsHash.constBegin();
    QHash<int,int>::const_iterator stop = m_squareSumsHash.constEnd();
    while (iter != stop) {
        ui->logListWidget->insertItem(0,QString::number(iter.key())+" "+QString::number(iter.value()));
        ++iter;
    }


    ui->logListWidget->addItem(0);
}

/**
 * @brief squareSumUI::generateSequence - генерирует последовательность чисел с шагом 2, начиная с 1.
 * @details Из школьного курса алгебры известно, что квадрат любого натурального числа может быть
 * представлен, как сумма N натуральных чисел с шагом два, где N - натуральное число, чей квадрат мы ищем.
 * Пример:
 * Дана последовательность: 1, 3, 5, 7. 2^2 = 1+3 = 4; 3^2 = 1 + 3 + 5 = 4 + 5 = 9.
 * @param n_count - сколько элементов должно быть в последовательности.
 */

/* TODO сохранение последовательности в файл и считывание при старте*/
bool squareSumUI::generateSequence(int n_count)
{
//    if (n_fromNumber % 2 == 0)
//    {
//        ui->logListWidget->insertItem(0,"Стартовое число для генерации последовательности должно быть нечётным.");
//        return false;
//    }
//    else
//    {
    int number = 1;
    int sum = number;
    m_squaresSet.clear();
    m_squaresSet.insert(sum);
    for(int i=0; i <= n_count; ++i )
    {
        number+=2;
        sum += number;
        m_squaresSet.insert(sum);
    }
    return true;
}

bool squareSumUI::findSumSquares(int n_inputNumber)
{
    m_squareSumsHash.clear();
    if(m_squaresSet.contains(n_inputNumber))
    {
        m_squareSumsHash.insert(0, n_inputNumber);
    }

    // TODO подумать, как избавиться от повторных пар. Проходя по set мы найдём и левую часть и правую.
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

    return true;
}
