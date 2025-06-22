#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "ControllerNode.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(std::shared_ptr<ControllerNode> ctrl, QWidget *parent = nullptr);
    ~MainWindow();

    private slots:
        void onAddDocument();
        void onDeleteDocument();
        void onListDocuments();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<ControllerNode> controller_;
};

#endif // MAINWINDOW_H
