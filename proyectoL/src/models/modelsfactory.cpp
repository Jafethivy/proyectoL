#include "modelsfactory.h"

ModelFactory::ModelFactory(DB* db, QObject* parent) : QObject(parent), m_db(db) {
}