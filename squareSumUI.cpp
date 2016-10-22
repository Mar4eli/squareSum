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
    this->generateSequence(ui->leIn->text().toInt(),20);
    int entry;
    foreach(entry, m_list)
    {
        ui->logListWidget->insertItem(0, QString::number(entry));
    }

    ui->logListWidget->addItem(0);
}

/**
 * @brief squareSumUI::generateSequence - генерирует последовательность чисел с шагом 2.
 * @details Из школьного курса алгебры известно, что квадрат любого натурального числа может быть
 * представлен, как сумма N натуральных чисел с шагом два, где N - натуральное число, чей квадрат мы ищем.
 * Пример:
 * Дана последовательность: 1, 3, 5, 7. 2^2 = 1+3 = 4; 3^2 = 1 + 3 + 5 = 4 + 5 = 9.
 * @param n_fromNumber - нечетное число, с которого начинаем генерировать последовательность.
 * @param n_count - сколько элементов должно быть в последовательности.
 */
bool squareSumUI::generateSequence(int n_fromNumber, int n_count)
{
    // TODO: проверка на то, что число нечетное.
    if (n_fromNumber % 2 == 0)
    {
        ui->logListWidget->insertItem(0,"Стартовое число для генерации последовательности должно быть нечётным.");
        return false;
    }
    else
    {
        m_list.clear();
        m_list.append(n_fromNumber);
        for(int i=0; i <= n_count; ++i )
        {
            n_fromNumber+=2;
            m_list.append(n_fromNumber);
        }
        return true;
    }
    return true;
}
