/**
* LICENSE PLACEHOLDER
*
* @file sahara.cpp
* @package OpenPST
* @brief sahara program entry point
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "gui/application.h"
#include "gui/sahara_window.h"

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    OpenPST::Application application(argc, argv, "sahara_");

    OpenPST::SaharaWindow window;

    window.show();

    return application.exec();
}