#include "Desktop.h"
#include "ui_Desktop.h"
#include <QtDBus>
#include <QWindow>

namespace nQTrucks {


Desktop::Desktop(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Desktop)
{
    ui->setupUi(this);

    /** KEYBOARD **/
    m_keyboard = new QProcess(this);
    m_keyboard->setProgram("/usr/bin/onboard");
    m_keyboard->start();
    /** END KEYBOARD **/

    m_control_center = new QProcess(this);
    m_control_center->setProgram("/usr/bin/gnome-control-center");

    /** APLICACION **/
    m_app_engine = new nQTrucksEngine();
    m_app_config = new Configuracion(m_app_engine);
    m_app_client = new Client(m_app_engine);

    /** KEYBOARD SIEMPRE VIVO **/
    connect(m_keyboard,SIGNAL(finished(int)),this,SLOT(on_exit_Keyboard(int)));
    //QDBusConnection::sessionBus().connect("org.onboard.Onboard","/org/onboard/Onboard/Keyboard","org.onboard.Onboard.Keyboard","PropertiesChanged",this,SLOT())

    /** Listado de StackWidgets **/
    //ui->appWidget->addWidget(m_app_config);
    //ui->appWidget->setVisible(false);
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
void Desktop::on_exit_Keyboard(const int &arg1){
    Q_UNUSED(arg1);
    m_keyboard->start();
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
   on_selectedAppChanged();

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
/** APPLICACION ***************************************************************/
void Desktop::on_selectedAppChanged(){

    switch (m_app) {
    case appConfig:
        ui->appWidget->removeWidget(m_app_config);
        m_app_config->setMaximumSize(ui->appWidget->size());
        m_app_config->setSizePolicy(ui->appWidget->sizePolicy());
        ui->appWidget->addWidget(m_app_config);
        ui->appWidget->setCurrentWidget(m_app_config);
        break;
    case appClient:
        ui->appWidget->removeWidget(m_app_client);
        m_app_client->setMaximumSize(ui->appWidget->size());
        m_app_client->setSizePolicy(ui->appWidget->sizePolicy());
        ui->appWidget->addWidget(m_app_client);
        ui->appWidget->setCurrentWidget(m_app_client);
        break;
    case appNone:
        ui->appWidget->removeWidget(m_app_config);
        ui->appWidget->removeWidget(m_app_client);
        break;
    }
}


/** CONFIGURACION APP **/
void Desktop::on_actionConfiguracion_toggled(bool arg1){
    if (arg1){
        m_app=appConfig;
    }else{
        m_app=appNone;
    }
    on_selectedAppChanged();
}
/** END CONFIGURACION APP **/

/** CLIENT **/
void Desktop::on_actionClient_toggled(bool arg1){
    if (arg1){
        m_app=appClient;
    }else{
        m_app=appNone;
   }
    on_selectedAppChanged();
}
/** END CLIENT **/

}
