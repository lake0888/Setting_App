#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QSystemTrayIcon>
#include <QCloseEvent>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSettings>

#include <QMenu>
#include <QAction>

#include "license.h"
#include <QtService>
#include <QProcess>
#include <QTcpSocket>
#include <QGroupBox>

class Settings : public QDialog
{
    Q_OBJECT
public:
    Settings(QWidget *parent = 0);
    ~Settings();

    void messages();

    void saveSettings();
    void loadSettings();

    bool validateLicense();
    bool validateLicense(License *);
    int existCodeLicense(QString code);

protected:
    void closeEvent(QCloseEvent *);    

public slots:
    void activatedReason(QSystemTrayIcon::ActivationReason);

    void slotUpdate();

    void showLicense();
    void insertLicense();

    /*SERVICE*/
    void connectToService();

    void startService();
    void stopService();
    void pauseService();
    void resumeService();

    void initDB();
    void displayError(QAbstractSocket::SocketError socketError);

private:
    QLineEdit *editHostname, *editDataBaseName, *editUsername, *editPassword;
    QSpinBox *spinPort;
    QPushButton *updateButton, *closeButton;

    //QSystemTrayIcon
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction *actionLicense, *actionRLicense, *actionClose;
    QAction *actionStartService, *actionStopService, *actionPauseService, *actionResumeService;

    void createTrayIconMenu();
    void createTryIcon();

    QList<License *> *licenses;
    License *currentLicense;

    void createLicenses();
    QtServiceController *controller;
    QProcess *process;

    QTcpSocket *tcpSocket;
    quint16 blockSize;
    QLineEdit *editHostSC, *editPortSC;
    QPushButton *buttonSC;
    QGroupBox *boxService;

    int port;
    QString hostname, database, username, password;

    QSqlDatabase db;
};

#endif // SETTINGS_H
