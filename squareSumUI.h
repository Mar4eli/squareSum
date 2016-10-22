#ifndef SQUARESUMUI_H
#define SQUARESUMUI_H

#include <QMainWindow>
#include <QSet>
#include <QTime>
#include <QFile>
#include <QDebug>
#include <set>

namespace Ui {
class squareSumUI;
}

class squareSumUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit squareSumUI(QWidget *parent = 0);
    ~squareSumUI();

private slots:
    void on_findSquares_clicked();

    void on_findSquaresVector_clicked();

private:
    Ui::squareSumUI *ui;
    QSet<qint64> m_squaresSet; /** @brief Используется для хранения квадратов чисел.*/
    std::set<qint64> m_squaresStdSet;
    QVector<qint64> m_squaresVector; /** @brief Используется для хранения квадратов чисел.*/
    std::vector<qint64> m_squaresStdVector;
    QHash<qint64, qint64> m_squareSumsHash;
    qint64 m_inNumber;

    bool generateSequence();
    bool findSumSquares(qint64 n_inputNumber);

//    Для теста скорости чтения и закрузки из файла
//    bool saveSequenceToFile();
    //    bool loadSequence();
    bool generateSequenceVector();
    //bool findSumSquaresVector(qint64 n_inputNumber);
    bool generateSequenceStdVector();
    bool findSumSquaresStd(qint64 n_inputNumber);
};

#endif // SQUARESUMUI_H
