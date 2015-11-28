import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    width: 500
    height: 500
    anchors.fill: parent

    // Signalok
    signal resetCommand
    signal accelerateCommand
    signal stopCommand

    // Parancsok csoportja
    GroupBox{
        id: commandsGB
        title: "Parancsok"
        anchors.left: parent.left
        anchors.top: parent.top
        width: 250

        // Gombok oszlopbarendezése
        ColumnLayout{
            id: colBtn
            // töltse ki a rendelkezésre álló teret
            anchors.fill: parent

            // Reset nyomógomb
            Button{
                id: resetBtn
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Reset")
                anchors.leftMargin: 0
                anchors.rightMargin: 0
                onClicked: resetCommand()
            }

            // Gyorsítás nyomógomb
            Button{
                id: accelerateBtn
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Gyorsítás")
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                onClicked: accelerateCommand()
            }

            // Stop nyomógomb
            Button{
                id: stopBtn
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Stop")
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                onClicked: stopCommand()
            }

            // Kanyarodás
            Slider{
                id: kanyarSlider
                antialiasing: true
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 0
                anchors.rightMargin: 0
                value: 0.5
                maximumValue: 90
                minimumValue: -90
                stepSize: 1
            }

            Text{ text: "Elfordulás szöge: " + kanyarSlider.value + "°"}
        }
    }



}

