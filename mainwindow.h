#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLCDNumber>
#include <QPushButton>
#include <QRadioButton>
#include <QTextEdit>
#include <QTableWidget>
#include <QLabel>
#include <QButtonGroup>
#include <QGroupBox>
#include <QAction>

#include "acquisitionsettings.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void configureWidgets();
    void createActions();
    void createToolBar();
    void createStatusBar();


    QLabel* m_status;

    QLCDNumber* m_nbReps;
    QLabel* m_percentageBest;
    QLabel* m_percentagePrevious;
    QLabel* m_value;
    QRadioButton *m_range;
    QRadioButton* m_meanSpeed;
    QRadioButton* m_maxSpeed;
    QRadioButton* m_maxSpeedTime;

    QButtonGroup *m_buttons;

    QTextEdit* m_notes;

    QTableWidget* m_table;
    QPushButton* m_deleteSelected;
    QPushButton* m_saveSelected;
    QPushButton* m_saveAll;
    QPushButton* m_exportAll;

    QAction* m_administration;
    QAction* m_connect;
    QAction* m_disconnect;
    QAction* m_start;
    QAction* m_stop;
    QAction* m_calibrate;

    uint m_nbCurve = 0;
    uint threshold = 80;

    const QString c_prefixFolderName = "data_";
    const QString c_rawDataFileName = "raw.csv";
    const QString c_notesDataFileName = "notes.txt";
    const QString c_statsDataFileName = "stats.csv";

    AcquisitionSettings* m_acquisitionSettingsWindow;

public slots:
    void onNewCurve(std::vector<float>);
    void save();
    /*tmp */
    void onNewConfiguration();
    void onError(QString&);
    void onStatus(QString&);

signals:
    void incReps(int);

};

#endif // MAINWINDOW_H
