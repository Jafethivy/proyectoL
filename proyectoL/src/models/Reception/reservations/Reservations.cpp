#include "Reservations.h"

Reservations::Reservations(QObject *parent)
	: QObject(parent)
{}

Reservations::~Reservations()
{}

void Reservations::addReservation(const QJsonObject& reservation){
}

void Reservations::removeReservation(int index){
}

void Reservations::updateReservation(int index, const QJsonObject& data){
}
