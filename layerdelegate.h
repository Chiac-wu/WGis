#ifndef LAYERDELEGATE_H
#define LAYERDELEGATE_H
#include <QStyledItemDelegate>
#include <QLabel>

class LayerDelegate : public QStyledItemDelegate
{
public:
    LayerDelegate(QWidget *);
    QLabel * labeltest;

    QWidget * listViewPtr;


//        QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

//        void setEditorData(QWidget *editor, const QModelIndex &index) const override;

//        void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // LAYERDELEGATE_H
