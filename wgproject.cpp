#include "wgproject.h"
#include "layer.h"

WGProject::WGProject():layersUrl()
{
    layersListModel = new QStandardItemModel();
}

QString WGProject::getTopLayerUrl() {
    for(int i=0; i<layersListModel->rowCount(); i++) {
        QStandardItem * item = layersListModel->item(i);
        if(item->data().value<Layer>().getVisible() == true) {
            return item->data().value<Layer>().getPath();
        }
    }
    return "";
}

void WGProject::setLayersUrl(const QStringList &list) {
    layersUrl = list;
}
