import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Screenshots comparer"

    Connections {
        target: appCore

        function onSendScreenshotData(path, hash, percentage) {
            screenshotsModel.append({"path": "", "hash": "", "percentage": ""})
            screenshotsModel.move(0, 1, screenshotsModel.count - 1)
            screenshotsModel.insert(0,{"path": path, "hash": hash, "percentage": percentage})
        }
    }

    ListModel {
        id: screenshotsModel
    }

    GridView {
        id: gridView
        model: screenshotsModel
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        cellHeight: 150

        delegate: Column {
            Image {
                width: 200
                height: 100
                source: path
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Text {
                text: hash
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Text {
                text: percentage
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
