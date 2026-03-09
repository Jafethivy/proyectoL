#pragma once
#include <QObject>
#include <QThread>
#include <memory>

class proyectoL;
class login;
class reception;

class Controller;
class reception_controller;

class DB;
class Auth;
class ModelFactory;

class AppContext : public QObject {
    Q_OBJECT
public:
    explicit AppContext(QObject* parent = nullptr);
    ~AppContext();

    // No copiable
    AppContext(const AppContext&) = delete;
    AppContext& operator=(const AppContext&) = delete;

    // Inicializa todo y retorna la ventana principal
	proyectoL* initialize();

private:
    void createObjects();
    void setupThreads();
    void setupConnections();
    void cleanup();
	void debugConnections();

    // Hilo del modelo
    QThread WorkerThread;
    
    // Objetos (raw pointers porque manejamos lifetime manualmente con deleteLater)
	proyectoL* m_window = nullptr;
    
    //Login StackedWidget
	login* m_loginWidget = nullptr;
	//Reception StackedWidget
	reception* m_receptionWidget = nullptr;

	//Controllers
    Controller* m_controller = nullptr;
	reception_controller* m_receptionController = nullptr;

    Auth* m_auth = nullptr;
	DB* m_db = nullptr;
    ModelFactory* m_modelFactory = nullptr;
};