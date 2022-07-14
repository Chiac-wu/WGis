#include "threebutton.h"
#include "ui_threebutton.h"
#include <QLayout>
#include <QDebug>

ThreeButton::ThreeButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ThreeButton)
{
    ui->setupUi(this);
    minimize = new QPushButton();
    maximize = new QPushButton();
    close = new QPushButton();

    QString minimize_qss = "QPushButton{"
                          "border-image:url(':/images/zuixiaohua.png');"
                          "background-position:center;"
                          "height:18;"
                          "width:18;"
                          "border:none;"
                          "}"
                           "QPushButton:pressed{"
                           "   background-color:rgb(71,71,71);"
                           "}";

    QString maximize_qss = "QPushButton{"
                           "border-image:url(':/images/zuidahua.png');"
                           "background-position:center;"
                           "height:18;"
                           "width:18;"
                           "border:none;"
                           "}"
                           "QPushButton:pressed{"
                           "   background-color:rgb(71,71,71);"
                           "}";

    QString close_qss = "QPushButton{"
                        "border-image:url(':/images/guanbi.png');"
                        "background-position:center;"
                        "height:18;"
                        "width:18;"
                        "border:none;"
                        "}"
                        "QPushButton:pressed{"
                        "   background-color:rgb(71,71,71);"
                        "}";
    minimize->setStyleSheet(minimize_qss);
    maximize->setStyleSheet(maximize_qss);
    close->setStyleSheet(close_qss);

    QRect size = QRect(0,0,104,18);
    this->setGeometry(size);

    QHBoxLayout * hlayout = new QHBoxLayout;
    hlayout->setGeometry(size);
    hlayout->setContentsMargins(0, 3, 6, 0);
    hlayout->setSpacing(10);
    hlayout->addWidget(minimize);
    hlayout->addWidget(maximize);
    hlayout->addWidget(close);

    setLayout(hlayout);

    connect(close, &QPushButton::clicked, [=](){
        emit close_pressed();
    });
    connect(minimize, &QPushButton::clicked, [=](){
        emit minimize_pressed();
    });
    connect(maximize, &QPushButton::clicked, [=](){
        emit maximize_pressed();
    });

    this->show();

}

ThreeButton::~ThreeButton()
{
    delete ui;
}
