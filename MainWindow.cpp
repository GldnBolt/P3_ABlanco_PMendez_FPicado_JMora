#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QPushButton>
#include <QFileInfo>
#include <QTextStream>
#include <QApplication>  // Asegúrate de incluir QApplication

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) // Correcta inicialización
{
    ui->setupUi(this);

    // Crear los clientes remotos para los discos (asegúrate de tener los puertos correctos)
    std::vector<std::shared_ptr<RemoteDiskClient>> disks;
    disks.push_back(std::make_shared<RemoteDiskClient>("localhost", 8081));  // Dirección de los discos
    disks.push_back(std::make_shared<RemoteDiskClient>("localhost", 8082));
    disks.push_back(std::make_shared<RemoteDiskClient>("localhost", 8083));
    disks.push_back(std::make_shared<RemoteDiskClient>("localhost", 8084));

    controller_ = std::make_shared<ControllerNode>("config/config.xml", disks);  // Asegúrate de que la ruta sea correcta

    // Conectar las señales de los botones a los slots
    connect(ui->btnAdd, &QPushButton::clicked, this, &MainWindow::onAddDocument);
    connect(ui->btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteDocument);
    connect(ui->btnList, &QPushButton::clicked, this, &MainWindow::onListDocuments);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onAddDocument() {
    // Mostrar un diálogo para seleccionar un archivo
    QString fileName = QFileDialog::getOpenFileName(this, "Selecciona un archivo", "", "Todos los archivos (*)");

    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Advertencia", "No se ha seleccionado un archivo.");
        return;
    }

    // Obtener el nombre del archivo (puedes pedirle al usuario que ingrese un nombre también)
    QString name = QFileInfo(fileName).fileName();  // Nombre del archivo seleccionado

    // Abrir el archivo y obtener su contenido
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo.");
        return;
    }
    QTextStream in(&file);
    QString content = in.readAll();  // Leer el contenido del archivo

    // Llamar al controlador para agregar el documento
    if (controller_->addDocument(name.toStdString(), content.toStdString())) {
        QMessageBox::information(this, "Éxito", "Documento agregado correctamente.");
    } else {
        QMessageBox::critical(this, "Error", "No se pudo agregar el documento.");
    }
}

void MainWindow::onDeleteDocument() {
    QString name = ui->lineSearch->text();
    if (controller_->deleteDocument(name.toStdString())) {
        QMessageBox::information(this, "Éxito", "Documento eliminado.");
    } else {
        QMessageBox::critical(this, "Error", "No se pudo eliminar el documento.");
    }
}

void MainWindow::onListDocuments() {
    auto documents = controller_->getDocuments();
    ui->listWidget->clear();
    for (const auto &doc : documents) {
        ui->listWidget->addItem(QString::fromStdString(doc));
    }
}
