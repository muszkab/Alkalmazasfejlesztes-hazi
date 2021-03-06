import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

// Főablak
ApplicationWindow {
    title: qsTr("RobotDiagnosticApp")
    width: 810
    height: 600
    visible: true

    // Signalok
    signal resetCommandCpp()
    signal accelerateCommandCpp()
    signal stopCommandCpp()
    signal selfTestCommandCpp()
    signal kormanyChangedCpp(int fok)

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
        onSelfTestCommand: {
            selfTestCommandCpp();
        }
        onKormanyChanged:   {
            kormanyChangedCpp(fok);
        }
    }
}
