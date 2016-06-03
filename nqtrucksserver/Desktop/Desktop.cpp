#include "Desktop.h"
#include "ui_Desktop.h"
#include <QtDBus>
#include <QWindow>

Desktop::Desktop(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Desktop)
{
    ui->setupUi(this);

    /** KEYBOARD **/
    m_keyboard = new QProcess(this);
    m_keyboard->setProgram("/usr/bin/onboard");
    m_keyboard->start();
    /** KEYBOARD SIEMPRE VIVO **/
    connect(m_keyboard,SIGNAL(finished(int)),this,SLOT(on_Keyboard_exit(int)));
    /** END KEYBOARD **/

    m_control_center = new QProcess(this);
    m_control_center->setProgram("/usr/bin/gnome-control-center");

    /** APLICACION **/
    m_app_engine = new nQTrucks::nQTrucksEngine();
    m_app_config = new Configuracion(m_app_engine);

}

Desktop::~Desktop(){
    delete ui;
    m_keyboard->deleteLater();
    m_control_center->deleteLater();
    m_app_engine->deleteLater();
    m_app_config->deleteLater();
}

void Desktop::changeEvent(QEvent *e){
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/** HARDWARE **************************************************************/
/** KEYBOARD ***/
void Desktop::on_Keyboard_exit(int arg1){
    m_keyboard->start();
//    QDBusConnection dbus =QDBusConnection::sessionBus();
//    QDBusMessage msg = QDBusMessage::createMethodCall("org.onboard.Onboard", "/org/onboard/Onboard/Keyboard" , "org.onboard.Onboard.Keyboard","ToggleVisible");
//    dbus.send(msg);
}

void Desktop::on_actionKeyboard_toggled(bool arg1){
    if (arg1){
        QDBusConnection dbus =QDBusConnection::sessionBus();
        QDBusMessage msg = QDBusMessage::createMethodCall("org.onboard.Onboard", "/org/onboard/Onboard/Keyboard" , "org.onboard.Onboard.Keyboard","Show");
        dbus.send(msg);
    }else{
        QDBusConnection dbus =QDBusConnection::sessionBus();
        QDBusMessage msg = QDBusMessage::createMethodCall("org.onboard.Onboard", "/org/onboard/Onboard/Keyboard" , "org.onboard.Onboard.Keyboard","Hide");
        dbus.send(msg);
    }
}
/** END KEYBOARD **/
/** SYSTEM SETTINGS **/
void Desktop::on_actionSystemSettings_triggered(){
    if (m_control_center->Running){
            m_control_center->close();
    }
    m_control_center->start();
}
/** END SYSTEM SETTINGS **/
/** END HARDWARE **********************************************************************************/


/** APPLICAION ***************************************************************/
/** CONFIGURACION APP **/
void Desktop::on_actionConfiguracion_toggled(bool arg1){
    if (arg1){
         ui->stackedWidget->addWidget(m_app_config);
         m_app_config->setMaximumSize(ui->stackedWidget->size());
         ui->stackedWidget->setCurrentWidget(m_app_config);
    }else{
        ui->stackedWidget->removeWidget(m_app_config);
    }
}
/** END CONFIGURACION APP **/

