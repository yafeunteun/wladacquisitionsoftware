#include "data.h"
#include "serialport.h"
#include "debug.h"
#include <fstream>
#include <ios>



Data* Data::instance = nullptr;

Data* Data::getInstance(void)
{
    if(Data::instance == nullptr){
        instance = new Data();
        return instance;
    }
    else{
        return Data::instance;
    }
}

Data::Data()
{
    QObject::connect(SerialPort::getInstance(), SIGNAL(newData(float,float,float)), this, SLOT(recordData(float,float,float)));
    Curve c;
    m_curves.push_back(c);
}



void Data::recordData(float t, float dist, float f)
{
    static float last_distance = 0.;
    static float last_time = 0.;


    /* We consider only positive data */
    if(m_reverse)
        dist = - dist;
    if(dist < 0)
        return;

    /* End of the positive part of the movement */
    if((dist < last_distance) && (m_rise)){
        m_rise = false;

        /* function find velocity peak and time to peak*/
        /* function average */
        size_t i = std::max_element(m_curves[m_current].speed.begin(), m_curves[m_current].speed.end()) - m_curves[m_current].speed.begin();
        m_curves[m_current].peakP = m_curves[m_current].speed.at(i);
        m_curves[m_current].timeToPeakP = m_curves[m_current].time.at(i) - m_curves[m_current].time.at(0);
        m_curves[m_current].distanceP = m_curves[m_current].distance[m_curves[m_current].distance.size()-1];


        float delta_t = m_curves[m_current].time[m_curves[m_current].time.size()-1]-m_curves[m_current].time.at(0);
        float delta_d = m_curves[m_current].distance[m_curves[m_current].distance.size()-1]-m_curves[m_current].distance.at(0);
        m_curves[m_current].averageP = delta_d / delta_t;

        std::vector<float> vect;
        vect.push_back(m_curves[m_current].time[0]/1000000);
        vect.push_back((m_curves[m_current].time[m_curves[m_current].time.size()-1]-m_curves[m_current].time.at(0))/1000000);
        vect.push_back(m_curves[m_current].distanceP/10); /* conversion from mm to cm */
        vect.push_back(m_curves[m_current].averageP*1000);
        vect.push_back(m_curves[m_current].peakP);
        vect.push_back(m_curves[m_current].timeToPeakP/1000000);
        emit newCurve(vect);


    }

    /* End of negative part of the movement */
    else if((dist > last_distance) && (!m_rise)){
        Curve c;
        m_curves.push_back(c);
        ++m_current;

        m_rise = true;


    }else{  /* Not a new curve, we simply add the new data */
        m_curves[m_current].time.push_back(t);
        m_curves[m_current].distance.push_back(dist);
        m_curves[m_current].force.push_back(f);

        float deltaD = dist - last_distance;
        float deltaT = t - last_time;
        deltaD *= 0.001;
        deltaT /= 1000000;
        m_curves[m_current].speed.push_back(deltaD/deltaT);
    }

    last_distance = dist;
    last_time = t;
}



void Data::save(QString& filename)
{

    /* To keep the current state of the acquisition */

    Curve c;
    m_curves.push_back(c);
    ++m_current;
    m_rise = true;



    uint indexCurve = 0;

    std::ofstream fstream(filename.toStdString(), std::ios::out | std::ios::trunc);
    if (fstream){
        fstream << ";Time(µs);"
                << "Distance(mm);"
                << "Speed(mm/µs);"
                << "Force"
                <<std::endl;

        for(indexCurve = 0; indexCurve < m_curves.size() -1; ++indexCurve){ /* -1 because we've had another vector for the next curve */
            fstream << "Curve " + QString::number(indexCurve+1).toStdString();
            for(uint i = 0; i < m_curves[indexCurve].distance.size(); ++i){
                fstream << ";" + QString::number(m_curves[indexCurve].time[i], 'g', 6).toStdString() + ";"
                        <<QString::number(m_curves[indexCurve].distance[i], 'g', 6).toStdString() + ";"
                       <<QString::number(m_curves[indexCurve].speed[i], 'g', 6).toStdString() + ";"
                      <<QString::number(m_curves[indexCurve].force[i], 'g', 6).toStdString() + "\n";
            }

        }
        fstream.close();
    }
}


void Data::onReverse()
{
    m_reverse = !m_reverse;
}


void Data::deleteEntry(std::set<int> indexes)
{
    uint i = 0;
    for(int index : indexes){
        m_curves.removeAt(index-i);
        ++i;
    }

    m_current -= indexes.size();
}

