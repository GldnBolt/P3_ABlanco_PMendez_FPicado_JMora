#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <memory>
#include "ControllerNode.h"

class ServerThread : public QThread {
    Q_OBJECT  // Necesario si usas señales y slots en Qt

private:
    std::shared_ptr<ControllerNode> controller;  // Guardar controller

public:
    // Declaración del constructor (solo la declaración aquí)
    ServerThread(std::shared_ptr<ControllerNode> ctrl);

    void run() override;  // Método que se ejecuta en el hilo
};

#endif // SERVERTHREAD_H
