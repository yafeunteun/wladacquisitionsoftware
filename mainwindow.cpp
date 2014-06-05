#include "mainwindow.h"
#include "data.h"
#include "debug.h"
#include "serialport.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QToolBar>
#include <QFileDialog>
#include <QFont>
#include <QDateTime>
#include <iostream>
#include <fstream>
#include <QDir>
#include <QStatusBar>
#include <QMessageBox>
#include <set>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_acquisitionSettingsWindow = new AcquisitionSettings();
    m_acquisitionSettingsWindow->setVisible(false);

    m_status = new QLabel;
    m_buttons = new QButtonGroup;
    m_nbReps = new QLCDNumber();
    m_percentageBest = new QLabel;
    m_percentagePrevious = new QLabel;
    m_value = new QLabel;
    m_range = new QRadioButton("Range (cm)");
    m_meanSpeed = new QRadioButton("Mean Speed (m/s)");
    m_maxSpeed = new QRadioButton("Max Speed (m/s)");
    m_maxSpeedTime = new QRadioButton("Max Speed Time (s)");
    m_notes = new QTextEdit;
    m_table = new QTableWidget(0, 7);
    m_deleteSelected = new QPushButton("delete selected");
    m_saveSelected = new QPushButton("save selected");
    m_saveAll = new QPushButton("save all");
    m_exportAll = new QPushButton("export all");

    configureWidgets();
    createActions();
    createToolBar();
    createStatusBar();


    QFont font1;
    font1.setPointSize(10);
    QLabel *label1 = new QLabel("<strong>Reps</strong>");
    label1->setFont(font1);
    QLabel *label2 = new QLabel("of the previous rep");
    QLabel *label2_bis = new QLabel("of the best rep");
    QLabel *label3 = new QLabel("<strong>Notes</strong>");

    QVBoxLayout *layout1 = new QVBoxLayout;
    layout1->addWidget(label1);
    layout1->addWidget(m_nbReps);

    QVBoxLayout *layout2 = new QVBoxLayout();
    QHBoxLayout *layout2_bis = new QHBoxLayout;
    QHBoxLayout *layout2_ter = new QHBoxLayout;
    layout2->addWidget(m_value);
    layout2_bis->addWidget(m_percentagePrevious);
    layout2_bis->addWidget(label2);
    layout2_ter->addWidget(m_percentageBest);
    layout2_ter->addWidget(label2_bis);
    layout2->addLayout(layout2_bis);
    layout2->addLayout(layout2_ter);

    QVBoxLayout *layout3 = new QVBoxLayout;
    layout3->addLayout(layout1);
    layout3->addLayout(layout2);

    QGroupBox* groupbox = new QGroupBox;
    QVBoxLayout *gblayout = new QVBoxLayout;
    gblayout->addWidget(m_range);
    gblayout->addWidget(m_meanSpeed);
    gblayout->addWidget(m_maxSpeed);
    gblayout->addWidget(m_maxSpeedTime);
    groupbox->setTitle("Criterias");
    groupbox->setLayout(gblayout);

    QHBoxLayout *layout4 = new QHBoxLayout;
    layout4->addLayout(layout3);
    layout4->addWidget(groupbox);

    QVBoxLayout *layout5 = new QVBoxLayout;
    layout5->addWidget(label3);
    layout5->addWidget(m_notes);

    QHBoxLayout *layout6 = new QHBoxLayout;
    layout6->addLayout(layout4);
    layout6->addLayout(layout5);

    QHBoxLayout *layout7 = new QHBoxLayout;
    layout7->addWidget(m_deleteSelected);
    layout7->addWidget(m_saveSelected);
    layout7->addWidget(m_saveAll);
    layout7->addWidget(m_exportAll);

    QVBoxLayout* layout8 = new QVBoxLayout;
    layout8->addLayout(layout6);
    layout8->addWidget(m_table);
    layout8->addLayout(layout7);

    this->setCentralWidget(new QWidget());
    this->centralWidget()->setLayout(layout8);

    connect(this, SIGNAL(incReps(int)), m_nbReps, SLOT(display(int)));
    connect(Data::getInstance(), SIGNAL(newCurve(std::vector<float>)), this, SLOT(onNewCurve(std::vector<float>)));
    connect(m_saveAll, SIGNAL(clicked()), this, SLOT(save()));
    connect(m_deleteSelected, SIGNAL(clicked()), this, SLOT(deleteEntry()));
    connect(SerialPort::getInstance(), SIGNAL(error(QString&)), this, SLOT(onError(QString&)));
    connect(SerialPort::getInstance(), SIGNAL(status(QString&, int)), this, SLOT(onStatus(QString&)));
}

MainWindow::~MainWindow()
{

}

void MainWindow::configureWidgets()
{
    m_nbReps->setFixedSize(150, 150);


    m_buttons->addButton(m_range);
    m_buttons->addButton(m_meanSpeed);
    m_buttons->addButton(m_maxSpeed);
    m_buttons->addButton(m_maxSpeedTime);
    m_buttons->setId(m_range, 0);
    m_buttons->setId(m_meanSpeed, 1);
    m_buttons->setId(m_maxSpeed, 2);
    m_buttons->setId(m_maxSpeedTime, 3);


    m_percentagePrevious->setText("0%");
    m_percentageBest->setText("0%");
    QFont font;
    font.setPointSize(25);
    m_percentagePrevious->setFont(font);
    m_percentageBest->setFont(font);

    m_maxSpeed->setChecked(true);


    QStringList headers = {"Curve",
                           "Start\n(s)",
                           "Duration\n(s)",
                           "Range\n(cm)",
                           "MeanSpeed\n(m/s)",
                           "MaxSpeed\n(m/s)",
                           "MaxSpeedTime\n(s)",
                          };


    m_table->setHorizontalHeaderLabels(headers);

    m_deleteSelected->setMaximumWidth(100);


    m_saveSelected->setMaximumWidth(100);


    m_saveAll->setMaximumWidth(100);


    m_exportAll->setMaximumWidth(100);
}

void MainWindow::createActions()
{
    m_administration = new QAction("Administration", this);
    connect(m_administration, SIGNAL(triggered()), this, SLOT(onNewConfiguration()));

    m_connect = new QAction("Connect Interface", this);
    connect(m_connect, SIGNAL(triggered()), SerialPort::getInstance(), SLOT(connectInterface()));

    m_disconnect = new QAction("Disconnect Interface", this);

    m_start = new QAction("Start", this);
    connect(m_start, SIGNAL(triggered()), SerialPort::getInstance(), SLOT(start()));

    m_calibrate = new QAction("Calibrate", this);
    connect(m_calibrate, SIGNAL(triggered()), SerialPort::getInstance(), SLOT(calibrate()));

    m_stop = new QAction("Stop", this);
}

void MainWindow::createToolBar()
{
    QToolBar* t_acquisition = new QToolBar;
    t_acquisition->addAction(m_start);
    t_acquisition->addAction(m_calibrate);
    t_acquisition->addAction(m_stop);

    QToolBar *t_administration = new QToolBar;
    t_administration->addAction(m_administration);

    QToolBar *t_connect = new QToolBar;
    t_connect->addAction(m_connect);


    QToolBar *t_disconnect = new QToolBar;
    t_disconnect->addAction(m_disconnect);
    t_disconnect->setEnabled(false);


    this->addToolBar(t_acquisition);
    this->addToolBar(t_administration);
    this->addToolBar(t_connect);
    this->addToolBar(t_disconnect);
}

void MainWindow::onNewCurve(std::vector<float> vect)
{
    static std::vector<float> refBest = {0.01,0.01,0.01,0.01};
    static std::vector<float> refPrevious = {0.01,0.01,0.01,0.01};

    emit incReps(++m_nbCurve);

    m_table->setRowCount(m_table->rowCount()+1);
    m_table->setItem(m_nbCurve-1, 0, new QTableWidgetItem(QString::number(m_nbCurve)));
    for(ushort i = 0; i < vect.size(); ++i){
        m_table->setItem(m_nbCurve-1,i+1,new QTableWidgetItem(QString::number(vect[i])));
    }

    if(m_nbCurve == 1){
        for(ushort i = 0; i < 4; ++i){
            refBest[i] = vect[i+2];
            refPrevious[i] = vect[i+2];
        }
    }else{
        float percentageBest = vect[m_buttons->checkedId()+2]*100./refBest[m_buttons->checkedId()];
        float percentagePrevious = vect[m_buttons->checkedId()+2]*100./refPrevious[m_buttons->checkedId()];

        m_percentagePrevious->setText(QString::number(percentagePrevious, 'f',0)+"%");
        m_percentageBest->setText(QString::number(percentageBest, 'f', 0)+"%");


        if(percentageBest < threshold){
            /*"emit bip"*/
        }


        for(ushort i = 0; i < 4; ++i){
            if(vect[i+2] > refBest[i]){
                refBest[i] = vect[i+2];
            }
            refPrevious[i] = vect[i+2];
        }
    }

    m_table->scrollToBottom();
}


void MainWindow::save()
{

    QString folder = QFileDialog::getExistingDirectory(this, "Choose a folder");
    if(folder.isEmpty()){
        return;
    }

    QString f = c_prefixFolderName + QString::number(QDateTime::currentMSecsSinceEpoch());

    /* Create destination path */
    QDir dir(folder);
    if(!dir.mkpath(folder+"/"+f))
    {
        QString err("Impossible to create the save folder, if the problem persists please contact the administrator.");
        onError(err);
    }

    std::ofstream fnotes;
    std::ofstream fstats;

    QString raw_filename = folder+"/"+f+"/"+c_rawDataFileName;
    Data::getInstance()->save(raw_filename);

    fnotes.open((folder+"/"+f+"/"+c_notesDataFileName).toStdString());
    fstats.open((folder+"/"+f+"/"+c_statsDataFileName).toStdString());

    fnotes << m_notes->toPlainText().toStdString();


    fstats << "Curve;"
           <<"Start(s);"
          <<"Duration(s);"
         << "Range(cm);"
         <<"MeanSpeed(m/s);"
        <<"MaxSpeed(m/s);"
       <<"MaxSpeedTime(s)";
    fstats << "\n";

    for(int i = 0; i < m_table->rowCount(); ++i){
        for(int j = 0; j < m_table->columnCount(); ++j){
            fstats << m_table->item(i, j)->text().toStdString() + ";";
            std::cout << m_table->item(i, j)->text().toStdString();
        }

        fstats << "\n";
    }

    fnotes.close();
    fstats.close();
}

void MainWindow::createStatusBar()
{
    this->statusBar()->addWidget(m_status);
}

void MainWindow::onError(QString& err)
{
    QMessageBox::warning(this, "Error", err);
}

void MainWindow::onStatus(QString& stat)
{
    m_status->setText("");
    m_status->setText(stat);
}

void MainWindow::onNewConfiguration()
{
    m_acquisitionSettingsWindow->setVisible(true);
}


void MainWindow::deleteEntry()
{
    uint i = 0;
    QList<QTableWidgetItem*> list = m_table->selectedItems();
    std::set<int> indexes;

    for(QTableWidgetItem* item : list)
    {
        indexes.insert(item->row());
    }

    for(int index : indexes)
    {
        //std::cout<<index<<std::endl;
        m_table->removeRow(index-i);
        ++i;
    }

    m_nbCurve -= indexes.size();


    for(ushort i = 0; i < m_table->rowCount(); ++i)
    {
        m_table->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
    }

    Data::getInstance()->deleteEntry(indexes);

}
