#ifndef SQUARESUMUI_H
#define SQUARESUMUI_H

#include <QMainWindow>
#include <QTime>
#include <QFile>
#include <QDebug>
#include <set>
#include <unordered_set>
#include <QException>
#include <generationWorker.h>
#include <findSquareSumWorker.h>

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

    void on_threadsFindSquares_clicked();

private:
    Ui::squareSumUI *ui;
    QSet<qint64> m_squaresSet; /** @brief Используется для хранения квадратов чисел.*/
    QSharedPointer<QSet<qint64>> m_squaresSetPtr;
    std::unordered_set<qint64> m_squaresStdSet;
    QVector<qint64> m_squaresVector; /** @brief Используется для хранения квадратов чисел.*/
    std::vector<qint64> m_squaresStdVector;
    QHash<qint64, qint64> m_squareSumsHash;
    QSharedPointer<QHash<qint64, qint64>> m_squareSumsHashPtr;
    qint64 m_inNumber;

    bool generateSequence();
    bool findSumSquares(qint64 n_inputNumber);

//    Для теста скорости чтения и закрузки из файла
//    bool saveSequenceToFile();
    //    bool loadSequence();
    bool generateSequenceVector();
    //bool findSumSquaresVector(qint64 n_inputNumber);
    //bool generateSequenceStdVector();
    //bool findSumSquaresStd(qint64 n_inputNumber);

    bool findSumSquaresPtr(qint64 n_inputNumber);
private slots:
    void on_generationFinished(int n_time);
    void on_findSquaresThreadFinished(int n_time);
};

#endif // SQUARESUMUI_H
