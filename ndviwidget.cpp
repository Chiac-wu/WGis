#include "ndviwidget.h"
#include "ui_ndviwidget.h"
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include "layer.h"
#include "calculator.h"

NdviWidget::NdviWidget(QStandardItemModel * itemModel) :
    QWidget(),
    ui(new Ui::NdviWidget)
{
    ui->setupUi(this);
    loadStyleSheet();

    this->itemModel = itemModel;
    ui->progressBar->setValue(0);

    // 加载图层列表
    for (int i=0; i<itemModel->rowCount(); i++) {
        ui->RComboBox->addItem(itemModel->item(i)->data().value<Layer>().getName());
        ui->NIRComboBox->addItem(itemModel->item(i)->data().value<Layer>().getName());
    }

    ui->comboBox_3->addItem("RYG");

    connect(ui->exploreButton, &QPushButton::clicked, this, &NdviWidget::openExplorer);

    connect(ui->ok_button, &QPushButton::clicked, this, &NdviWidget::handleOkButton);

    connect(ui->cancel_button, &QPushButton::clicked, this, &NdviWidget::handleCancelButton);
}

void NdviWidget::loadStyleSheet() {
    QFile file(":/qss/main.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}

void NdviWidget::openExplorer() {
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Image"), "new-image.tif",
        tr("Tag Image File (*.tif);;Joint Photographic Experts Group (*.jpg);;Portable Network Graphics (*.png);;All Files (*)"));
    ui->outPathLineEdit->setText(fileName);
}

void NdviWidget::handleOkButton() {

    ui->ok_button->setEnabled(false);
    ui->cancel_button->setEnabled(false);

    QImage NIR = QImage(itemModel->item(ui->NIRComboBox->currentIndex())->data().value<Layer>().getPath());
    QImage R = QImage(itemModel->item(ui->RComboBox->currentIndex())->data().value<Layer>().getPath());

    // 两波段大小一致
    if(NIR.size() != R.size()) {
        QMessageBox::information(this, "wrong", "波段大小不一致", QMessageBox::Cancel);
        return ;
    }

    if(ui->outPathLineEdit->text().length() == 0) {
        QMessageBox::information(this, "wrong", "请选择保存路径", QMessageBox::Cancel);
        return ;
    }

    Calculator calculator;
    connect(&calculator, &Calculator::progressValueChanged, this, &NdviWidget::setProgressValue);
    connect(&calculator, &Calculator::progressRangeChanged, this, &NdviWidget::setProgressRange);

    float ** rst = calculator.ndvi(R, NIR);
    float nowNdvi;

    QImage result(R.size(), QImage::Format_RGB32);

    float low = 100, high = -100;
    // -1~1映射到伪彩色九个颜色上, 线性函数y = 4x + 4
    for (int i = 0; i<R.size().width(); i++) {
        for (int j = 0; j<R.size().height(); j++) {
            nowNdvi = rst[j][i];

            low = low<nowNdvi?low:nowNdvi;
            high = high>nowNdvi?high:nowNdvi;

            if(nowNdvi<-1) {
                result.setPixel(i, j, RYG[0]);
                continue;
            }
            if(nowNdvi>1) {
                result.setPixel(i, j, RYG[8]);
                continue;
            }
            result.setPixel(i, j, RYG[int(4*nowNdvi + 4)]);
//            result.setPixel(i, j, qRgb(255,0,0));

        }
    }

    qDebug()<<low<<", "<<high;
    result.save(ui->outPathLineEdit->text());

    handleProgressFinished();

    // 让主窗口收到图片
    emit ndviFinished(ui->outPathLineEdit->text());

    this->close();

}

void NdviWidget::setProgressValue(int value) {
    ui->progressBar->setValue(value);
}

void NdviWidget::setProgressRange(int beg, int end) {
    ui->progressBar->setRange(beg, end);
}

void NdviWidget::handleProgressFinished() {
    ui->progressBar->setValue(ui->progressBar->maximum());
    ui->progressBar->setEnabled(false);
    ui->ok_button->setEnabled(true);
    ui->cancel_button->setEnabled(true);
}

void NdviWidget::handleCancelButton() {
    this->close();
}
NdviWidget::~NdviWidget()
{
    delete ui;
}
