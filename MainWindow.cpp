#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QFile>
#include <QTableWidgetItem>

MainWindow::MainWindow(std::shared_ptr<ControllerNode> controller, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      controller_(controller)    // <— inicializamos
{
    ui->setupUi(this);
    connect(ui->btnAdd,    &QPushButton::clicked, this, &MainWindow::onAddDocument);
    connect(ui->btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteDocument);
    connect(ui->btnList,   &QPushButton::clicked, this, &MainWindow::onListDocuments);
    connect(ui->btnDownload, &QPushButton::clicked, this, &MainWindow::onDownloadDocument);
    updateRaidStatus();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onAddDocument() {
    // Filtro directo a PDF
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

    QByteArray data = file.readAll();

    std::string content(data.constData(), data.size());

    try {
        controller_->addDocument(name.toStdString(), content);
        QMessageBox::information(this, "Éxito", "PDF agregado correctamente.");
        updateRaidStatus();
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
        updateRaidStatus();
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

void MainWindow::onDownloadDocument() {
    // Obtener el nombre del documento desde el campo de búsqueda (o lista seleccionada)
    QString docName = ui->lineSearch->text();

    // Si el nombre está vacío, mostramos un error
    if (docName.isEmpty()) {
        QMessageBox::warning(this, "Advertencia", "Por favor, ingrese el nombre del documento.");
        return;
    }

    // Seleccionar la ruta de destino para guardar el archivo
    QString filePath = QFileDialog::getSaveFileName(this, "Guardar Documento", docName, "PDF Files (*.pdf);;All Files (*)");
    if (filePath.isEmpty()) {
        return;  // Si el usuario cancela, salimos.
    }

    // Descargar el documento desde el ControllerNode
    try {
        // Asumimos que tienes un método para obtener el contenido del archivo
        std::string content = controller_->downloadDocument(docName.toStdString());

        // Guardar el contenido en el archivo seleccionado
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly)) {
            throw std::runtime_error("No se pudo abrir el archivo para escribir.");
        }

        file.write(content.c_str(), content.size());
        file.close();

        // Mensaje de éxito
        QMessageBox::information(this, "Éxito", "Documento descargado correctamente.");
    }
    catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", QString("No se pudo descargar el documento: %1").arg(e.what()));
    }
}


void MainWindow::updateRaidStatus() {
    auto status = controller_->getRaidStatus(); // [nodos][stripes]
    int numNodes = status.size();
    int numStripes = status[0].size();

    ui->raidStatusTable->setRowCount(numNodes);
    ui->raidStatusTable->setColumnCount(numStripes);

    // Encabezados opcionales:
    QStringList colHeaders;
    for (int stripe = 0; stripe < numStripes; ++stripe)
        colHeaders << QString("S%1").arg(stripe);
    ui->raidStatusTable->setHorizontalHeaderLabels(colHeaders);

    QStringList rowHeaders;
    for (int node = 0; node < numNodes; ++node)
        rowHeaders << QString("Nodo %1").arg(node);
    ui->raidStatusTable->setVerticalHeaderLabels(rowHeaders);

    for (int node = 0; node < numNodes; ++node) {
        for (int stripe = 0; stripe < numStripes; ++stripe) {
            QString text;
            if (status[node][stripe].isParity) text = "Parity";
            else if (status[node][stripe].present) text = "OK";
            else text = "Missing";
            QTableWidgetItem* item = new QTableWidgetItem(text);
            if (!status[node][stripe].present)
                item->setBackground(Qt::red);
            else if (status[node][stripe].isParity)
                item->setBackground(Qt::yellow);
            else
                item->setBackground(Qt::green);
            ui->raidStatusTable->setItem(node, stripe, item);
        }
    }
}

