import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#E4E2F3"
    
    property var reservationsModel: []
    property int selectedIndex: -1

    property color headerColor: "#2c3e50"
    property color rowEvenColor: "#DDDBF1"
    property color rowOddColor: "#C7C4E8"
    property color textColor: "#2c3e50"
    property color borderColor: "#bdc3c7"

    readonly property int colId:          40
    readonly property int colFecha:       60
    readonly property int colHora:        60
    readonly property int colComensales:  60
    readonly property int sidePad:        20

    function onDeleteRequested(index, data) {
        //console.log("Eliminar solicitado — indice:", index, "| nombre:", data.name_resv)
    }

    function addReservation(reservation) {
        reservationsModel.push(reservation)
        reservationsModel = reservationsModel
    }

    function removeReservation(index) {
        reservationsModel.splice(index, 1)
        reservationsModel = reservationsModel
    }

    function update_resvReservations(index, new_reservation) {
        reservationsModel[index] = Object.assign({}, reservationsModel[index], new_reservation)
        reservationsModel = reservationsModel
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            height: 50
            color: headerColor

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin:  root.sidePad
                anchors.rightMargin: root.sidePad
                spacing: 8

                Text {
                    Layout.preferredWidth: root.colId
                    text: "ID"
                    color: "white"
                    font.bold: true
                    font.pixelSize: 12
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                Text {
                    Layout.fillWidth: true
                    text: "A NOMBRE"
                    color: "white"
                    font.bold: true
                    font.pixelSize: 12
                    verticalAlignment: Text.AlignVCenter
                }

                Text {
                    Layout.preferredWidth: root.colFecha
                    text: "FECHA"
                    color: "white"
                    font.bold: true
                    font.pixelSize: 12
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                Text {
                    Layout.preferredWidth: root.colHora
                    text: "HORA"
                    color: "white"
                    font.bold: true
                    font.pixelSize: 12
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                Text {
                    Layout.preferredWidth: root.colComensales
                    text: "USUARIOS"
                    color: "white"
                    font.bold: true
                    font.pixelSize: 12
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }

        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: root.reservationsModel
            spacing: 1

            delegate: Rectangle {
                id: delegateRoot
                width: ListView.view.width
                height: 60
                color: index % 2 === 0 ? rowEvenColor : rowOddColor
                clip: true

                Rectangle {
                    anchors.fill: parent
                    color: mouseArea.containsMouse ? "#3498db" : "transparent"
                    opacity: mouseArea.containsMouse ? 0.1 : 0
                    Behavior on opacity { NumberAnimation { duration: 150 } }
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin:  root.sidePad
                    anchors.rightMargin: root.sidePad
                    spacing: 8

                    Text {
                        Layout.preferredWidth: root.colId
                        text: modelData.id_resv !== undefined ? modelData.id_resv : "—"
                        color: mouseArea.containsMouse ? "#2980b9" : textColor
                        font.pixelSize: 12
                        font.family: "Monospace"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Text {
                        Layout.fillWidth: true
                        text: modelData.name_resv || "—"
                        color: mouseArea.containsMouse ? "#2980b9" : textColor
                        font.pixelSize: 12
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }

                    Text {
                        Layout.preferredWidth: root.colFecha
                        text: modelData.date_resv || "—"
                        color: mouseArea.containsMouse ? "#2980b9" : textColor
                        font.pixelSize: 12
                        font.family: "Monospace"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Text {
                        Layout.preferredWidth: root.colHora
                        text: modelData.time_resv || "—"
                        color: mouseArea.containsMouse ? "#2980b9" : textColor
                        font.pixelSize: 12
                        font.family: "Monospace"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Text {
                        Layout.preferredWidth: root.colComensales
                        text: modelData.guest_resv !== undefined ? modelData.guest_resv : "—"
                        color: mouseArea.containsMouse ? "#2980b9" : textColor
                        font.pixelSize: 12
                        font.family: "Monospace"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                }

                Rectangle {
                    id: deletePanel
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    width: root.selectedIndex === index ? 120 : 0
                    color: "#e74c3c"
                    clip: true
                    z: 1

                    Behavior on width { NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }

                    Text {
                        anchors.centerIn: parent
                        text: "Eliminar"
                        color: "white"
                        font.bold: true
                        font.pixelSize: 12
                        visible: root.selectedIndex === index
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.onDeleteRequested(index, modelData)
                    }
                }

                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 1
                    color: borderColor
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        root.selectedIndex = (root.selectedIndex === index) ? -1 : index
                    }
                }
            }

            Rectangle {
                visible: listView.count === 0
                anchors.fill: parent
                color: "#F1F0F9"

                Column {
                    anchors.centerIn: parent
                    spacing: 10

                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "Hora"
                        font.pixelSize: 32
                    }

                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "No hay reservaciones"
                        color: "#7f8c8d"
                        font.pixelSize: 12
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        addReservation({ id_resv: 1, name_resv: "Juan Perez",   date_resv: "2025-07-15", time_resv: "19-00-00", guest_resv: 4  })
        addReservation({ id_resv: 2, name_resv: "Maria Garcia", date_resv: "2025-07-15", time_resv: "20-30-00", guest_resv: 2  })
        addReservation({ id_resv: 3, name_resv: "Carlos Lopez", date_resv: "2025-07-16", time_resv: "13-15-00", guest_resv: 10 })
    }
}