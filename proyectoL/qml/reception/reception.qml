import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    width: 200
    height: 300
    color: "#2d2d2d"
    radius: 8
    border.color: "#404040"
    border.width: 1
    
    signal itemSelected(string itemName)
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 5
        
        Text {
            text: "Menu QML"
            color: "white"
            font.bold: true
            font.pixelSize: 16
        }
        
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#404040"
        }
        
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: ["Opcion 1", "Opcion 2", "Opcion 3", "Configuracion", "Salir"]
            clip: true
            
            delegate: ItemDelegate {
                width: parent.width
                height: 40
                
                contentItem: Text {
                    text: modelData
                    color: "white"
                    verticalAlignment: Text.AlignVCenter
                }
                
                background: Rectangle {
                    color: parent.hovered ? "#404040" : "transparent"
                    radius: 4
                }
                
                onClicked: {
                    console.log("Clic en:", modelData)
                    root.itemSelected(modelData)
                }
            }
        }
    }
}