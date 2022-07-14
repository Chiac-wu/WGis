#ifndef IMAGEFILTERWIDGET_H
#define IMAGEFILTERWIDGET_H

#include <QWidget>
#include <QImage>
#include "calculator.h"

namespace Ui {
class ImageFilterWidget;
}

class ImageFilterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageFilterWidget(QWidget *parent = nullptr);
    ImageFilterWidget(QImage & image);
    ~ImageFilterWidget();

private:
    Ui::ImageFilterWidget *ui;
    QWidget * parent;
    void loadStyleSheet();

    void kernelChanged(int index);
    void algoChanged(int index);
    void openExplorer();
    void handleOK();
    void handleCancel();
    void setProgressValue(int range);
    void setProgressRange(int beg, int end);
    void handleProgressFinished();
    QImage * image;
    float * kernel;
    Calculator calculator;

signals:
    void filterFinished(QString path);
};

#endif // IMAGEFILTERWIDGET_H
