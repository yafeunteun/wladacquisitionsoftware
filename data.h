#ifndef DATA_H
#define DATA_H

#include <QVector>
#include <QObject>
#include <set>

class Data : public QObject
{
    Q_OBJECT

    struct Curve{
        std::vector<float> distance;
        std::vector<float> time;
        std::vector<float> force;
        std::vector<float> speed;

        float peakP;
        float timeToPeakP;
        float averageP;
        float distanceP;
        float peakN;
        float timeToPeakN;
        float averageN;
        float distanceN;

    };

public:
    static Data* getInstance(void);
private:
    Data();
    ~Data(){}
    static Data *instance;

    int m_current = 0; /* current index */
    QVector<Curve> m_curves;
    bool m_reverse = false;
    bool m_rise = true;

public slots:
    void recordData(float, float, float);
    void save(QString& filename);
    void onReverse();
    void deleteEntry(std::set<int>);

signals:
    void newCurve(std::vector<float>);

};

#endif // DATA_H
