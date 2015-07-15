/**
* LICENSE PLACEHOLDER
*
* @file application.cpp
* @package OpenPST
* @brief GUI main application wrapper
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include <QApplication>
#include <QMetaType>
#include <QLibraryInfo>
#include <QTranslator>
#ifdef _WIN32
#include <windows.h>
#endif

namespace openpst {
	class Application : public QApplication {
		public:
			Application(int argc, char *argv[], const char* translationPrefix);
			~Application();
		private:
			QTranslator qtTranslator;
			QTranslator	translator;

#if defined (_WIN32) && defined (DEBUG)
			FILE* sout;
			FILE* serr;
#endif
	};

}