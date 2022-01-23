#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::captureDeviceProperties(const QBluetoothDeviceInfo &device)
{
    if(ui->comboBoxDevices->findText(device.name() + " " + device.address().toString()) == -1)
    {
        ui->comboBoxDevices->addItem(device.name() + " " + device.address().toString());
        addToLogs("Znaleziono urządzenie: " + device.name() + " " + device.address().toString());
    }
}
void MainWindow::on_searchButton_clicked()
{
    addToLogs("Szukam urzadzen:");
    ui->comboBoxDevices->clear();
    ui->searchButton->setEnabled(false);
    ui->StopSearchButton->setEnabled(true);
    ui->comboBoxDevices->clear();
    this->discoveryAgent->start(); // rozpoczęcie wyszukiwania
    connect(this->discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(captureDeviceProperties(QBluetoothDeviceInfo)));
    connect(this->discoveryAgent, SIGNAL(finished()),this, SLOT(searchingFinished()));
}
void MainWindow::searchingFinished()
{
    ui->connectButton->setEnabled(true);
    ui->StopSearchButton->setEnabled(false);
    addToLogs("Zakończono wyszukiwanie.");
    if(ui->comboBoxDevices->count() != 0)
    {
        ui->connectButton->setEnabled(true);
    }else
    {
        ui->connectButton->setEnabled(false);
    }
}
void MainWindow::addToLogs(QString message)
{
    QString dateAndTime =  QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ui->textEditLogs->append(dateAndTime + "\t" + message);
}
void MainWindow::on_connectButton_clicked()
{
    addToLogs("Łączę z urządzeniem...");
}
void MainWindow::on_disconnectButton_clicked()
{
    addToLogs("Zamknięto połączenie.");
}
void MainWindow::on_turnOnLedButton_clicked()
{
    addToLogs("Włączam LED");
}
void MainWindow::on_TurnOffLedButton_clicked()
{
    addToLogs("Wyłączam LED");
}
void MainWindow::on_ClearButton_clicked()
{
    ui->textEditLogs->clear();
}
void MainWindow::on_StopSearchButton_clicked()
{
    this->discoveryAgent->stop();
    ui->searchButton->setEnabled(true);
    ui->StopSearchButton->setEnabled(false);
    if(ui->comboBoxDevices->count() != 0)
    {
        ui->connectButton->setEnabled(true);
    }else
    {
        ui->connectButton->setEnabled(false);
    }
}
