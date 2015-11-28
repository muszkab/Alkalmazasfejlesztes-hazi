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
    signal selfTestCommand

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

            // Öntesztelés
            Button{
                id:selfTestBtn
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Önteszt")
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                onClicked: selfTestCommand()
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

    Text{
        anchors.top: kormany.bottom
        anchors.topMargin: 10
        anchors.left: kormany.left
        text: "Kormányállás: " + kanyarSlider.value + "°"
    }

    // History listának szüksége lesz egy delegate-re. Minden lista elem ennek a
    // komponensnek egy példánya lesz.
    Component{
        id: stateDelegate
        Row{
            // A model a lista egyik eleme.
            Text{ text: model.statusName }
            Text{ text: "X: " + model.x.toFixed(3) }
            Text{ text: "Y: " + model.y.toFixed(3) }
            Text{ text: "V: " + model.v.toFixed(3) }
            Text{ text: "A: " + model.A.toFixed(3) }
            //Text{ text: "Szög: " + model.alfa.toFixed(3) }

        }
    }

    // Állapotlista
    GroupBox{
        id: stateListGB
        title: qsTr("Állapotlista")
        // Oldalra és lefelé kitölti a szülőt
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        // Felfelé a leglejjebb lógó GB-hoz igazodik
        anchors.top: (commandsGB.bottom > currValGB.bottom ? commandsGB.bottom : currValGB.bottom)
        anchors.topMargin: 0

        // Egymás mellett lista és grafikon
        RowLayout{
            anchors.fill: parent
            spacing: 0
            // A lista scrollozható
            ScrollView{
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumWidth: 250
                Layout.preferredWidth: 250
                Layout.maximumWidth: 300
                Layout.minimumHeight: 150

                // A tényleges lista
                ListView{
                    id: stateHistoryList
                    model: historyModel
                    delegate: stateDelegate

                    onCountChanged: {
                        stateHistoryList.currentIndex = stateHistoryList.count - 1;
                    }
                }
            }

            // TODO grafikon
            HistoryGraph{
                id: historyGraph
                objectName: "historyGraph"

                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumWidth: 200
                Layout.preferredWidth: 400
                Layout.minimumHeight: 150

                //graphTimestamps: historyGraphTimestamps
                //graphVelocities: historyGraphVelocity
                //graphAccelerations: historyGraphAcceleration
           }
        }
    }
    // TODO lista és térkép
}

