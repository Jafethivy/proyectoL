#include <QObject>

class DB;

class ModelFactory : public QObject {
    Q_OBJECT
public:
    explicit ModelFactory(DB* db, QObject* parent = nullptr);

public slots:
    // Crea y devuelve el modelo, ya en el hilo correcto (Model Thread)
    //QObject* createReceptionModel();

private:
    DB* m_db;
};