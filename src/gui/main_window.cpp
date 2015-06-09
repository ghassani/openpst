#include "main_window.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->startSaharaButton, SIGNAL(clicked()), this, SLOT(startSaharaMode()));
    QObject::connect(ui->startPortRawWriteReadButton, SIGNAL(clicked()), this, SLOT(startPortRawWriteRead()));
    QObject::connect(ui->startMbnToolButton, SIGNAL(clicked()), this, SLOT(startMbnTool()));


}

MainWindow::~MainWindow()
{
    delete ui;

    if (saharaWindow) {
        saharaWindow->close();
        delete saharaWindow;
    }

    if (sendRawWindow) {
        sendRawWindow->close();
        delete sendRawWindow;
    }
}

void MainWindow::startSaharaMode()
{
    saharaWindow = new SaharaWindow(this);

    saharaWindow->show();
}

void MainWindow::startMbnTool()
{
    mbnToolWindow = new MbnToolWindow(this);

    mbnToolWindow->show();
}


void MainWindow::startPortRawWriteRead()
{
    sendRawWindow = new SendRawWindow(this);

    sendRawWindow->show();

}
