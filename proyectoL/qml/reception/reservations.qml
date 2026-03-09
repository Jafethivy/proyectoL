import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#E4E2F3"
    
    // Propiedades para conectar con el modelo (Controller/Model)
    property var reservationsModel: []

    property color headerColor: "#2c3e50"
    property color rowEvenColor: "#DDDBF1"
    property color rowOddColor: "#C7C4E8"
    property color textColor: "#2c3e50"
    property color borderColor: "#bdc3c7"
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        
        // Header de la tabla
        Rectangle {
            Layout.fillWidth: true
            height: 50
            color: headerColor
            
            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                spacing: 0
                
                Text {
                    Layout.preferredWidth: parent.width * 0.4
                    text: "A NOMBRE"
                    color: "white"
                    font.bold: true
                    font.pixelSize: 12
                    verticalAlignment: Text.AlignVCenter
                }
                
                Text {
                    Layout.preferredWidth: parent.width * 0.3
                    text: "HORA"
                    color: "white"
                    font.bold: true
                    font.pixelSize: 12
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                
                Text {
                    Layout.preferredWidth: parent.width * 0.3
                    text: "MESA"
                    color: "white"
                    font.bold: true
                    font.pixelSize: 12
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                }
            }
        }
        
        // Lista de reservaciones
        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: root.reservationsModel
            spacing: 1
            
            delegate: Rectangle {
                width: ListView.view.width
                height: 60
                color: index % 2 === 0 ? rowEvenColor : rowOddColor
                
                // Efecto hover
                Rectangle {
                    anchors.fill: parent
                    color: mouseArea.containsMouse ? "#3498db" : "transparent"
                    opacity: mouseArea.containsMouse ? 0.1 : 0
                    Behavior on opacity {
                        NumberAnimation { duration: 150 }
                    }
                }
                
                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 20
                    anchors.rightMargin: 20
                    spacing: 0
                    
                    Text {
                        Layout.preferredWidth: parent.width * 0.4
                        text: modelData.name || "—"
                        color: mouseArea.containsMouse ? "#2980b9" : textColor
                        font.pixelSize: 12
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    
                    Text {
                        Layout.preferredWidth: parent.width * 0.3
                        text: modelData.time || "—"
                        color: mouseArea.containsMouse ? "#2980b9" : textColor
                        font.pixelSize: 12
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    
                    Rectangle {
                        Layout.preferredWidth: parent.width * 0.3
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        height: 30
                        width: Math.min(tableNumberText.implicitWidth + 20, 80)
                        radius: 15
                        color: getTableColor(modelData.table)
                        
                        Text {
                            id: tableNumberText
                            anchors.centerIn: parent
                            text: modelData.table || "—"
                            color: "white"
                            font.bold: true
                            font.pixelSize: 12
                        }
                    }
                }
                
                // Línea divisoria inferior
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
                        root.reservationSelected(index, modelData)
                    }
                }
            }
            
            // Estado vacío
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
    
    // Función helper para colores de mesa
    function getTableColor(tableNumber) {
        if (!tableNumber) return "#95a5a6"
        const colors = ["#e74c3c", "#3498db", "#2ecc71", "#f39c12", "#9b59b6", "#1abc9c"]
        return colors[(parseInt(tableNumber) - 1) % colors.length] || "#95a5a6"
    }
    
    // Método público para actualizar datos desde el Controller
    function addReservation(reservation){
        reservationsModel.push(reservation)
        reservationsModel = reservationsModel
    }

    function removeReservation(index){
        reservationsModel.splice(index, 1)
        reservationsModel = reservationsModel
    }

    function updateReservations(index, new_reservation) {
        reservationsModel[index] = Object.assing({}, reservationsModel[index], new_reservation)
        reservationsModel = reservationsModel
    }

    Component.onCompleted: {
        var dat1 = { name: "Juan Perez", time: "19:00", table: "Mesa 1" }
        var dat2 = { name: "Maria Garcia", time: "20:30", table: "Mesa 3" }
        addReservation(dat1)
        addReservation(dat2)
    }
}