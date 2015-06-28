/**
* LICENSE PLACEHOLDER
*
* @file send_raw_window.ccpp
* @class SendRawWindow
* @package OpenPST
* @brief Send raw read/write GUI interface class
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#include "send_raw_window.h"
#include "ui_send_raw_window.h"

using namespace openpst;

SendRawWindow::SendRawWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SendRawWindow)
{
    ui->setupUi(this);
}

SendRawWindow::~SendRawWindow()
{
    delete ui;
}
