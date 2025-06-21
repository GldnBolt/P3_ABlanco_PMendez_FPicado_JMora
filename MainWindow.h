#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "ControllerNode.h"
#include "RemoteDiskClient.h"
#include "HttpServer.h"
#include "ui_mainwindow.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    private slots:
        void onAddDocument();
    void onDeleteDocument();
    void onListDocuments();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<ControllerNode> controller_;  // Declaración de controller_

    void updateDocumentList();
};

#endif // MAINWINDOW_H
