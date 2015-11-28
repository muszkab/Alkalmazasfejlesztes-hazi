import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

// Főablak
ApplicationWindow {
    title: qsTr("RobotDiagnosticApp")
    width: 640
    height: 480
    visible: true

    // Signalok TODO
    signal resetCommandCpp()
    signal accelerateCommandCpp()
    signal stopCommandCpp()

    menuBar: MenuBar{
        // Exit menü
        Menu{
            title: qsTr("&Exit")
            MenuItem{
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }

    MainForm{
        id: mainFormControl
        anchors.fill: parent

        onResetCommand: {
            resetCommandCpp();
        }
        onAccelerateCommand: {
            accelerateCommandCpp();
        }
        onStopCommand: {
            stopCommandCpp();
        }
    }
}

