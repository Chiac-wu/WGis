#ifndef LAYER_H
#define LAYER_H
#define EYE_STYLE_STR "QCheckBox::indicator:checked{image:url(:/images/eye_on.png);} QCheckBox::indicator:unchecked{image:url(:/images/eye_off.png);}"
#include <QString>
#include <QMetaType>
#include <QCheckBox>

class Layer
{
public:
    Layer();
    Layer(QString);
    ~Layer();
    bool getVisible();
    QString getName();
    QString getPath();
    void setVisible(bool);

    QCheckBox * checkWidget;
private:
    bool visible;
    QString name;
    QString path;

};

Q_DECLARE_METATYPE(Layer)

#endif // LAYER_H
