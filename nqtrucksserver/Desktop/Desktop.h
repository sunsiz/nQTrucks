#ifndef DESKTOP_H
#define DESKTOP_H

#include <QMainWindow>
#include <QProcess>


/** APLICACION **/
//#include "QStackedWidget"
#include "nQTrucksEngine.h"
#include "Configuracion/Configuracion.h"
#include "Client/Client.h"
#include "RegistrosUi/RegistrosUi.h"


namespace Ui {
class Desktop;
}

namespace nQTrucks {
class Desktop : public QMainWindow
{
    Q_OBJECT

public:
    explicit Desktop(QWidget *parent = 0);
    ~Desktop();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Desktop *ui;
    void loadconfig();
    /** HARDWARE **/
private:
    QProcess *m_keyboard;
    QProcess *m_printers;
    QProcess *m_networks;
    QProcess *m_vinagre;
    QProcess *m_control_center;
    bool      m_running=false;

private slots:
    void on_exit_Keyboard(const int &arg1);
    void on_actionKeyboard_toggled(bool arg1);
    void on_actionSystemSettings_triggered();
    void isRunning(bool clicked);

    /** APLICACION **/
private:
    nQTrucks::nQTrucksEngine *m_app_engine;
    Configuracion *m_app_config;
    Client  *m_app_client;
    RegistrosUi *m_app_registros;
    int m_app=0;
public:
    enum SelecteAPP{
        appNone =0,
        appConfig,
        appClient,
        appRegistros,
    };

private slots:
    void on_selectedAppChanged();
    void on_actionConfiguracion_toggled(bool arg1);
    //void on_actionConfiguracion_triggered();
    //void on_actionClient_triggered();
    void on_actionClient_toggled(bool arg1);
    void on_actionRegistros_toggled(bool arg1);
};

}
#endif // DESKTOP_H
