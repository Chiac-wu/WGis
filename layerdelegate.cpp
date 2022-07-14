#include "layerdelegate.h"
#include "layer.h"
#include <QPainter>
#include <QCheckBox>
#include <QDebug>
#include <QStandardItemModel>

LayerDelegate::LayerDelegate(QWidget * ptr)
{
    listViewPtr = ptr;
}

void LayerDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();

    QVariant varient = index.data(Qt::UserRole+1);
    Layer layer = varient.value<Layer>();

    QRectF * nameRect = new QRectF();
    nameRect->setX(option.rect.x() + 40);
    nameRect->setY(option.rect.y() + (option.rect.height()-1 - painter->fontMetrics().height()) / 2);
    nameRect->setWidth(option.rect.width());
    nameRect->setHeight(painter->fontMetrics().height());

    QRect * eyeRect = new QRect();
    eyeRect->setX(option.rect.x() + 10);
    eyeRect->setY(option.rect.y() + (option.rect.height()-1 - 9) / 2);
    eyeRect->setWidth(16);
    eyeRect->setHeight(16);

    QRectF * highlightRect = new QRectF();
    highlightRect->setX(nameRect->x() - 5);
    highlightRect->setY(option.rect.y());
    highlightRect->setWidth(option.rect.width());
    highlightRect->setHeight(option.rect.height());

    // item背景色
    painter->fillRect(option.rect, QBrush(QColor(83,83,83)));
    if(option.state.testFlag(QStyle::State_Selected)) {
        painter->fillRect(*highlightRect, QBrush(QColor(107,107,107)));
    }

    // item间分割线
    painter->setPen(QColor(69,69,69));
    painter->drawLine(option.rect.x(), option.rect.y()+option.rect.height()-1, option.rect.x()+option.rect.width(),option.rect.y()+option.rect.height()-1);

    // 显示眼睛
    layer.checkWidget->setGeometry(*eyeRect);
    layer.checkWidget->show();

    // 图层名
    painter->setPen(QColor(240,240,240));
    painter->drawText(*nameRect, layer.getName());

    painter->restore();
}

QSize LayerDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    return QSize(400,51);
}
