#include "acquisitionsettingsproxy.h"

AcquisitionSettingsProxy* AcquisitionSettingsProxy::instance = nullptr;

AcquisitionSettingsProxy* AcquisitionSettingsProxy::getInstance(void)
{
    if(AcquisitionSettingsProxy::instance == nullptr){
        instance = new AcquisitionSettingsProxy();
        return instance;
    }
    else{
        return AcquisitionSettingsProxy::instance;
    }
}

void AcquisitionSettingsProxy::start()
{
    if(!m_configured)
    {
        QString err("You must configure the source before !");
        error(err);
        return;
    }

    if(m_source == AcquisitionSettings::SERIAL_PORT){
        SerialPort::getInstance()->start();
    }

}

void AcquisitionSettingsProxy::stop()
{
    if(m_source == AcquisitionSettings::SERIAL_PORT){
        SerialPort::getInstance()->stop();
    }

}

void AcquisitionSettingsProxy::resume()
{

}

void AcquisitionSettingsProxy::configure(AcquisitionSettings::Type_t type, QVector<QVariant> parameters)
{
    m_source = type;
    m_configured = true;

    if(m_source == AcquisitionSettings::SERIAL_PORT)
    {
        //DEBUG("Configuring serial port");
        SerialPort* instance = SerialPort::getInstance();
        instance->setPortName(parameters[0].toString());
        instance->setBaudRate(*(reinterpret_cast<BaudRateType*>(&parameters[1])));
        instance->setDataBits(*(reinterpret_cast<DataBitsType*>(&parameters[2])));
        instance->setFlowControl(*(reinterpret_cast<FlowType*>(&parameters[3])));
        instance->setParity(*(reinterpret_cast<ParityType*>(&parameters[4])));
        instance->setStopBits(*(reinterpret_cast<StopBitsType*>(&parameters[5])));


    }

}

void AcquisitionSettingsProxy::onError(QString& err)
{
    emit error(err);
}

