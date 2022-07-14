#include "lc08_mtl.h"
#include <QFile>


LC08_MTL::LC08_MTL()
{

}

LC08_MTL::LC08_MTL(const QString & filePath) {
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        process_line(in, line);
    }
}

void LC08_MTL::process_line(QTextStream & in, QString & line) {
    if(line.indexOf("CORNER_UL_LAT_PRODUCT") != -1) {
        CORNER_UL.setX(line.mid(line.indexOf('=') + 2, line.length()).toFloat());
        line = in.readLine();
        CORNER_UL.setY(line.mid(line.indexOf('=') + 2, line.length()).toFloat());
        line = in.readLine();

        CORNER_UR.setX(line.mid(line.indexOf('=') + 2, line.length()).toFloat());
        line = in.readLine();
        CORNER_UR.setY(line.mid(line.indexOf('=') + 2, line.length()).toFloat());
        line = in.readLine();

        CORNER_LL.setX(line.mid(line.indexOf('=') + 2, line.length()).toFloat());
        line = in.readLine();
        CORNER_LL.setY(line.mid(line.indexOf('=') + 2, line.length()).toFloat());
        line = in.readLine();

        CORNER_LR.setX(line.mid(line.indexOf('=') + 2, line.length()).toFloat());
        line = in.readLine();
        CORNER_LR.setY(line.mid(line.indexOf('=') + 2, line.length()).toFloat());

        return;
    }

    if(line.indexOf("CORNER_UL_PROJECTION_X_PRODUCT") != -1) {
        CORNER_UL_PROJECTION.setX(line.mid(line.indexOf('=') + 2, line.length()).toFloat());
        line = in.readLine();
        CORNER_UL_PROJECTION.setY(line.mid(line.indexOf('=') + 2, line.length()).toFloat());
        line = in.readLine();

        CORNER_UR_PROJECTION.setX(line.mid(line.indexOf('=') + 2, line.length()).toFloat());
        line = in.readLine();
        CORNER_UR_PROJECTION.setY(line.mid(line.indexOf('=') + 2, line.length()).toFloat());
        line = in.readLine();

        CORNER_LL_PROJECTION.setX(line.mid(line.indexOf('=') + 2, line.length()).toFloat());
        line = in.readLine();
        CORNER_LL_PROJECTION.setY(line.mid(line.indexOf('=') + 2, line.length()).toFloat());
        line = in.readLine();

        CORNER_LR_PROJECTION.setX(line.mid(line.indexOf('=') + 2, line.length()).toFloat());
        line = in.readLine();
        CORNER_LR_PROJECTION.setY(line.mid(line.indexOf('=') + 2, line.length()).toFloat());

    }
}
