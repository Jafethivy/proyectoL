import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    anchors.fill: parent
    color: "#293651"

    property var tableStates: [
        0, 0, 1, 0, 2, 2, 1, 0, 0, 0,
        0, 0, 1, 0, 2, 2, 1, 0, 0, 0
        ]

    function getColor(state) {
        if (state === 0) return "#4CAF50"
        if (state === 1) return "#FFC107"
        if (state === 2) return "#F44336"
        return "#9E9E9E"
    }

    function getStatusText(state) {
        if (state === 0) return "Disponible"
        if (state === 1) return "Reservado"
        if (state === 2) return "Ocupado"
        return "Desconocido"
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        Text {
            text: "Mesas del Restaurante"
            font.pixelSize: 24
            font.bold: true
            color: "#ffffff"
            Layout.alignment: Qt.AlignHCenter
        }

        GridLayout {
            Layout.alignment: Qt.AlignHCenter
            columns: 8
            rowSpacing: 45
            columnSpacing: 30

            Repeater {
                model: 20

                Rectangle {
                    id: tableItem
                    width: 60
                    height: 60
                    radius: 35
                    
                    // BINDING AUTOMATICO - se actualiza solo cuando cambia tableStates
                    color: getColor(tableStates[index])
                    
                    border.width: 2
                    border.color: "#ffffff"

                    // Sombra
                    Rectangle {
                        anchors.fill: parent
                        radius: 35
                        color: "#000000"
                        opacity: 0.3
                        z: -1
                        anchors.margins: -4
                    }

                    Text {
                        anchors.centerIn: parent
                        text: index + 1
                        font.pixelSize: 20
                        font.bold: true
                        color: "#ffffff"
                    }

                    // Tooltip
                    Rectangle {
                        id: statusTooltip
                        anchors.top: parent.bottom
                        anchors.topMargin: 8
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: statusText.width + 16
                        height: 24
                        radius: 4
                        color: "#2d2d44"
                        border.color: tableItem.color
                        border.width: 1
                        opacity: 0
                        visible: opacity > 0

                        Text {
                            id: statusText
                            anchors.centerIn: parent
                            text: getStatusText(tableStates[index])
                            font.pixelSize: 11
                            color: "#ffffff"
                        }
                        
                        Behavior on opacity {
                            NumberAnimation { duration: 150 }
                        }
                    }

                    // UN SOLO MOUSEAREA que maneja todo
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        
                        onEntered: {
                            statusTooltip.opacity = 1
                            tableItem.scale = 1.1
                        }
                        onExited: {
                            statusTooltip.opacity = 0
                            tableItem.scale = 1.0
                        }
                        onClicked: {
                            // Crear nueva copia del array para forzar actualizacion
                            var newStates = tableStates.slice()
                            newStates[index] = (newStates[index] + 1) % 3
                            tableStates = newStates
                        }
                    }

                    scale: 1.0
                    Behavior on scale {
                        NumberAnimation { duration: 150 }
                    }
                }
            }
        }

        // Leyenda
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 20

            Repeater {
                model: [
                    { color: "#4CAF50", text: "Disponible" },
                    { color: "#FFC107", text: "Reservado" },
                    { color: "#F44336", text: "Ocupado" }
                ]

                RowLayout {
                    spacing: 6
                    Rectangle {
                        width: 16
                        height: 16
                        radius: 8
                        color: modelData.color
                    }
                    Text {
                        text: modelData.text
                        font.pixelSize: 12
                        color: "#ffffff"
                    }
                }
            }
        }
    }
}