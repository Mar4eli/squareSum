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

private:
    Ui::squareSumUI *ui;
};

#endif // SQUARESUMUI_H
