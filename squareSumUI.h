#ifndef SQUARESUMUI_H
#define SQUARESUMUI_H

#include <QMainWindow>
#include <QSet>
#include <QTime>

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
    QList<int> m_list; // TODO: сделать sharedPointer'ом
    QSet<int> m_squaresSet; /** @brief Используется для хранения квадратов чисел.*/
    QHash<int, int> m_squareSumsHash;
    int m_inNumber;

    bool generateSequence(int n_count);
    bool generateSequence();
    bool findSumSquares(int n_inputNumber);

};

#endif // SQUARESUMUI_H
