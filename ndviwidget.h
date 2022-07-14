#ifndef NDVIWIDGET_H
#define NDVIWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class NdviWidget;
}

class NdviWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NdviWidget(QStandardItemModel *);
    ~NdviWidget();

private:
    Ui::NdviWidget *ui;
    QStandardItemModel * itemModel;

    const QRgb RYG[9] = {
        qRgb(215, 25, 28), qRgb(234, 100, 63), qRgb(253, 174, 97),
        qRgb(254, 215, 145), qRgb(255, 255, 192), qRgb(211, 236, 149),
        qRgb(166, 217, 106), qRgb(96, 184, 86), qRgb(26, 150, 65),
    };

    void loadStyleSheet();
    void openExplorer();
    void handleOkButton();
    void handleCancelButton();

    void setProgressValue(int range);
    void setProgressRange(int beg, int end);
    void handleProgressFinished();

signals:
    void ndviFinished(QString path);
};

#endif // NDVIWIDGET_H
