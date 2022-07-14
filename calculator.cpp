#include "calculator.h"
#include <QDebug>
#include <QRgb>
#include <QDateTime>

Calculator::Calculator()
{

}

float* Calculator::lowFrequencyFilter() {
    return *(new float[3][3]{{1,1,1},{1,1,1},{1,1,1}});
}
float* Calculator::embossNorthwest() {
    return *(new float[3][3]{{0,0,1},{0,0,0},{-1,0,0}});
}
float* Calculator::highFrequencyFilter() {
    return *(new float[3][3]{{1,-2,1},{-2,5,-2},{1,-2,1}});
}
float* Calculator::edgeEnhancement() {
    return *(new float[3][3]{{-0.7,-1,-0.7},{-1,6.8,-1},{-0.7,-1,-0.7}});
}
float* Calculator::laplacian4() {
    return *(new float[3][3]{{0,-1,0},{-1,4,-1},{0,-1,0}});
}
float* Calculator::laplacian7() {
    return *(new float[3][3]{{1,1,1},{1,-7,1},{1,1,1}});
}
QImage Calculator::imageFilter(QImage &img, float * filter) {
    QImage rst(img.width(), img.height(), img.format());
    float rsum = 0, gsum = 0, bsum = 0;
    QRgb pixel;

    // 计时
//    unsigned time = QDateTime::currentDateTime().toTime_t();

    emit progressRangeChanged(0, img.width());

    // 发送信号的步长 最小为1
    int step = img.width()/100 > 1?img.width()/100:1;

    // 系数和 归一化
    int sum = *filter + *(filter+1) + *(filter+2) + *(filter+3) + *(filter+4) + *(filter+5) + *(filter+6) + *(filter+7) + *(filter+8);
    qDebug()<<"sum:"<<sum;

    for(int i = 1;i<img.width()-1; i++) {
        for(int j = 1;j<img.height()-1;j++) {
            pixel = img.pixel(i-1, j-1);

            rsum += qRed(pixel) * *filter;
            gsum += qGreen(pixel) * *filter;
            bsum += qBlue(pixel) * *filter;
            filter++;

            pixel = img.pixel(i-1, j);

            rsum += qRed(pixel) * *filter;
            gsum += qGreen(pixel) * *filter;
            bsum += qBlue(pixel) * *filter;
            filter++;

            pixel = img.pixel(i-1, j+1);

            rsum += qRed(pixel) * *filter;
            gsum += qGreen(pixel) * *filter;
            bsum += qBlue(pixel) * *filter;
            filter++;

            pixel = img.pixel(i, j-1);

            rsum += qRed(pixel) * *filter;
            gsum += qGreen(pixel) * *filter;
            bsum += qBlue(pixel) * *filter;
            filter++;

            pixel = img.pixel(i, j+1);

            rsum += qRed(pixel) * *filter;
            gsum += qGreen(pixel) * *filter;
            bsum += qBlue(pixel) * *filter;
            filter++;

            pixel = img.pixel(i+1, j-1);

            rsum += qRed(pixel) * *filter;
            gsum += qGreen(pixel) * *filter;
            bsum += qBlue(pixel) * *filter;
            filter++;

            pixel = img.pixel(i+1, j);

            rsum += qRed(pixel) * *filter;
            gsum += qGreen(pixel) * *filter;
            bsum += qBlue(pixel) * *filter;
            filter++;

            pixel = img.pixel(i+1, j+1);

            rsum += qRed(pixel) * *filter;
            gsum += qGreen(pixel) * *filter;
            bsum += qBlue(pixel) * *filter;
            filter++;

            pixel = img.pixel(i, j);

            rsum += qRed(pixel) * *filter;
            gsum += qGreen(pixel) * *filter;
            bsum += qBlue(pixel) * *filter;

            filter -= 8;

            if(sum) {
                rsum /= sum;
                gsum /= sum;
                bsum /= sum;
            } else {
//                rsum += qRed(pixel);
//                gsum += qGreen(pixel);
//                bsum += qBlue(pixel);

                rsum += 127;
                gsum += 127;
                bsum += 127;
            }

            rst.setPixel(i, j, qRgb((int)rsum, (int)gsum, (int)bsum));

            rsum = 0;
            bsum = 0;
            gsum = 0;
        }
        if(i%step == 0)
            emit progressValueChanged(i);
    }

    emit progressFinished();
//    qDebug()<<QDateTime::currentDateTime().toTime_t() - time;
    return rst;
}


QImage Calculator::medianFilter(QImage &img) {
    QImage rst(img.width(), img.height(), img.format());
    float rsum[9] = {0}, gsum[9] = {0}, bsum[9] = {0};
    QRgb pixel;

    // 计时
//    unsigned time = QDateTime::currentDateTime().toTime_t();

    emit progressRangeChanged(0, img.width());

    // 发送信号的步长 最小为1
    int step = img.width()/100 > 1?img.width()/100:1;

    for(int i = 1;i<img.width()-1; i++) {
        for(int j = 1;j<img.height()-1;j++) {
            pixel = img.pixel(i-1, j-1);

            rsum[0] = qRed(pixel);
            gsum[0] = qGreen(pixel);
            bsum[0] = qBlue(pixel);

            pixel = img.pixel(i-1, j);

            rsum[1] = qRed(pixel);
            gsum[1] = qGreen(pixel);
            bsum[1] = qBlue(pixel);

            pixel = img.pixel(i-1, j+1);

            rsum[2] = qRed(pixel);
            gsum[2] = qGreen(pixel);
            bsum[2] = qBlue(pixel);

            pixel = img.pixel(i, j-1);

            rsum[3] = qRed(pixel);
            gsum[3] = qGreen(pixel);
            bsum[3] = qBlue(pixel);

            pixel = img.pixel(i, j);

            rsum[4] = qRed(pixel);
            gsum[4] = qGreen(pixel);
            bsum[4] = qBlue(pixel);

            pixel = img.pixel(i, j+1);

            rsum[5] = qRed(pixel);
            gsum[5] = qGreen(pixel);
            bsum[5] = qBlue(pixel);

            pixel = img.pixel(i+1, j-1);

            rsum[6] = qRed(pixel);
            gsum[6] = qGreen(pixel);
            bsum[6] = qBlue(pixel);

            pixel = img.pixel(i+1, j);

            rsum[7] = qRed(pixel);
            gsum[7] = qGreen(pixel);
            bsum[7] = qBlue(pixel);

            pixel = img.pixel(i+1, j+1);

            rsum[8] = qRed(pixel);
            gsum[8] = qGreen(pixel);
            bsum[8] = qBlue(pixel);

            std::sort(rsum, rsum+8);
            std::sort(gsum, gsum+8);
            std::sort(bsum, bsum+8);

            rst.setPixel(i, j, qRgb((int)rsum[4], (int)gsum[4], (int)bsum[4]));
        }
        if(i%step == 0)
            emit progressValueChanged(i);
    }

    emit progressFinished();
//    qDebug()<<QDateTime::currentDateTime().toTime_t() - time;
    return rst;
}

float ** Calculator::ndvi(QImage & R, QImage & NIR) {


    QSize size = R.size();

    // 发送信号的步长 最小为1
    int step = size.height()/100 > 1?size.height()/100:1;

    emit progressRangeChanged(0, size.height());

    float ** rst = new float* [size.height()];
    for (int i = 0; i<size.height(); i++) {
        rst[i] = new float[size.width()];
    }

    float pixelNIR, pixelR;

    for (int i = 0; i<size.height(); i++) {
        for (int j = 0; j<size.width(); j++) {
            pixelNIR = qGray(NIR.pixel(j, i));
            pixelR = qGray(R.pixel(j, i));
            if(pixelR + pixelNIR == 0) {
                rst[i][j] = 1;
                continue;
            }
            rst[i][j] = (pixelNIR - pixelR)/(pixelNIR + pixelR);
        }

        if(i%step == 0)
            emit progressValueChanged(i);
    }

    return rst;
}
