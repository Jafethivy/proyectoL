#include "proyectoL.h"

proyectoL::proyectoL(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	Controller* controller = new Controller();


}

proyectoL::~proyectoL()
{}

void proyectoL::on_button_clicked(){
	qDebug() << "Button clicked!";
}