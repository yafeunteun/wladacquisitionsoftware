#ifndef DEBUG_H
#define DEBUG_H

#include <QDebug>

#ifndef QT_NO_DEBUG

#define FATAL_ERROR(msg) qFatal(qPrintable(msg))
#define CRITICAL_ERROR(msg) qCritical(qPrintable(msg))
#define WARNING(msg) qWarning(qPrintable(msg))
#define DEBUG(msg) qDebug(qPrintable(msg))

#endif

#endif
