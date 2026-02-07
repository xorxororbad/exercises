import QtQuick
import QtQuick.Window
import QtQuick.Controls
import Qt.labs.platform

SystemTrayIcon {
    id: tray

    visible: true

    property var window: null

    icon.source: "qrc:/assets/icon.png"

    tooltip: "MediaCTL – click to restore"

    onActivated: reason => {
        if (tray.window) {
            if (!tray.window.visible) {
                console.log("Showing");
                tray.window.show();
                tray.window.raise();
                tray.window.requestActivate();
            } else {
                console.log("Minimizing");
                tray.window.hide();
            }
        }
    }

    menu: Menu {
        MenuItem {
            text: "Quit"
            onTriggered: Qt.quit()
        }
    }

    Component.onCompleted: {
    }
}

