#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtWidgets/QMainWindow>  // Asegúrate de incluir QMainWindow
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QApplication>  // Incluir QApplication

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *btnAdd;
    QPushButton *btnDelete;
    QPushButton *btnList;
    QLineEdit *lineSearch;
    QListWidget *listWidget;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(400, 300);

        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        MainWindow->setCentralWidget(centralwidget);

        btnAdd = new QPushButton(centralwidget);
        btnAdd->setObjectName(QString::fromUtf8("btnAdd"));
        btnAdd->setText("Agregar Documento");
        btnAdd->setGeometry(QRect(10, 10, 200, 30));

        btnDelete = new QPushButton(centralwidget);
        btnDelete->setObjectName(QString::fromUtf8("btnDelete"));
        btnDelete->setText("Eliminar Documento");
        btnDelete->setGeometry(QRect(10, 50, 200, 30));

        btnList = new QPushButton(centralwidget);
        btnList->setObjectName(QString::fromUtf8("btnList"));
        btnList->setText("Listar Documentos");
        btnList->setGeometry(QRect(10, 90, 200, 30));

        lineSearch = new QLineEdit(centralwidget);
        lineSearch->setObjectName(QString::fromUtf8("lineSearch"));
        lineSearch->setPlaceholderText("Buscar Documento...");
        lineSearch->setGeometry(QRect(10, 130, 200, 30));

        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(10, 170, 380, 100));
    }

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Documentos", nullptr));
    }
};

namespace Ui {
class MainWindow : public Ui_MainWindow {};
}  // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
