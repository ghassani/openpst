#include "send_raw_window.h"
#include "ui_send_raw_window.h"

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
