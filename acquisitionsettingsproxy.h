#ifndef ACQUISITIONSETTINGSPROXY_H
#define ACQUISITIONSETTINGSPROXY_H

#include <QObject>
#include "serialport.h"
#include "acquisitionsettings.h"


class AcquisitionSettingsProxy : public QObject
{
    Q_OBJECT

public:
    static AcquisitionSettingsProxy* getInstance(void);
private:
    AcquisitionSettingsProxy(){}
    ~AcquisitionSettingsProxy(){}
    static AcquisitionSettingsProxy *instance;
    bool m_configured = false;
    AcquisitionSettings::Type_t m_source;
public slots:
    void start();
    void stop();
    void resume();
    void configure(AcquisitionSettings::Type_t type, QVector<QVariant> parameters);
    void onError(QString& err);
signals:
    void error(QString&);


};

#endif // ACQUISITIONSETTINGSPROXY_H
