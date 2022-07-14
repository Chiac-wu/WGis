#ifndef WGPROJECT_H
#define WGPROJECT_H

#include <QString>
#include <QStringList>
#include <QStandardItemModel>
/*
 * gis项目类
 * 存储一个项目的信息
 * 与ui解耦
 *
 */
class WGProject
{
public:
    WGProject();
    QString getTopLayerUrl();
    void setLayersUrl(const QStringList &);

    QStandardItemModel * layersListModel;

private:
    QStringList layersUrl;
};

#endif // WGPROJECT_H
