#include "serialport.h"
#include <QMessageBox>
#include <iostream>
#include <fstream>
#include <ios>
#include <cstring>

SerialPort* SerialPort::instance = nullptr;
quint64 SerialPort::offsetR = 0;
quint64 SerialPort::offsetW = 0;
char* SerialPort::buffer = new char[10000000]; /* 10^7 bytes */

SerialPort* SerialPort::getInstance(void)
{
    if(SerialPort::instance == nullptr){
        instance = new SerialPort();
        return instance;
    }
    else{
        return SerialPort::instance;
    }
}


SerialPort::SerialPort()
{
    m_timer1 = new QTimer();
    m_timer1->setInterval(0);

    m_timer2 = new QTimer();
    m_timer2->setInterval(0);

}

SerialPort::~SerialPort()
{
}


void SerialPort::connectInterface()
{
    if(!this->open(QIODevice::ReadWrite))
    {
        QString err("Can't open port with specified settings !");
        //DEBUG("Can't open port");
        emit status(err, 10000);
        emit error(err);
    }else{
        QString stat("Interface connected on port " + this->portName());
        emit status(stat, 10000);
        connect(m_timer1, SIGNAL(timeout()), this, SLOT(clearBuffer()));
        m_timer1->start();
    }
}

void SerialPort::calibrate()
{
    if(!this->isOpen())
    {
        QString err("Interface not connected !");
        emit status(err, 10000);
        emit error(err);
    }

    else
    {
       this->write("1\n");
    }
}

void SerialPort::start()
{
    QString stat = "Acquisition started";

    if(!this->isOpen())
    {
        QString err("Interface not connected !");
        emit status(err, 10000);
        emit error(err);
    }

    else
    {
        disconnect(m_timer1, SIGNAL(timeout()), this, SLOT(clearBuffer()));
        connect(m_timer1, SIGNAL(timeout()), this, SLOT(writeToFile()));
        connect(m_timer2, SIGNAL(timeout()), this, SLOT(readFromFile()));
        m_timer1->start();
        m_timer2->start();

        emit status(stat, 10000);
    }
}


void SerialPort::stop()
{
    QString stat = "Record has been stopped.";
    emit status(stat, 10000);
    m_timer1->stop();
    m_timer2->stop();
    this->close();
}


void SerialPort::resume()
{
    this->start();
}



void SerialPort::writeToFile(void)
{
    QString read  = this->readAll();
    std::memcpy((void*)(buffer+offsetW), (void*)read.toUtf8().constData(), strlen(read.toUtf8().constData()));
    offsetW += strlen(read.toUtf8().constData());
}

void SerialPort::readFromFile(void)
{
    static void *pointer = nullptr;

    char data[50];
    std::memset(data, '\0', 50);

    pointer = memchr(buffer+offsetR, '\n', offsetW);

    if(pointer != NULL){
        std::memcpy((void*)data, (void*)(buffer+offsetR), ((char*)pointer-(buffer+offsetR)));
        offsetR += (char*)pointer-(buffer+offsetR) + 1;

        QString tmp = QString::fromUtf8(data);
        //DEBUG("recv : " + tmp);

        float t,d,f;
        d = tmp.section(' ', 0, 0).toFloat();
        t = tmp.section(' ', 1, 1).toFloat();
        f = tmp.section(' ', 2, 2).toFloat();

        emit newData(t,d,f);
    }

}


void SerialPort::clearBuffer()
{
    this->readAll();
}

