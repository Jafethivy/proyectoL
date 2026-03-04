#include <QtWidgets/QApplication>
#include "AppContext.h"
#include "view/Login/proyectoL.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    AppContext context;
	auto* window = context.initialize();

    window->show();
    return app.exec();
}
