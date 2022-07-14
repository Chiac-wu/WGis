#include "wstyle.h"
#include <QDebug>
#include <QPainter>
#include <QStyleOption>
#include <QStyle>

WStyle::WStyle()
{

}

QPalette WStyle::standardPalette() const {

    palette.setColor(QPalette::Window, QColor(83,83,83));
    palette.setColor(QPalette::WindowText, QColor(27,27,27));
    palette.setColor(QPalette::Base, QColor(66,66,66));
    palette.setColor(QPalette::AlternateBase, QColor(255,255,255));
    palette.setColor(QPalette::ToolTipBase, QColor(27,27,27));
    palette.setColor(QPalette::ToolTipText, QColor(27,27,27));
    palette.setColor(QPalette::PlaceholderText, QColor(27,27,27));
    palette.setColor(QPalette::Text, QColor(240,240,240));
    palette.setColor(QPalette::Button, QColor(43,43,43));
    palette.setColor(QPalette::ButtonText, QColor(240,240,240));
    palette.setColor(QPalette::BrightText, QColor(255,255,255));
    palette.setColor(QPalette::NoRole, QColor(27,27,27));

    palette.setColor(QPalette::Light, QColor(27,27,27));
    palette.setColor(QPalette::Midlight, QColor(27,27,27));
    palette.setColor(QPalette::Dark, QColor(27,27,27));
    palette.setColor(QPalette::Mid, QColor(27,27,27));
    palette.setColor(QPalette::Shadow, QColor(27,27,27));

    palette.setColor(QPalette::Highlight, QColor(0, 120, 215));
    palette.setColor(QPalette::HighlightedText, QColor(255, 255, 242));
    return palette;
}

int WStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    switch (metric) {
    case PM_MenuBarPanelWidth:  // 这tm是menubar高度?? 确实改的是高度
        return 3;
    case PM_DockWidgetTitleBarButtonMargin:
        return 50;
    case PM_ScrollBarExtent:
        return QProxyStyle::pixelMetric(metric, option, widget) - 4;
    default:
        return QProxyStyle::pixelMetric(metric, option, widget);
    }
}

void WStyle::drawPrimitive(
        PrimitiveElement element,
        const QStyleOption *option,
        QPainter *painter,
        const QWidget *widget) const
{
//    qDebug()<<element;
    switch (element) {
    case PE_Frame:
        {

        }
        break;
    case PE_Widget:
        {
//            painter->fillRect(option->rect, QColor(255,0,0));
        }
        break;
    case PE_PanelButtonCommand:
        {

        }
        break;
    case PE_PanelStatusBar: // 底状态栏
        {
//            painter->fillRect(option->rect, QColor(255,0,0));
        }
        break;
    case PE_PanelMenu:  // menu 里按钮
        {
//            painter->fillRect(option->rect, QColor(255,0,0));
        }
        break;
    case PE_FrameMenu:  // menu 框架
        {
//            painter->save();

//            painter->setPen(QPen(QColor(255,0,0)));
//            painter->setBrush(QBrush(QColor(255,0,0)));
//            qDebug()<<option->rect;

//            painter->drawRect(option->rect);
//            painter->fillRect(option->rect, QColor(255,0,0));

//            painter->restore();
        }
        break;
//    case PE_PanelScrollAreaCorner:
//        {
//            painter->fillRect(option->rect, QColor(255,0,0));
//        }
//        break;


    default:
        {
            QProxyStyle::drawPrimitive(element, option, painter, widget);
        }

    }

}

void WStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{


    switch (element) {
    case CE_ToolBar:
        {
            QRect rect(option->rect.x(), option->rect.y(), option->rect.width(), option->rect.height());
            painter->fillRect(rect, QColor(40,40,40));
        }
        break;
    case CE_FocusFrame:
        {
            painter->fillRect(option->rect, QColor(255,0,0));
        }
        break;
    default:
        QProxyStyle::drawControl(element, option, painter, widget);
    }
}

void WStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    switch (control) {
    case CC_ScrollBar:
    {
        // 沟槽
        QRect drawRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarGroove, widget);
        painter->fillRect(drawRect, QColor(74,74,74));

        // 滑动条
        drawRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarSlider, widget);
        drawRect = QRect(drawRect.left()+4, drawRect.top()+4, drawRect.width()-8, drawRect.height()-8);
        painter->fillRect(drawRect, QColor(105,105,105));

        // 加箭头
        drawRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarAddLine, widget);
        painter->fillRect(drawRect, QColor(74,74,74));

        // 减箭头
        drawRect = subControlRect(CC_ScrollBar, option, SC_ScrollBarSubLine, widget);
        painter->fillRect(drawRect, QColor(74,74,74));

    }
        break;
    default:
        QProxyStyle::drawComplexControl(control, option, painter, widget);
    }
}
