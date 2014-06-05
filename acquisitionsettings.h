#ifndef ACQUISITIONSETTINGS_H
#define ACQUISITIONSETTINGS_H

/*!
 * \file acquisitionsettings.h
 * \brief Graphical interface to configure data source.
 * \author yann feunteun
 * \version 0.1
 * \date 05/14/2014
 */


#include <QDialog>
#include <QPushButton>
#include <QGroupBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QVector>
#include <QVariant>
#include <QComboBox>
#include <map>
#include "qextserialport.h"

using std::map;


/*! \class AcquisitionSettings
   * \brief This class represents the graphical widget allowing to configure the data source.
   * The source can be : a remote peer, a remote server, a device connected to the serial port.
   */
class AcquisitionSettings : public QDialog
{
    Q_OBJECT
public:
    typedef enum{SERVER, SERIAL_PORT}Type_t;
    explicit AcquisitionSettings(QWidget *parent = 0);
private:
    void fillVector();
    void makeConnexions();
    QPushButton* m_ok;
    QPushButton* m_cancel;
    QVector<QVariant> m_settings;

    /* Serial port */
    void initMap(void);
    map<QString, QString> map_port;
    map<QString, BaudRateType> map_baud;
    map<QString, DataBitsType> map_dataBits;
    map<QString, FlowType> map_flowControl;
    map<QString, ParityType> map_parity;
    map<QString, StopBitsType> map_stopBits;

    QComboBox *box_port;
    QComboBox *box_baud;
    QComboBox *box_dataBits;
    QComboBox *box_flowControl;
    QComboBox *box_parity;
    QComboBox *box_stopBit;

    QCheckBox* m_reverse;


signals:
    void newConfiguration(AcquisitionSettings::Type_t type, const QVector<QVariant>& parameters);/*emitted in onNewConfiguration()*/

public slots:
    void onNewConfiguration(); /* Connected to Ok button */
    void onCancel();            /* Connected to Cancel button */


};

#endif // ACQUISITIONSETTINGS_H
