import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    width: 220
    height: 380
    color: "#2d2d2d"
    radius: 8
    border.color: "#404040"
    border.width: 1

    property int currentView: 0

    signal reservationCreated(var data)
    signal reservationEdited(var data)

    StackLayout {
        anchors.fill: parent
        currentIndex: root.currentView

        // Vista 0: Menu principal
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 10
            spacing: 12

            Text {
                text: "Reservaciones"
                color: "white"
                font.bold: true
                font.pixelSize: 14
                Layout.topMargin: 10
                Layout.alignment: Qt.AlignHCenter
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: "#404040"
            }

            Item { Layout.fillHeight: true }

            ItemDelegate {
                Layout.fillWidth: true
                height: 48

                contentItem: Text {
                    text: "Crear Reservacion"
                    color: "white"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 14
                }

                background: Rectangle {
                    color: parent.hovered ? "#555555" : "#444444"
                    radius: 10
                    border.color: "#666666"
                    border.width: 1
                }

                onClicked: root.currentView = 1
            }

            ItemDelegate {
                Layout.fillWidth: true
                height: 48

                contentItem: Text {
                    text: "Editar Reservacion"
                    color: "white"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 14
                }

                background: Rectangle {
                    color: parent.hovered ? "#404040" : "#2d2d2d"
                    radius: 10
                    border.color: "#555555"
                    border.width: 1
                }

                onClicked: root.currentView = 2
            }

            Item { Layout.fillHeight: true }
        }

        // Vista 1: Crear reservacion
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 8

            Rectangle {
                Layout.fillWidth: true
                height: 42
                color: "#1e1e1e"

                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    text: "Crear Reservacion"
                    color: "#aaaaaa"
                    font.pixelSize: 14
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: root.currentView = 0
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.leftMargin: 12
                Layout.rightMargin: 12
                spacing: 6

                MenuField {
                    id: createName
                    label: "A nombre de"
                    placeholder: "Nombre"
                    maxLength: 40
                    errorMsg: "Requerido, max 40 caracteres"
                    validatorObj: RegularExpressionValidator {
                        regularExpression: /^.{1,40}$/
                    }
                }
                MenuField {
                    id: createDate
                    label: "Fecha"
                    placeholder: "YYYY-MM-DD"
                    errorMsg: "Formato requerido: YYYY-MM-DD"
                    validatorObj: RegularExpressionValidator {
                        regularExpression: /^\d{4}-\d{2}-\d{2}$/
                    }
                }
                MenuField {
                    id: createTime
                    label: "Hora"
                    placeholder: "HH-MM-SS"
                    errorMsg: "Hora invalida: HH(0-23)-MM(0-59)-SS(0-59)"
                    validatorObj: RegularExpressionValidator {
                        regularExpression: /^(0[0-9]|1[0-9]|2[0-3])-(0[0-9]|[1-5][0-9])-(0[0-9]|[1-5][0-9])$/
                    }
                }
                MenuField {
                    id: createGuests
                    label: "Comensales"
                    placeholder: "1"
                    errorMsg: "Numero entre 1 y 999"
                    validatorObj: IntValidator { bottom: 1; top: 999 }
                }

                Rectangle { height: 2; color: "transparent" }

                MenuButton {
                    Layout.fillWidth: true
                    label: "Guardar"
                    enabled: createName.isValid && createDate.isValid && createTime.isValid && createGuests.isValid
                    onAction: {
                        var data = {
                            name_resv:   createName.value,
                            date_resv:   createDate.value,
                            time_resv:   createTime.value,
                            guest_resv: parseInt(createGuests.value) || 0
                        }
                        reservationCreated(data)
                        createName.clear(); createDate.clear()
                        createTime.clear(); createGuests.clear()
                        root.currentView = 0
                    }
                }

                MenuButton {
                    Layout.fillWidth: true
                    label: "Volver"
                    secondary: true
                    onAction: {
                        createName.clear(); createDate.clear()
                        createTime.clear(); createGuests.clear()
                        root.currentView = 0
                    }
                }
            }

            Item { Layout.fillHeight: true }
        }

        // Vista 2: Editar reservacion
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 8

            Rectangle {
                Layout.fillWidth: true
                height: 42
                color: "#1e1e1e"

                Text {
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    text: "Editar Reservacion"
                    color: "#aaaaaa"
                    font.pixelSize: 14
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: root.currentView = 0
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.leftMargin: 12
                Layout.rightMargin: 12
                spacing: 6

                MenuField {
                    id: editId
                    label: "ID"
                    placeholder: "1"
                    errorMsg: "ID invalido"
                    required: true
                    validatorObj: IntValidator { bottom: 1; top: 999999 }
                }
                MenuField {
                    id: editName
                    label: "A nombre de"
                    placeholder: "Nombre (opcional)"
                    maxLength: 40
                    errorMsg: "Max 40 caracteres"
                    required: false
                    validatorObj: RegularExpressionValidator {
                        regularExpression: /^.{1,40}$/
                    }
                }
                MenuField {
                    id: editDate
                    label: "Fecha"
                    placeholder: "YYYY-MM-DD (opcional)"
                    errorMsg: "Formato requerido: YYYY-MM-DD"
                    required: false
                    validatorObj: RegularExpressionValidator {
                        regularExpression: /^\d{4}-\d{2}-\d{2}$/
                    }
                }
                MenuField {
                    id: editTime
                    label: "Hora"
                    placeholder: "HH-MM-SS (opcional)"
                    errorMsg: "Hora invalida: HH(0-23)-MM(0-59)-SS(0-59)"
                    required: false
                    validatorObj: RegularExpressionValidator {
                        regularExpression: /^(0[0-9]|1[0-9]|2[0-3])-(0[0-9]|[1-5][0-9])-(0[0-9]|[1-5][0-9])$/
                    }
                }
                MenuField {
                    id: editGuests
                    label: "Comensales"
                    placeholder: "1-999 (opcional)"
                    errorMsg: "Numero entre 1 y 999"
                    required: false
                    validatorObj: IntValidator { bottom: 1; top: 999 }
                }

                MenuButton {
                    Layout.fillWidth: true
                    label: "Guardar cambios"
                    enabled: editId.isValid && editName.isValid && editDate.isValid && editTime.isValid && editGuests.isValid
                    onAction: {
                        var data = {
                            id_resv:     parseInt(editId.value) || 0,
                            name_resv:   editName.value,
                            date_resv:   editDate.value,
                            time_resv:   editTime.value,
                            guest_resv: parseInt(editGuests.value) || 0
                        }
                        reservationEdited(data)
                        editId.clear(); editName.clear(); editDate.clear()
                        editTime.clear(); editGuests.clear()
                        root.currentView = 0
                    }
                }

                MenuButton {
                    Layout.fillWidth: true
                    label: "Volver"
                    secondary: true
                    onAction: {
                        editId.clear(); editName.clear(); editDate.clear()
                        editTime.clear(); editGuests.clear()
                        root.currentView = 0
                    }
                }
            }

            Item { Layout.fillHeight: true }
        }
    }

    component MenuField: ColumnLayout {
    property string label: ""
    property string placeholder: ""
    property string errorMsg: ""
    property int    maxLength: 32767
    property alias  value: tf.text
    property alias  validatorObj: tf.validator
    property bool   required: true
    readonly property bool isValid: required ? tf.acceptableInput : (tf.text.length === 0 || tf.acceptableInput)
    spacing: 2
    Layout.fillWidth: true

    property bool touched: false

    function clear() {
        tf.text = ""
        touched = false
    }

    Text {
        text: label
        color: "#aaaaaa"
        font.pixelSize: 14
    }

    TextField {
        id: tf
        Layout.fillWidth: true
        height: 32
        placeholderText: placeholder
        placeholderTextColor: "#666666"
        color: "white"
        font.pixelSize: 14
        leftPadding: 8
        maximumLength: maxLength

        onActiveFocusChanged: {
            if (!activeFocus && text.length > 0)
                touched = true
        }

        background: Rectangle {
            color: "#1e1e1e"
            radius: 4
            border.width: 1
            border.color: {
                if (tf.activeFocus)                                return "#666666"
                if (touched && !tf.acceptableInput)               return "#e74c3c"
                if (touched && tf.acceptableInput)                return "#2ecc71"
                return "#404040"
            }
        }
    }

    Text {
        visible: touched && !tf.acceptableInput
        text: errorMsg
        color: "#e74c3c"
        font.pixelSize: 10
        wrapMode: Text.WordWrap
        Layout.fillWidth: true
    }
}

    component MenuButton: Rectangle {
        property string label: ""
        property bool   secondary: false
        signal action()

        height: 32
        radius: 4
        color: !enabled
               ? "#333333"                                                         // COLOR: boton deshabilitado
               : secondary
                 ? (btnArea.containsMouse ? "#404040" : "#2d2d2d")                // COLOR: boton secundario (hover : normal)
                 : (btnArea.containsMouse ? "#555555" : "#444444")                // COLOR: boton primario   (hover : normal)
        border.color: "#555555"
        border.width: secondary ? 1 : 0
        opacity: enabled ? 1.0 : 0.4

        Text {
            anchors.centerIn: parent
            text: label
            color: "white"
            font.pixelSize: 14
            font.bold: !secondary
        }

        MouseArea {
            id: btnArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
            onClicked: if (parent.enabled) action()
        }
    }
}