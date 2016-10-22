#ifndef SQUARESUMUI_H
#define SQUARESUMUI_H

#include <QMainWindow>

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

    bool generateSequence(int n_fromNumber, int n_count);
};

#endif // SQUARESUMUI_H
