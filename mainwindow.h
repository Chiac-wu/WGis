#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QImage>
#include "lc08_mtl.h"
#include "wgproject.h"
#include "imagefilterwidget.h"
#include "ndviwidget.h"
#include "threebutton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QAction;
class QMenu;
class QToolBar;
class QLabel;
class QScrollArea;
class QMouseEvent;
class QContextMenuEvent;
QT_END_NAMESPACE

class Widget : public QMainWindow
{
    Q_OBJECT

public:
    Widget();
    QStandardItemModel& getItemModel();
    ~Widget();

protected:
    void mousePressEvent(QMouseEvent * event) override;
    void mouseMoveEvent(QMouseEvent * event) override;
    void contextMenuEvent(QContextMenuEvent * event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void paintEvent(QPaintEvent * event) override;

private:    
    Ui::MainWindow *ui;
    void createActions();
    void createMenus();
    void openFile();
    void setImage(QImage &);
    void zoomIn();
    void zoomOut();
    void scaleImage(double);
    void handleLayerVisibleChanged(int);
    void redrawTopImage();
    void copyCoordinate();
    void copyProjection();
    void openImageFilterWidget();
    void openNdviWidget();
    void handleFilterFinished(QString path);
    void addLayer(QString path);

    WGProject * project;
    ImageFilterWidget * imageFilterWidget;

    QLabel * imageLabel;
    QMenu * fileMenu;
    QMenu * viewMenu;
    QMenu * contextMenu;
    QMenu * copyCoordinateMenu;
    QToolBar * fileToolBar;
    QToolBar * algoToolBar;

    // 打开文件
    QAction * openAct;

    // 放大缩小
    QAction * zoomInAct;
    QAction * zoomOutAct;

    // 复制坐标
    QAction * copyCoordinateAct;
    QAction * copyProjectionAct;

    // 算法acts
    QAction * imageFilterAct;
    QAction * ndviAct;

    QImage * topImage;
    QString topLayerUrl;
    QScrollArea * scrollArea;
    LC08_MTL * lc08_mtl = nullptr;
    QPoint contextMenuPosition;

    // 当前最大图层id, 图层唯一编号
    int maxLayerId;
    QPoint dragPosition;

    double scaleFactor = 1;
};
#endif // WIDGET_H
