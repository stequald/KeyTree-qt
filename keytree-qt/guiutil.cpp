#include "guiutil.h"

namespace GUIUtil
{
std::string stdStringFromQString(QString str) {
    std::string s = str.toUtf8().constData();
    return s;
}

QString qStringFromSTDString(std::string str) {
    QString qstr = QString::fromStdString(str);
    return qstr;
}
}

