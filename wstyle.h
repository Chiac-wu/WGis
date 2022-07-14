#include <QProxyStyle>

#ifndef WSTYLE_H
#define WSTYLE_H


class WStyle : public QProxyStyle
{
public:
    WStyle();

    QPalette standardPalette() const override;

    int pixelMetric(PixelMetric metric, const QStyleOption *option = nullptr, const QWidget *widget = nullptr) const override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = nullptr) const override;


    mutable QPalette palette;
};

#endif // WSTYLE_H
