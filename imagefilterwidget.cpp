#include "imagefilterwidget.h"
#include "ui_imagefilterwidget.h"
#include <QDebug>
#include <QFileDialog>
#include <QListView>
#include "wstyle.h"

ImageFilterWidget::ImageFilterWidget(QImage & image) :
    QWidget(),
    ui(new Ui::ImageFilterWidget)
{
    ui->setupUi(this);
    this->image = &image;

    loadStyleSheet();

    ui->algoComboBox->setView(new QListView());
    ui->kernelComboBox->setView(new QListView());
    ui->algoComboBox->addItem("线性滤波");
    ui->algoComboBox->addItem("中值滤波");

    ui->kernelComboBox->addItem("low frequency");
    ui->kernelComboBox->addItem("high frequency");
    ui->kernelComboBox->addItem("emboss northwest");
    ui->kernelComboBox->addItem("edge enhancement");
    ui->kernelComboBox->addItem("laplacian4");
    ui->kernelComboBox->addItem("laplacian7");
    ui->kernelComboBox->addItem("自定义滤波器");

    ui->num1->setReadOnly(true);
    ui->num2->setReadOnly(true);
    ui->num3->setReadOnly(true);
    ui->num4->setReadOnly(true);
    ui->num5->setReadOnly(true);
    ui->num6->setReadOnly(true);
    ui->num7->setReadOnly(true);
    ui->num8->setReadOnly(true);
    ui->num9->setReadOnly(true);

    // 初始化九格
    kernel = Calculator::lowFrequencyFilter();
    kernelChanged(0);

    setProgressValue(0);
    setProgressRange(0, 100);
    ui->progressBar->setEnabled(false);

    connect(ui->exploreButton, &QPushButton::clicked, this, &ImageFilterWidget::openExplorer);

    connect(ui->kernelComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ImageFilterWidget::kernelChanged);

    connect(ui->algoComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ImageFilterWidget::algoChanged);

    connect(ui->cancelButton, &QPushButton::clicked, this, &ImageFilterWidget::handleCancel);

    connect(ui->okButton, &QPushButton::clicked, this, &ImageFilterWidget::handleOK);

    connect(ui->outPathLineEdit, &QLineEdit::textChanged, [=](){
        if(ui->outPathLineEdit->text().length() == 0) {
            ui->okButton->setEnabled(false);
        } else {
            ui->okButton->setEnabled(true);
        }
    });

}

void ImageFilterWidget::loadStyleSheet() {
    QFile file(":/qss/main.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}

void ImageFilterWidget::kernelChanged(int index) {

    switch (index) {
    case 0:
        kernel = Calculator::lowFrequencyFilter();
        break;
    case 1:
        kernel = Calculator::highFrequencyFilter();
        break;
    case 2:
        kernel = Calculator::embossNorthwest();
        break;
    case 3:
        kernel = Calculator::edgeEnhancement();
        break;
    case 4:
        kernel = Calculator::laplacian4();
        break;
    case 5:
        kernel = Calculator::laplacian7();
        break;
    case 6:
        ui->num1->setReadOnly(false);
        ui->num2->setReadOnly(false);
        ui->num3->setReadOnly(false);
        ui->num4->setReadOnly(false);
        ui->num5->setReadOnly(false);
        ui->num6->setReadOnly(false);
        ui->num7->setReadOnly(false);
        ui->num8->setReadOnly(false);
        ui->num9->setReadOnly(false);
        break;
    default:
        return;
    }
    ui->num1->setText(QString::number(*kernel));
    ui->num2->setText(QString::number(*(kernel+1)));
    ui->num3->setText(QString::number(*(kernel+2)));
    ui->num4->setText(QString::number(*(kernel+3)));
    ui->num5->setText(QString::number(*(kernel+4)));
    ui->num6->setText(QString::number(*(kernel+5)));
    ui->num7->setText(QString::number(*(kernel+6)));
    ui->num8->setText(QString::number(*(kernel+7)));
    ui->num9->setText(QString::number(*(kernel+8)));
}

void ImageFilterWidget::algoChanged(int index) {
    switch (index) {
    // 线性滤波
    case 0:
        ui->kernelComboBox->setEnabled(true);
        ui->num1->setEnabled(true);
        ui->num2->setEnabled(true);
        ui->num3->setEnabled(true);
        ui->num4->setEnabled(true);
        ui->num5->setEnabled(true);
        ui->num6->setEnabled(true);
        ui->num7->setEnabled(true);
        ui->num8->setEnabled(true);
        ui->num9->setEnabled(true);
        break;
    // 中值滤波
    case 1:
        ui->kernelComboBox->setEnabled(false);
        ui->num1->setEnabled(false);
        ui->num2->setEnabled(false);
        ui->num3->setEnabled(false);
        ui->num4->setEnabled(false);
        ui->num5->setEnabled(false);
        ui->num6->setEnabled(false);
        ui->num7->setEnabled(false);
        ui->num8->setEnabled(false);
        ui->num9->setEnabled(false);
        break;
    }
}

void ImageFilterWidget::openExplorer() {
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Image"), "new-image.tif",
        tr("Tag Image File (*.tif);;Joint Photographic Experts Group (*.jpg);;Portable Network Graphics (*.png);;All Files (*)"));
    ui->outPathLineEdit->setText(fileName);
//    qDebug()<<fileName;
}

void ImageFilterWidget::setProgressValue(int value) {
    ui->progressBar->setValue(value);
}

void ImageFilterWidget::setProgressRange(int beg, int end) {
    ui->progressBar->setRange(beg, end);
}

void ImageFilterWidget::handleProgressFinished() {
    ui->progressBar->setValue(ui->progressBar->maximum());
    ui->progressBar->setEnabled(false);
    ui->okButton->setEnabled(true);
    ui->cancelButton->setEnabled(true);
}

void ImageFilterWidget::handleCancel() {
    this->close();
}

void ImageFilterWidget::handleOK() {
    ui->progressBar->setValue(0);
    ui->progressBar->setEnabled(true);
    ui->okButton->setEnabled(false);
    ui->cancelButton->setEnabled(false);

    if(ui->algoComboBox->currentIndex() == 0) {
        *kernel = ui->num1->text().toFloat();
        *(kernel+1) = ui->num2->text().toFloat();
        *(kernel+2) = ui->num3->text().toFloat();
        *(kernel+3) = ui->num4->text().toFloat();
        *(kernel+4) = ui->num5->text().toFloat();
        *(kernel+5) = ui->num6->text().toFloat();
        *(kernel+6) = ui->num7->text().toFloat();
        *(kernel+7) = ui->num8->text().toFloat();
        *(kernel+8) = ui->num9->text().toFloat();
    }


    connect(&calculator, &Calculator::progressValueChanged, this, &ImageFilterWidget::setProgressValue);
    connect(&calculator, &Calculator::progressRangeChanged, this, &ImageFilterWidget::setProgressRange);
//    connect(&calculator, &Calculator::progressFinished, this, &ImageFilterWidget::handleProgressFinished);

    QImage result;
    switch (ui->algoComboBox->currentIndex()) {
    case 0:
        result = calculator.imageFilter(*image, kernel);
        break;
    case 1:
        result = calculator.medianFilter(*image);
        break;
    }

    result.save(ui->outPathLineEdit->text());
    handleProgressFinished();

    emit filterFinished(ui->outPathLineEdit->text());
    this->close();
}

ImageFilterWidget::~ImageFilterWidget()
{
    delete ui;
}
