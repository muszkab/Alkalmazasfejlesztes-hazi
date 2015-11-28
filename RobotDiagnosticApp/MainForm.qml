import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    width: 640
    height: 480
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
        }
    }

    // Aktuális értékek
    GroupBox{
        id: currValGB
        title: "Pillanatnyi értékek"
        // Fent és jobbra kitölti a szülőt
        // balra a parancs GB jobb oldalához illeszkedik
        anchors.top: parent.top
        anchors.left: commandsGB.right
        width: 200

        // Oszlopba rendezés
        ColumnLayout{
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left

            // TODO
            Text{ text: "Állapot: "}
            Text{ text: "Idő: "}
            Text{ text: "X: "}
            Text{ text: "Y: "}
            Text{ text: "V: "}
            Text{ text: "A: "}
            Text{ text: "Kormányállás: " + kanyarSlider.value + "°"}
            Text{ text: "Lámpa: "}
        }
    }

    Image{
        id: kormany
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: currValGB.right
        anchors.leftMargin: 20
        width: 150
        height: 150
        source: "kormany.png"
        rotation: kanyarSlider.value
    }

    // TODO lista és térkép
}

