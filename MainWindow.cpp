#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QFile>

MainWindow::MainWindow(std::shared_ptr<ControllerNode> controller, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      controller_(controller)    // <— inicializamos
{
    ui->setupUi(this);
    connect(ui->btnAdd,    &QPushButton::clicked, this, &MainWindow::onAddDocument);
    connect(ui->btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteDocument);
    connect(ui->btnList,   &QPushButton::clicked, this, &MainWindow::onListDocuments);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onAddDocument() {
    // Filtro directo a PDF (puedes ajustarlo o quitarlo)
    QString fileName = QFileDialog::getOpenFileName(this,
        "Selecciona un PDF", QString(), "PDF Files (*.pdf);;Todos los archivos (*)");
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Advertencia", "No se ha seleccionado ningún archivo.");
        return;
    }

    QString name = QFileInfo(fileName).fileName();

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo.");
        return;
    }
    // **Leer todo como binario**, no con QTextStream
    QByteArray data = file.readAll();
    // Construimos std::string conservando bytes null:
    std::string content(data.constData(), data.size());

    try {
        controller_->addDocument(name.toStdString(), content);
        QMessageBox::information(this, "Éxito", "PDF agregado correctamente.");
    }
    catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", QString("No se pudo agregar: %1").arg(e.what()));
    }
}

void MainWindow::onDeleteDocument() {
    QString name = ui->lineSearch->text();
    try {
        controller_->deleteDocument(name.toStdString());
        QMessageBox::information(this, "Éxito", "Documento eliminado.");
    }
    catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", QString("No se pudo eliminar: %1").arg(e.what()));
    }
}

void MainWindow::onListDocuments() {
    ui->listWidget->clear();
    auto docs = controller_->getDocuments();
    for (auto &d : docs) {
        ui->listWidget->addItem(QString::fromStdString(d));
    }
}
