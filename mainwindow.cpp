#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "layer.h"
#include "layerdelegate.h"
#include "wstyle.h"
#include <QtWidgets>
#include <QCheckBox>
#include <QApplication>
#include <QtConcurrent/QtConcurrent>


Widget::Widget()
    : ui(new Ui::MainWindow),
      project(new WGProject),
      imageLabel(new QLabel),
      scrollArea(new QScrollArea),
      scaleFactor(1.0)

{
    ui->setupUi(this);

    QApplication::setStyle(new WStyle());
    QApplication::setPalette(QApplication::style()->standardPalette());
    menuBar()->setStyleSheet("QMenuBar{ background-color: #00535353; } QMenuBar::item:selected { background: #474747; } QMenuBar::item:pressed {  background: #383838; }"); // 在ubuntu windows上palette不对menubar起作用

    setAttribute(Qt::WA_TranslucentBackground);

    //  自定义右上角三个按钮
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    ThreeButton * threeBt = new ThreeButton(this);
    threeBt->show();
    menuBar()->setCornerWidget(threeBt, Qt::TopRightCorner);

    connect(threeBt, &ThreeButton::close_pressed, this, &QWidget::close);
    connect(threeBt, &ThreeButton::minimize_pressed, [=](){
        setWindowState(windowState()^Qt::WindowMinimized);
    });
    connect(threeBt, &ThreeButton::maximize_pressed, [=](){
        setWindowState(windowState()^Qt::WindowMaximized);
    });

    menuBar()->installEventFilter(this);

    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);


    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    scrollArea->setAlignment(Qt::AlignCenter);

    // 为右侧添加膨胀权重2，左为0
    QSizePolicy scrollPolicy = scrollArea->sizePolicy();
    scrollPolicy.setHorizontalStretch(2);
    scrollArea->setSizePolicy(scrollPolicy);

    QSplitter * splitterMain = new QSplitter(Qt::Horizontal, 0);
    ui->listView->setParent(splitterMain);
    scrollArea->setParent(splitterMain);
    setCentralWidget(splitterMain);

    imageLabel->setMouseTracking(true);
    scrollArea->setMouseTracking(true);
    splitterMain->setMouseTracking(true);
    setMouseTracking(true);

    imageLabel->setVisible(false);

    ui->listView->setModel(project->layersListModel);
    ui->listView->setItemDelegate(new LayerDelegate(ui->listView));
    ui->listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
//    ui->listView->setStyleSheet("background-color:rgb(77,77,77);");
//    ui->listView->setPalette(QPalette(QColor(77,77,77), QColor(77,77,77)));

    createActions();
    createMenus();

    maxLayerId = 0;

    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

void Widget::createActions() {
    openAct = new QAction(QIcon("://images/folder-open.png"), tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open a file"));
    connect(openAct, &QAction::triggered, this, &Widget::openFile);

    zoomInAct = new QAction(tr("Zoom &In"), this);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setStatusTip(tr("zoom in"));
    connect(zoomInAct, &QAction::triggered, this, &Widget::zoomIn);

    zoomOutAct = new QAction(tr("Zoom &Out"), this);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setStatusTip(tr("zoom out"));
    connect(zoomOutAct, &QAction::triggered, this, &Widget::zoomOut);

    copyCoordinateAct = new QAction(tr("经纬坐标"), this);
    connect(copyCoordinateAct, &QAction::triggered, this, &Widget::copyCoordinate);

    copyProjectionAct = new QAction(tr("投影坐标"), this);
    connect(copyProjectionAct, &QAction::triggered, this, &Widget::copyProjection);

    imageFilterAct = new QAction(QIcon("://images/algo1.png"), tr("滤波"), this);
    connect(imageFilterAct, &QAction::triggered, this, &Widget::openImageFilterWidget);

    ndviAct = new QAction(QIcon("://images/algo2.png"), tr("ndvi"), this);
    connect(ndviAct, &QAction::triggered, this, &Widget::openNdviWidget);

}

void Widget::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    viewMenu = menuBar()->addMenu(tr("&View"));

    fileMenu->addAction(openAct);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);

    copyCoordinateMenu = new QMenu(tr("复制坐标"), this);
    copyCoordinateMenu->addAction(copyCoordinateAct);
    copyCoordinateMenu->addAction(copyProjectionAct);

    fileToolBar = addToolBar(tr("file"));
    fileToolBar->addAction(openAct);

    algoToolBar = addToolBar(tr("algos"));
    algoToolBar->addAction(imageFilterAct);
    algoToolBar->addAction(ndviAct);
}

void Widget::openFile() {
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);

    //定义文件对话框标题
    fileDialog->setWindowTitle(tr("打开文件"));

    //设置默认文件路径
    fileDialog->setDirectory(".");

    //设置文件过滤器
    fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp *.tif *.tiff)"));

    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);

    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);


    if(fileDialog->exec()) {
        QStringList fileList = fileDialog->selectedFiles();
        // 读取的urls加入viewModel
        for(int i = 0; i < fileList.length(); i++) {
            addLayer(fileList[i]);
        }
        // 已有图层
        if(topLayerUrl != NULL) {
            return ;
        }
        topLayerUrl = project->getTopLayerUrl();
        QImageReader reader(topLayerUrl);
        reader.setAutoTransform(true);
        topImage = new QImage(reader.read());
        setImage(*topImage);

        // 读配置文件
        if(lc08_mtl == nullptr) {
            QDir dir = fileDialog->directory();
            QStringList mtl_filter;
            mtl_filter << "*MTL.txt";
            dir.setNameFilters(mtl_filter);
            if(dir.entryInfoList().length() > 0)
                lc08_mtl = new LC08_MTL(dir.entryInfoList()[0].filePath());
        }

    }

}

QStandardItemModel& Widget::getItemModel() {
    return *(this->project->layersListModel);
}

// 点图层的眼睛
void Widget::handleLayerVisibleChanged(int state) {
    // 获得发送者
    QCheckBox * box = dynamic_cast<QCheckBox*>(sender());
    int id = box->property("id").toInt();
    QStandardItem * item;

    // 找这个id的item
    for(int i = 0; i<project->layersListModel->rowCount(); i++) {
        // model里东西太tm难找了
        if(project->layersListModel->item(i)->data().value<Layer>().checkWidget->property("id").toInt() == id) {
            item = project->layersListModel->item(i);
        }
    }

    QVariant variant = item->data(Qt::UserRole+1);
    Layer layer = variant.value<Layer>();

    layer.setVisible(state);

    variant.setValue(layer);
    item->setData(variant);

    qDebug()<<project->getTopLayerUrl();
    if(topLayerUrl == project->getTopLayerUrl()) {
        return ;
    }
    // 异步重绘图片，先让checkbox图标变化
    QtConcurrent::run(this, &Widget::redrawTopImage);

}

// 重绘当前图片
void Widget::redrawTopImage() {
    topLayerUrl = project->getTopLayerUrl();

    QImageReader reader(topLayerUrl);
    reader.setAutoTransform(true);
    delete topImage;
    topImage = new QImage(reader.read());

    imageLabel->setPixmap(QPixmap::fromImage(*topImage));
}

void Widget::setImage(QImage & image)
{
    if (image.colorSpace().isValid())
        image.convertToColorSpace(QColorSpace::SRgb);
    imageLabel->setPixmap(QPixmap::fromImage(image));

    imageLabel->setVisible(true);

//    updateActions();

    imageLabel->adjustSize();
}

void Widget::zoomIn() {
    scaleImage(1.25);
}

void Widget::zoomOut() {
    scaleImage(0.8);
}

// 复制经纬坐标
void Widget::copyCoordinate() {
    QClipboard *clipboard = QApplication::clipboard();   // 获取系统剪贴板指针

    // 设置剪贴板内容
    clipboard->setText(
        QString::number(lc08_mtl->CORNER_UL.x() + (float)contextMenuPosition.x()/imageLabel->width()*(lc08_mtl->CORNER_UR.x() - lc08_mtl->CORNER_UL.x()), 'f', 3)
        + ", " +
        QString::number(lc08_mtl->CORNER_LL.y() + (float)(imageLabel->height() - contextMenuPosition.y())/imageLabel->height()*(lc08_mtl->CORNER_UL.y() - lc08_mtl->CORNER_LL.y()), 'f', 3)
    );
}

// 复制投影坐标
void Widget::copyProjection() {
    QClipboard *clipboard = QApplication::clipboard();   // 获取系统剪贴板指针

    // 设置剪贴板内容
    clipboard->setText(
        QString::number(lc08_mtl->CORNER_UL_PROJECTION.x() + (float)contextMenuPosition.x()/imageLabel->width()*(lc08_mtl->CORNER_UR_PROJECTION.x() - lc08_mtl->CORNER_UL_PROJECTION.x()), 'f', 0)
        + ", " +
        QString::number(lc08_mtl->CORNER_LL_PROJECTION.y() + (float)(imageLabel->height() - contextMenuPosition.y())/imageLabel->height()*(lc08_mtl->CORNER_UL_PROJECTION.y() - lc08_mtl->CORNER_LL_PROJECTION.y()), 'f', 0)
    );
}


void Widget::scaleImage(double factor) {
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());
}

// 打开滤波窗口
void Widget::openImageFilterWidget() {
    imageFilterWidget = new ImageFilterWidget(*topImage);
    connect(imageFilterWidget, &ImageFilterWidget::filterFinished, this, &Widget::handleFilterFinished);
    imageFilterWidget->show();
}

// 打开ndvi窗口
void Widget::openNdviWidget() {
    NdviWidget * ndviWidget = new NdviWidget(project->layersListModel);
    connect(ndviWidget, &NdviWidget::ndviFinished, this, &Widget::handleFilterFinished);    // 两个函数结束后的操作其实一样
    ndviWidget->show();
}

void Widget::handleFilterFinished(QString path) {
    addLayer(path);
    redrawTopImage();
}

void Widget::addLayer(QString path) {
    Layer l(path);

    // layer里存有checkbox, 定 property id 用于槽函数识别发送信号的box
    l.checkWidget->setParent(ui->listView);
    l.checkWidget->setProperty("id", maxLayerId++);
    connect(l.checkWidget, &QCheckBox::stateChanged, this, &Widget::handleLayerVisibleChanged);

    QStandardItem *item = new QStandardItem();
    QVariant variant = QVariant::fromValue(l);
    item->setData(variant, Qt::UserRole+1);
    item->setCheckable(true);
    project->layersListModel->insertRow(0,item);
}

void Widget::mousePressEvent(QMouseEvent *event)
{

}

void Widget::mouseMoveEvent(QMouseEvent *event) {

    if(imageLabel->isVisible() && lc08_mtl != nullptr) {
        // 鼠标在图像坐标系的位置
        QPoint pos = imageLabel->mapFromGlobal(event->globalPos());
        statusBar()->showMessage(
            QString::number(lc08_mtl->CORNER_UL_PROJECTION.x() + (float)pos.x()/imageLabel->width()*(lc08_mtl->CORNER_UR_PROJECTION.x() - lc08_mtl->CORNER_UL_PROJECTION.x()), 'f', 0)
            + ", " +
            QString::number(lc08_mtl->CORNER_LL_PROJECTION.y() + (float)(imageLabel->height() - pos.y())/imageLabel->height()*(lc08_mtl->CORNER_UL_PROJECTION.y() - lc08_mtl->CORNER_LL_PROJECTION.y()), 'f', 0)
        );
    }
}

bool Widget::eventFilter(QObject *watched, QEvent *event) {
    if (watched == menuBar())
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* mouse_event = dynamic_cast<QMouseEvent*>(event);
            if (mouse_event->button() == Qt::LeftButton)
            {
                dragPosition = mouse_event->globalPos() - frameGeometry().topLeft();
                return false;
            }
        }
        else if (event->type() == QEvent::MouseMove)
        {
            QMouseEvent* mouse_event = dynamic_cast<QMouseEvent*>(event);
            if (mouse_event->buttons() & Qt::LeftButton)
            {
                move(mouse_event->globalPos() - dragPosition);
                return false;
            }
        }


    }
    return false;
}

void Widget::contextMenuEvent(QContextMenuEvent *event) {

    // 在scrollarea内右键复制坐标
    contextMenuPosition = scrollArea->mapFromGlobal(event->globalPos());
    if(contextMenuPosition.x()>0 && contextMenuPosition.y()>0 &&contextMenuPosition.x()<scrollArea->width() && contextMenuPosition.y()<scrollArea->height()) {
        contextMenu = new QMenu(this);
        contextMenu->addMenu(copyCoordinateMenu);
        contextMenu->exec(QCursor::pos());
        event->accept();
        return ;
    }
}

void Widget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);	// 反锯齿;
    painter.setBrush(QBrush(QColor(83,83,83)));
    painter.setPen(Qt::transparent);
    QRect rect = this->rect();
    rect.setWidth(rect.width());
    rect.setHeight(rect.height() - 1);
    painter.drawRoundedRect(rect, 5, 5);

    QWidget::paintEvent(event);
}

Widget::~Widget()
{
    delete ui;
}


