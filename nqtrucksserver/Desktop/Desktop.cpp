#include "Desktop.h"
#include "ui_Desktop.h"
#include <QtDBus>
#include <QWindow>


Desktop::Desktop(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Desktop)
{
    ui->setupUi(this);
    m_keyboard = new QProcess(this);
    m_keyboard->setProgram("/usr/bin/onboard");
    m_keyboard->start();

//    m_keyboard = new QProcess();
//    m_keyboard->setProgram("/usr/bin/onboard");
//    m_keyboard->start();

//    m_control_center = new QProcess(this);
//    m_control_center->setProgram("/usr/bin/gnome-control-center");

//    m_printers = new QProcess(this);
//    m_printers->setProgram("/usr/bin/system-config-printer");

//    m_networks= new QProcess(this);
//    m_networks->setProgram("/usr/bin/nm-connection-editor");

//    m_vinagre = new QProcess(this);
//    m_vinagre->setProgram("/usr/bin/vino-preferences");

}

Desktop::~Desktop()
{
    delete ui;
}

void Desktop::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Desktop::on_actionKeyboard_toggled(bool arg1)
{
    if (m_keyboard == nullptr) {
        m_keyboard = new QProcess(this);
        m_keyboard->setProgram("/usr/bin/onboard");
        m_keyboard->start();
    }
    QDBusConnection dbus =QDBusConnection::sessionBus();
    QDBusMessage msg = QDBusMessage::createMethodCall("org.onboard.Onboard", "/org/onboard/Onboard/Keyboard" , "org.onboard.Onboard.Keyboard","ToggleVisible");
    dbus.send(msg);
}

void Desktop::on_actionImpresoras_triggered()
{
    if (m_printers == nullptr) {
        m_printers = new QProcess(this);
        m_printers->setProgram("/usr/bin/system-config-printer");
        m_printers->start();
    }else{
        if (m_printers->Running){
            m_printers->close();
        }
        m_printers->setProgram("/usr/bin/system-config-printer");
        m_printers->start();
    }
}

void Desktop::on_actionConexiones_triggered()
{
    if (m_networks == nullptr) {
        m_networks= new QProcess(this);
    }else{
        if (m_networks->Running){
            m_networks->close();
        }
        m_networks->setProgram("/usr/bin/nm-connection-editor");
        m_networks->start();
    }
}

void Desktop::on_actionRemoto_triggered()
{
    if (m_vinagre == nullptr) {
        m_vinagre= new QProcess(this);
        m_vinagre->setProgram("/usr/bin/vino-preferences");
        m_vinagre->start();
    }else{
        if (m_vinagre->Running){
            m_vinagre->close();
        }
        m_vinagre->setProgram("/usr/bin/vino-preferences");
        m_vinagre->start();
    }
}

void Desktop::on_actionSystemSettings_triggered()
{
    if (m_control_center == nullptr) {
        m_control_center = new QProcess(this);
        m_control_center->setProgram("/usr/bin/gnome-control-center");
        m_control_center->start();
    }else{
        if (m_control_center->Running){
            m_control_center->close();
        }
        m_control_center->setProgram("/usr/bin/gnome-control-center");
        m_control_center->start();
    }
}
