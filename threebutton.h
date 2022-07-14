#ifndef THREEBUTTON_H
#define THREEBUTTON_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class ThreeButton;
}

class ThreeButton : public QWidget
{
    Q_OBJECT

public:
    explicit ThreeButton(QWidget *parent = nullptr);
    ~ThreeButton();

private:
    Ui::ThreeButton *ui;
    QPushButton * minimize;
    QPushButton * maximize;
    QPushButton * close;

signals:
    void close_pressed();
    void minimize_pressed();
    void maximize_pressed();

};

#endif // THREEBUTTON_H
