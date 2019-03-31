#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initial values

    ui->lightPositionXSlider->setValue(0);
    ui->lightPositionYSlider->setValue(10);
    ui->lightPositionZSlider->setValue(0);
    ui->lightAmbientXSlider->setValue(50);
    ui->lightAmbientYSlider->setValue(50);
    ui->lightAmbientZSlider->setValue(50);
    ui->lightDiffuseXSlider->setValue(255);
    ui->lightDiffuseYSlider->setValue(255);
    ui->lightDiffuseZSlider->setValue(255);
    ui->lightSpecularXSlider->setValue(255);
    ui->lightSpecularYSlider->setValue(255);
    ui->lightSpecularZSlider->setValue(255);

    ui->materialShininessSlider->setValue(100);
    ui->materialAmbientXSlider->setValue(255);
    ui->materialAmbientYSlider->setValue(255);
    ui->materialAmbientZSlider->setValue(255);
    ui->materialDiffuseXSlider->setValue(255);
    ui->materialDiffuseYSlider->setValue(200);
    ui->materialDiffuseZSlider->setValue(0);
    ui->materialSpecularXSlider->setValue(255);
    ui->materialSpecularYSlider->setValue(200);
    ui->materialSpecularZSlider->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
