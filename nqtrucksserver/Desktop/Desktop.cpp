#include "Desktop.h"
#include "ui_Desktop.h"
#include <QtDBus>
#include <QWindow>

namespace nQTrucks {


Desktop::Desktop(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Desktop)
  , m_running(false)
{
    ui->setupUi(this);

    /** KEYBOARD **/
    m_keyboard = new QProcess(this);
    m_keyboard->setProgram("/usr/bin/onboard");
    /** END KEYBOARD **/

    /** SYSTEM CONTROL **/
    m_reboot         = new QProcess(this);
    m_halt           = new QProcess(this);
    m_control_center = new QProcess(this);
    m_control_center->setProgram("/usr/bin/gnome-control-center");

    /** APLICACION **/
    m_app_engine    = new nQTrucksEngine(this);
    m_app_client    = new Client(m_app_engine);
    m_app_registros = new RegistrosUi(m_app_engine);
    m_app_config    = new Configuracion(m_app_engine);

    /** KEYBOARD SIEMPRE VIVO **/
    //connect(m_keyboard,SIGNAL(finished(int)),this,SLOT(on_exit_Keyboard(int)));
    //QDBusConnection::sessionBus().connect("org.onboard.Onboard","/org/onboard/Onboard/Keyboard","org.onboard.Onboard.Keyboard","PropertiesChanged",this,SLOT())

    /** Listado de StackWidgets **/
    //ui->appWidget->addWidget(m_app_config);
    //ui->appWidget->setVisible(false);


    /** DAEMON **/
    connect(ui->runningCheckBox,SIGNAL(toggled(bool)),this,SLOT(isRunning(bool)));
    connect(m_app_engine,SIGNAL(registrandoChanged(bool)),ui->runningCheckBox,SLOT(setDisabled(bool)));

    loadconfig();
}

Desktop::~Desktop(){
    m_keyboard->deleteLater();
    m_control_center->deleteLater();
    m_app_config->deleteLater();
    m_app_client->deleteLater();
    m_app_registros->deleteLater();
    m_app_engine->deleteLater();
    delete ui;
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

void Desktop::loadconfig(){

    /** INTERFACE **/
    m_running = m_app_engine->appConfig()->value(QString("Daemon") + "/running","0").toBool();

    ui->actionRegistros->setVisible(true);
    ui->actionConexiones->setVisible(false);
    ui->actionKeyboard->setVisible(false);

    ui->actionConfiguracion->setVisible(!m_running);
    ui->actionSystemSettings->setVisible(!m_running);
    ui->actionReiniciar->setVisible(!m_running);
    ui->actionClient->setVisible(m_running);

    //DEBUG NO BLOQUEAR ui->configTabWidget->setEnabled(!m_running);
    if (m_running){
        ui->runningCheckBox->setText("Running...");
    }else{
        ui->runningCheckBox->setText("Stoped...");
    }




   ui->runningCheckBox->setChecked(m_running);
}

/** HARDWARE **************************************************************/
/** KEYBOARD ***/
//void Desktop::on_exit_Keyboard(const int &arg1){
//    Q_UNUSED(arg1);
//    m_keyboard->start();
//}

void Desktop::on_actionKeyboard_toggled(bool arg1){
    if (arg1){
        m_keyboard->start();
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

    ui->appWidget->removeWidget(ui->appWidget->currentWidget());
    switch (m_app) {
    case appConfig:
        ui->actionClient->setChecked(false);
        ui->actionRegistros->setChecked(false);
        m_app_config->setMaximumSize(ui->appWidget->size());
        m_app_config->setSizePolicy(ui->appWidget->sizePolicy());
        ui->appWidget->addWidget(m_app_config);
        ui->appWidget->setCurrentWidget(m_app_config);
        break;
    case appClient:
        ui->actionRegistros->setChecked(false);
        ui->actionConfiguracion->setChecked(false);
        m_app_client->setMaximumSize(ui->appWidget->size());
        m_app_client->setSizePolicy(ui->appWidget->sizePolicy());
        ui->appWidget->addWidget(m_app_client);
        ui->appWidget->setCurrentWidget(m_app_client);
        break;
    case appRegistros:
        ui->actionConfiguracion->setChecked(false);
        ui->actionClient->setChecked(false);
        m_app_registros->setMaximumSize(ui->appWidget->size());
        m_app_registros->setSizePolicy(ui->appWidget->sizePolicy());
        ui->appWidget->addWidget(m_app_registros);
        ui->appWidget->setCurrentWidget(m_app_registros);
        break;

    case appNone:
        ui->actionClient->setChecked(false);
        ui->actionRegistros->setChecked(false);
        ui->actionConfiguracion->setChecked(false);
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

/** Registros **/
void Desktop::on_actionRegistros_toggled(bool arg1){
    if (arg1){
        m_app=appRegistros;
    }else{
        m_app=appNone;
   }
    on_selectedAppChanged();
}
/** END CLIENT **/


/** DAEMON **/
void Desktop::isRunning(bool clicked){
    ui->appWidget->removeWidget(ui->appWidget->currentWidget());
    m_running=clicked;

    ui->actionConfiguracion->setVisible( !m_running);
    ui->actionSystemSettings->setVisible(!m_running);
    ui->actionApagar->setVisible(        !m_running);
    ui->actionReiniciar->setVisible(     !m_running);
    ui->actionClient->setVisible(         m_running);

    m_app_engine->appConfig()->setValue(QString("Daemon") + "/running",m_running);
    m_app_engine->setInitDaemon(m_running);

    //DEBUG NO BLOQUEAR ui->configTabWidget->setEnabled(!m_running);
    if (m_running){
        ui->runningCheckBox->setText("Running...");
    }else{
        ui->runningCheckBox->setText("Stoped...");
    }
}

void Desktop::registrandoChanged(const bool &_registrando){

}


void Desktop::on_actionReiniciar_triggered(){
    m_reboot->startDetached("/usr/bin/sudo",QStringList() << "reboot" );
}

void Desktop::on_actionApagar_triggered(){
    m_halt->startDetached("/usr/bin/sudo",QStringList() << "poweroff" );
}


} /** END NAMESPACE **/
