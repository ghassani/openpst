/**
* LICENSE PLACEHOLDER
*/

#include "main_window.h"

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    ui->setupUi(this);

    QObject::connect(ui->startSaharaButton, SIGNAL(clicked()), this, SLOT(startSaharaMode()));
    QObject::connect(ui->startPortRawWriteReadButton, SIGNAL(clicked()), this, SLOT(startPortRawWriteRead()));
    QObject::connect(ui->startMbnToolButton, SIGNAL(clicked()), this, SLOT(startMbnTool()));
    QObject::connect(ui->startQcdmButton, SIGNAL(clicked()), this, SLOT(startQcdm()));


}

/**
 * @brief MainWindow::~MainWindow
 */
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

/**
 * @brief MainWindow::startSaharaMode
 */
void MainWindow::startSaharaMode()
{
    saharaWindow = new SaharaWindow(this);

    saharaWindow->show();
}

/**
 * @brief MainWindow::startMbnTool
 */
void MainWindow::startMbnTool()
{
    mbnToolWindow = new MbnToolWindow(this);

    mbnToolWindow->show();
}

/**
 * @brief MainWindow::startPortRawWriteRead
 */
void MainWindow::startPortRawWriteRead()
{
    sendRawWindow = new SendRawWindow(this);

    sendRawWindow->show();

}

/**
 * @brief MainWindow::startQcdm
 */
void MainWindow::startQcdm()
{
    qcdmWindow = new QcdmWindow(this);

    qcdmWindow->show();
}
