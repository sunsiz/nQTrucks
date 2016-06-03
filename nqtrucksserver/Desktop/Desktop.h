#ifndef DESKTOP_H
#define DESKTOP_H

#include <QMainWindow>
#include <QProcess>


/** APLICACION **/
#include "nQTrucksEngine.h"
#include "Configuracion/Configuracion.h"

namespace Ui {
class Desktop;
}

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
    /** HARDWARE **/
private:
    QProcess *m_keyboard;
    QProcess *m_printers;
    QProcess *m_networks;
    QProcess *m_vinagre;
    QProcess *m_control_center;

private slots:
    void on_Keyboard_exit(int arg1);
    void on_actionKeyboard_toggled(bool arg1);
    //void on_actionImpresoras_triggered();
    //void on_actionConexiones_triggered();
    //void on_actionRemoto_triggered();
    void on_actionSystemSettings_triggered();

    /** APLICACION **/
private:
    nQTrucks::nQTrucksEngine *m_app_engine;
    Configuracion *m_app_config;
private slots:
    void on_actionConfiguracion_toggled(bool arg1);
};

#endif // DESKTOP_H
