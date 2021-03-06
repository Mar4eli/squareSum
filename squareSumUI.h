#ifndef SQUARESUMUI_H
#define SQUARESUMUI_H

#include <QMainWindow>
#include <QTime>
#include <QFile>
#include <QDebug>
#include <set>
#include <unordered_set>
#include <QException>
#include <findSquareSumWorker.h>
#include <QtConcurrent/QtConcurrent>


/* Постановка задачи
 * На вход поступает целое число (qint64). Необходимо найти все варианты разложения данного числа
 * на суммы квадратов двух других целых чисел (qint64). Вывести эти числа.
 *
 * Нельзя использовать продвинутые алгебраические и геометрические методы.
 * Программа должна иметь три варианта реализации:
 * 1. однопоточная.
 * 2. многопоточная.
 * 3. OpenCV (не уверен, что успею за этот срок).
 *
 * Программа должна содержать в себе документацию.
 */

namespace Ui {
class squareSumUI;
}

using namespace QtConcurrent;

class squareSumUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit squareSumUI(QWidget *parent = 0);
    ~squareSumUI();

private slots:
    void on_findSquares_clicked();
    void on_findSquaresVector_clicked();
    void on_threadsFindSquares_clicked();
    void on_findWorkerResultsReady(int n_time, QStringList n_resultsSL);
    void on_findSquaresComputation_clicked();

    void on_clearLog_clicked();

private:
    Ui::squareSumUI *ui;
    QSet<qint64> m_squaresSet; /** @brief Используется для хранения квадратов чисел.*/
    QVector<qint64> m_squaresVector; /** @brief Используется для хранения квадратов чисел.*/
    QHash<qint64, qint64> m_squareSumsHash; /** @brief Используется для хранения пар сумм квадратов.*/
    qint64 m_inNumber;

    bool generateSequence();
    bool generateSequenceVector();
    bool findSumSquares(qint64 n_inputNumber);
    void squareSumHashToLog();
    bool getInNumber();
    void blockButtons(bool n_block);
    bool findSquareComputation();
};

#endif // SQUARESUMUI_H
