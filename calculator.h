#ifndef ALGOS_H
#define ALGOS_H
#include <QImage>
#include <QWidget>


class Calculator:public QWidget
{
    Q_OBJECT

public:
    Calculator();
    QImage imageFilter(QImage & img, float * filter);
    QImage medianFilter(QImage & img);
    float ** ndvi(QImage & R, QImage & NIR);

    static float * lowFrequencyFilter();
    static float * embossNorthwest();
    static float * highFrequencyFilter();
    static float * edgeEnhancement();
    static float * laplacian4();
    static float * laplacian7();

signals:
    void progressValueChanged(int value);
    void progressRangeChanged(int beg, int end);
    void progressFinished();
};

#endif // ALGOS_H
