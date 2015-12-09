import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    width: 800
    height: 600
    anchors.fill: parent

    // Signalok
    signal resetCommand
    signal accelerateCommand
    signal stopCommand
    signal selfTestCommand
    signal kormanyChanged(int fok)

    GroupBox{
            id: felsoGB
            anchors.left: parent.left
            anchors.top: parent.top
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
                    onClicked: reset()
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
                    value: 0.5 //?biztos
                    maximumValue: 20
                    minimumValue: -20
                    stepSize: 5
                    tickmarksEnabled: true
                    onValueChanged: kormanyChanged(kanyarSlider.value)
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
                Text{ text:{ if(current.light===0) { "Állapot: " + current.statusName }
                             else if(current.light===2) "Állapot: Öntesztelés indul"
                             else if(current.light===3) "Állapot: Jobbra kanyarodás OK"
                             else if(current.light===4) "Állapot: Balra kanyarodás OK"
                             else if(current.light===5) "Állapot: Kormány alaphelyzet"
                             else if(current.light===6) "Állapot: Gyorsítás OK"
                             else if(current.light===7) "Állapot: Lassítás OK"
                             else if(current.light===8) "Állapot: Sikeres önteszt"
                             else                       "Állapot: Sikertelen önteszt!"
                      }
                    }
                Text{ text: "Idő: " + (current!=null ? current.timestamp.toFixed(1) : "?")}
                Text{ text: "X: " + (current!=null ? current.x.toFixed(3) : "?")}
                Text{ text: "Y: " + (current!=null ? current.y.toFixed(3) : "?")}
                Text{ text: "Orientáció: " + (current!=null ? current.orient+"°": "?")}
                Text{ text: "Kormány: " + (current!=null ? current.turn+"°": "?")}
                Text{ text: "V: " + (current != null ? current.v.toFixed(3) : "?")}
                Text{ text: "A: " + (current != null ? current.a.toFixed(3) : "?")}
            }
        }

        GroupBox{
            id: panel
            title: "Kijelzőpanel"
            anchors.top: parent.top
            anchors.left: currValGB.right
            width: 170

            Image{
                id: kormany
                anchors.top: parent.top
                anchors.left: currValGB.right
                anchors.leftMargin: 15
                width: 150
                height: 150
                source: "kormany.png"
                rotation:
                {
                    if(current.light) 3*current.turn
                    else 3*kanyarSlider.value
                }
            }

            Rectangle{
                id: indexBal
                anchors.top: kormany.bottom
                anchors.topMargin: 10
                anchors.left: kormany.left
                width: 20
                height: 30
                border.color: "black"
                color:
                {
                    if(kanyarSlider.value < 0 || current.statusName === "Önteszt") "yellow"
                    else "white"
                }
            }

            Rectangle{
                id: feklampaBal
                anchors.top: kormany.bottom
                anchors.topMargin: 10
                anchors.left: indexBal.right
                width: 50
                height: 30
                border.color: "black"
                color:
                {
                    if(current.statusName === "Megállás" || current.statusName === "Önteszt") "red"
                    else "white"
                }
            }

            Rectangle{
                id: feklampaJobb
                anchors.top: kormany.bottom
                anchors.topMargin: 10
                anchors.right: indexJobb.left
                width: 50
                height: 30
                border.color: "black"
                color:
                {
                    if(current.statusName === "Megállás" || current.statusName === "Önteszt") "red"
                    else "white"
                }
            }

            Rectangle{
                id: indexJobb
                anchors.top: kormany.bottom
                anchors.topMargin: 10
                anchors.right: kormany.right
                width: 20
                height: 30
                border.color: "black"
                color:
                {
                    if(kanyarSlider.value > 0 || current.statusName === "Önteszt") "yellow"
                    else "white"
                }
            }
        }

        GroupBox{
            id: vectorGB
            title: qsTr("Vektorkijelző")
            anchors.left: panel.right
            anchors.leftMargin: 15
            anchors.top: parent.top
            width: 165
            height: 180

            RowLayout{
                anchors.fill: parent
                spacing: 0
                VectorGraph{
                    id: vectorGraph
                    objectName: "vectorGraph"

                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.minimumWidth: 150
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 100
                    Layout.minimumHeight: 150
                }
            }
        }
    }

    // History listának szüksége lesz egy delegate-re. Minden lista elem ennek a
    // komponensnek egy példánya lesz.
    Component{
        id: stateDelegate
        Row{
            // A model a lista egyik eleme.
            Text{ text: model.statusName }
            Text{ text: " X:" + model.x.toFixed(2) ; }
            Text{ text: " Y:" + model.y.toFixed(2) }
            Text{ text: " Szög:" + model.orient }
            Text{ text: " V:" + model.v.toFixed(2) }
            Text{ text: " A:" + model.a.toFixed(2) }
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
        anchors.top: felsoGB.bottom
        anchors.topMargin: 0

        // Egymás mellett lista és grafikon
        RowLayout{
            anchors.fill: parent
            spacing: 0
            // A lista scrollozható
            ScrollView{
                // A scrollohzató elem igazítása a szölő RowLayouthoz.
                // Itt a ScrollViewon belül adjuk meg, hogy a RowLayoutban
                //  mik legyenek a rá (ScrollViewra) vonatkozó méret beállítások,
                //  mert ezeket a RowLayout kezeli ebben az esetben.
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumWidth: 250
                Layout.preferredWidth: 330
                Layout.maximumWidth: 350
                Layout.minimumHeight: 150

                // A tényleges lista
                ListView{
                    id: stateHistoryList
                    // A model az, ahonnan az adatokat vesszük.
                    // A historyModel változót a MainWindowsEventHandling::historyChanged metódus teszi
                    //  elérhetővé a QML oldalon is.
                    //  Típusa QList<QObject*>, a tárolt pointerek valójában RobotState-ekre mutatnak.
                    model: historyModel
                    // A delegate megadása, vagyis hogy egy listaelem hogyan jelenjen meg.
                    //  (Már fentebb definiáltuk.)
                    delegate: stateDelegate

                    // Eseménykezelő, az elemek darabszámának változása esetén a kijelölést
                    //  a legalsó elemre viszi. Ezzel oldja meg, hogy folyamatosan scrollozódjon
                    //  a lista és a legutoló elem mindig látható legyen.
                    onCountChanged: {
                        stateHistoryList.currentIndex = stateHistoryList.count - 1;
                    }
                }
            }

            // A HistoryGraph példányosítása, melyet külön fájlban definiáltunk.
            //  (A rendszer név alapján találja meg a fájlt.)
            HistoryGraph{
                id: historyGraph
                // Az objectName akkor jó, ha C++ oldalról kell megkeresnünk egy QML oldalon definiált
                //  objektumot a findChild metódus rekurzív hívásaival.
                objectName: "historyGraph"

                // A RowLayout erre az elemre vonatkozó elhelyezés beállításai.
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumWidth: 200
                Layout.preferredWidth: 400
                Layout.minimumHeight: 150

                // Ezek pedig a HistoryGraph tulajdonságai, amiket majd ott definiálunk,
                //  itt pedig értéket adunk nekik. Az alábbi változókat (pl. historyGraphTimestamps)
                //  szintén a MainWindowsEventHandling::historyChanged metódus teszi elérhetővé
                //  a QML oldal számára.
                // Ezek az értékek C++ oldalon folyamatosan változnak. Minden változás esetén
                //  lefut a MainWindowsEventHandling::historyChanged és ezeket újraregisztrálja a QML
                //  oldal számára, így frissülni fog a HistoryGraph tulajdonság is.
                graphPositionX: historyGraphPositionX
                graphPositionY: historyGraphPositionY
           }
        }
    }

    function reset()
    {
        kanyarSlider.value = 0;
        resetCommand();
    }
}
