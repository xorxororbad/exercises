import QtQuick
import QtQuick.Window 2.15
import QtQuick.Controls 2.15   // only if you need Controls

import MediaModule 1.0

ApplicationWindow {
    id: window

    x: 0
    y: 0

    width: 375
    height: 75

    minimumWidth: width
    maximumWidth: width
    minimumHeight: height
    maximumHeight: height

    visible: true
    title: qsTr("Basic Sound Control - dbus")

    flags: (Qt.FramelessWindowHint | Qt.Window)

    Rectangle {
        id: bg
        anchors.fill: parent

        color: "#323232"
        focus: true

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton

            onPressed: {
                window.startSystemMove();
            }
        }

        MediaButton {
            id: btnNext

            shapeStr: "<path d='M8.59 16.59L13.17 12 8.59 7.41 10 6l6 6-6 6z'/>"

            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
            }

            onClicked: {
                MediaCtl.next();
            }
        }

        MediaButton {
            id: btnTogglePlay

            property bool playing: MediaCtl.status == MediaCtl.PLAY

            property string play: "<path d='M8 5v14l11-7z'/>"
            property string pause: "<rect x='6' y='4' width='4' height='16' rx='1'/><rect x='14' y='4' width='4' height='16' rx='1'/>"

            color: playing ? "gray" : "#B10065"

            shapeStr: (playing ? pause : play)

            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }

            onClicked: {
                MediaCtl.toggle_play();
            }
        }

        MediaButton {
            id: btnPrevious

            shapeStr: "<path d='M15.41 7.41L14 6l-6 6 6 6 1.41-1.41L10.83 12z'/>"

            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }

            onClicked: {
                MediaCtl.previous();
            }
        }

        Keys.onEscapePressed: {
            console.log("Exiting!");
            Qt.quit();
        }
    }

    MediaSystemTray {
        window: window
    }

    Component.onCompleted: {
        console.log("Window completed..");
    }
}
