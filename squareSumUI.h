#ifndef SQUARESUMUI_H
#define SQUARESUMUI_H

#include <QMainWindow>
#include <QSet>
#include <QTime>
#include <QFile>

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
    void on_genSquares_clicked();

private:
    Ui::squareSumUI *ui;
    QSet<int> m_squaresSet; /** @brief Используется для хранения квадратов чисел.*/
    QHash<int, int> m_squareSumsHash;
    int m_inNumber;

    bool generateSequence();
    bool findSumSquares(int n_inputNumber);

//    Для теста скорости чтения и закрузки из файла
//    bool saveSequenceToFile();
//    bool loadSequence();
};

#endif // SQUARESUMUI_H
