#ifndef MBNTOOLWINDOW_H
#define MBNTOOLWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "ui_mbn_tool_window.h"
#include "mbn.h"
#include "util.h"

namespace Ui {
class MbnToolWindow;
}

class MbnToolWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MbnToolWindow(QWidget *parent = 0);
    ~MbnToolWindow();

public slots:
    void BrowseForFile();
    void LoadFile();
    void readX509Chain();

private:
    Ui::MbnToolWindow *ui;
    void log(const char* message);
    void log(std::string message);
    void log(QString message);
    void logHex(uint8_t* data, size_t amount);

};
#endif // MBNTOOLWINDOW_H
