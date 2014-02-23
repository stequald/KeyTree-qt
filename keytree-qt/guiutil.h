#ifndef GUIUTIL_H
#define GUIUTIL_H

#include <string>
#include <qstring.h>

namespace GUIUtil
{
std::string stdStringFromQString(QString str);
QString qStringFromSTDString(std::string str);
}

#endif // GUIUTIL_H
