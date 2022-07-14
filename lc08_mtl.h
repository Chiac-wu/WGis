#ifndef LC08_MTL_H
#define LC08_MTL_H
#include <QPointF>
#include <QTextStream>

class LC08_MTL
{
public:
    LC08_MTL();
    LC08_MTL(const QString & filePath);

    // 四角经纬度
    QPointF CORNER_UL;
    QPointF CORNER_UR;
    QPointF CORNER_LL;
    QPointF CORNER_LR;

    // 四角投影坐标
    QPointF CORNER_UL_PROJECTION;
    QPointF CORNER_UR_PROJECTION;
    QPointF CORNER_LL_PROJECTION;
    QPointF CORNER_LR_PROJECTION;

private:
    void process_line(QTextStream & in, QString & line);
};

#endif // LC08_MTL_H
