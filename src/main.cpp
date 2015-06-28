/**
* LICENSE PLACEHOLDER
*
* @file main.cpp
* @package OpenPST
* @brief program entry point
*
* @author Gassan Idriss <ghassani@gmail.com>
* @author Matteson Raab <mraabhimself@gmail.com>
*/

#include <QApplication>
#include <QMetaType>
#include "gui/main_window.h"
#ifdef _WIN32
	#include <windows.h>
#endif

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
	#ifdef _WIN32
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	#endif

	QApplication application(argc, argv);

    openpst::MainWindow mainWindow;

    mainWindow.show();

	qRegisterMetaType<size_t>("size_t");

    return application.exec();
}
