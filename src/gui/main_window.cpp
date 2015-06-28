/**
* LICENSE PLACEHOLDER
*
* @file main_window.h
* @class MainWindow
* @package OpenPST
* @brief Main Window GUI interface class definitions
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "main_window.h"

using namespace openpst;

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
	QObject::connect(ui->startStreamingDloadButton, SIGNAL(clicked()), this, SLOT(startStreamingDload()));
}

/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
	delete ui;
}

/**
* @brief MainWindow::startStreamingDload
*/
void MainWindow::startStreamingDload()
{
	streamingDloadWindow = new StreamingDloadWindow(this);

	streamingDloadWindow->show();
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
