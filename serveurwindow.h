#ifndef SERVEURWINDOW_H
#define SERVEURWINDOW_H

#include <QMainWindow>
#include "serveur.h"

class serveurWindow : QMainWindow
{
public:
    serveurWindow();

private:
    serveur*  server;
};

#endif // SERVEURWINDOW_H
