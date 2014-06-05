#include "acquisitionsettings.h"
#include "acquisitionsettingsproxy.h"
#include "data.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>

/*!
    *  \brief Constructor
    *
    *  Constructor of AcquisitionSettings class
    *
    *  \param parent : Constructs a widget which is a child of parent.
    */
AcquisitionSettings::AcquisitionSettings(QWidget *parent) :
    QDialog(parent)
{
    this->setModal(true);
    this->setFixedSize(this->minimumSize());

    m_ok = new QPushButton("Ok");
    m_cancel = new QPushButton("Cancel");
    m_reverse = new QCheckBox;
    m_reverse->setChecked(false);

    /*SERIAL PORT */

    QFormLayout* serialLayout = new QFormLayout();
    initMap();

    box_port = new QComboBox;
    box_port->addItem("COM1");
    box_port->addItem("COM2");
    box_port->addItem("COM3");
    box_port->addItem("COM4");
    box_port->addItem("COM5");
    box_port->addItem("COM6");
    box_port->addItem("COM7");
    box_port->addItem("COM8");
    box_port->addItem("COM9");
    box_port->setCurrentText("COM9");

    box_baud = new QComboBox;
    box_baud->addItem("BAUD9600");
    box_baud->addItem("BAUD19200");
    box_baud->addItem("BAUD115200");
    box_baud->setCurrentText("BAUD115200");

    box_dataBits = new QComboBox;
    box_dataBits->addItem("DATA_8");
    box_dataBits->addItem("DATA_7");
    box_dataBits->addItem("DATA_6");
    box_dataBits->addItem("DATA_5");

    box_flowControl = new QComboBox();
    box_flowControl->addItem("FLOW_OFF");           // No flow control
    box_flowControl->addItem("FLOW_HARDWARE");      // Hardware (RTS/CTS) flow control
    box_flowControl->addItem("FLOW_XONXOFF");       // Software (XON/XOFF) flow control

    box_parity = new QComboBox();
    box_parity->addItem("PAR_NONE");
    box_parity->addItem("PAR_SPACE");
    box_parity->addItem("PAR_EVEN");
    box_parity->addItem("PAR_ODD");

    box_stopBit = new QComboBox();
    box_stopBit->addItem("STOP_1");
    box_stopBit->addItem("STOP_2");


    serialLayout->addRow("Port name : ", box_port);
    serialLayout->addRow("Baud rate", box_baud);
    serialLayout->addRow("Number of data bits", box_dataBits);
    serialLayout->addRow("Flow control", box_flowControl);
    serialLayout->addRow("Parity", box_parity);
    serialLayout->addRow("Number of stop bits", box_stopBit);
    serialLayout->addRow("Reverse", m_reverse);


    QHBoxLayout *buttons = new QHBoxLayout();
    buttons->addWidget(m_ok);
    buttons->addWidget(m_cancel);


    QVBoxLayout *global = new QVBoxLayout();
    global->addLayout(serialLayout);
    global->addLayout(buttons);

    this->setLayout(global);
    this->makeConnexions();

}

/*!
    *  \brief Initialize drop down menus with parameters.
    */
void AcquisitionSettings::initMap()
{
    map_port["COM1"] = "COM1";
    map_port["COM2"] = "COM2";
    map_port["COM3"] = "COM3";
    map_port["COM4"] = "COM4";
    map_port["COM5"] = "COM5";
    map_port["COM6"] = "COM6";
    map_port["COM7"] = "COM7";
    map_port["COM8"] = "COM8";
    map_port["COM9"] = "COM9";

    map_baud["BAUD9600"] = BAUD9600;
    map_baud["BAUD19200"] = BAUD19200;
    map_baud["BAUD115200"] = BAUD115200;

    map_dataBits["DATA_5"] = DATA_5;
    map_dataBits["DATA_6"] = DATA_6;
    map_dataBits["DATA_7"] = DATA_7;
    map_dataBits["DATA_8"] = DATA_8;

    map_flowControl["FLOW_OFF"] = FLOW_OFF;
    map_flowControl["FLOW_HARDWARE"] = FLOW_HARDWARE;
    map_flowControl["FLOW_XONXOFF"] = FLOW_XONXOFF;

    map_parity["PAR_SPACE"] = PAR_SPACE;
    map_parity["PAR_NONE"] = PAR_NONE;
    map_parity["PAR_EVEN"] = PAR_EVEN;
    map_parity["PAR_ODD"] = PAR_ODD;

    map_stopBits["STOP_1"] = STOP_1;
    map_stopBits["STOP_2"] = STOP_2;
}

/*!
    *  \brief Make connexions between the buttons of the widget and the actions associated.
    */
void AcquisitionSettings::makeConnexions()
{
    QObject::connect(m_ok, SIGNAL(clicked()), this, SLOT(onNewConfiguration()));
    QObject::connect(m_cancel, SIGNAL(clicked()), this, SLOT(onCancel()));
    QObject::connect(this, SIGNAL(newConfiguration(AcquisitionSettings::Type_t,QVector<QVariant>)), AcquisitionSettingsProxy::getInstance(), SLOT(configure(AcquisitionSettings::Type_t,QVector<QVariant>)));
    QObject::connect(m_reverse, SIGNAL(toggled(bool)), Data::getInstance(), SLOT(onReverse()));
}

/*!
    *  \brief Performed each new configuration.
    *
    *  Send information about the new configuration by emitting a signal.
    *
    */
void AcquisitionSettings::onNewConfiguration()
{
    this->fillVector();
    emit newConfiguration(AcquisitionSettings::SERIAL_PORT, m_settings);
    this->close();
}

/*!
    *  \brief Performed when button cancel is clicked.
    *
    */
void AcquisitionSettings::onCancel()
{
    this->close();
}

/*!
    *  \brief Fills an array with the current configuration.
    */
void AcquisitionSettings::fillVector()
{
    /* reset m_setings content */
    m_settings.clear();


    QString name = map_port.find(box_port->currentText())->second;
    BaudRateType baudRate = map_baud.find(box_baud->currentText())->second;
    DataBitsType dataBits = map_dataBits.find(box_dataBits->currentText())->second;
    FlowType flow = map_flowControl.find(box_flowControl->currentText())->second;
    ParityType parity = map_parity.find(box_parity->currentText())->second;
    StopBitsType stopBits = map_stopBits.find(box_stopBit->currentText())->second;

    m_settings.append(name);
    m_settings.append(baudRate);
    m_settings.append(dataBits);
    m_settings.append(flow);
    m_settings.append(parity);
    m_settings.append(stopBits);

}



