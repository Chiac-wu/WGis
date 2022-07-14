#include "layer.h"
#include <QDebug>

Layer::Layer()
     :checkWidget(new QCheckBox),
      visible(true),
      name(""),
      path("")
{
}

Layer::Layer(QString path) :visible(true){
    this->path = path;
    name = path.mid(path.lastIndexOf("/") + 1, path.length());
    checkWidget = new QCheckBox();
    checkWidget->setStyleSheet(EYE_STYLE_STR);
    checkWidget->setChecked(visible);
}

Layer::~Layer() {

}

bool Layer::getVisible() {
    return visible;
}

QString Layer::getName() {
    return name;
}

QString Layer::getPath() {
    return path;
}

void Layer::setVisible(bool v) {
    visible = v;
}
