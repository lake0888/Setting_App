#include "settings.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <QMessageBox>

#include "licensewizard.h"

Settings::Settings(QWidget *parent) : QDialog(parent)
{
    QGroupBox *groupBox = new QGroupBox("Setting");
    QLabel *labelHost = new QLabel("Host:");
    QLabel *labelDB = new QLabel("Database:");
    QLabel *labelPort = new QLabel("Port:");
    QLabel *labelUser = new QLabel("User:");
    QLabel *labelPassWord = new QLabel("Password:");

    QVBoxLayout *vBox = new QVBoxLayout;
    vBox->addWidget(labelHost);
    vBox->addWidget(labelDB);
    vBox->addWidget(labelPort);
    vBox->addWidget(labelUser);
    vBox->addWidget(labelPassWord);

    editHostname = new QLineEdit;
    editDataBaseName = new QLineEdit;
    editUsername = new QLineEdit;
    editPassword = new QLineEdit;    
    editPassword->setEchoMode(QLineEdit::Password);

    spinPort = new QSpinBox;
    spinPort->setMinimum(0);
    spinPort->setMaximum(10000);

    QVBoxLayout *vBox1 = new QVBoxLayout;
    vBox1->addWidget(editHostname);
    vBox1->addWidget(editDataBaseName);
    vBox1->addWidget(spinPort);
    vBox1->addWidget(editUsername);
    vBox1->addWidget(editPassword);

    QHBoxLayout *hBox = new QHBoxLayout;
    hBox->addLayout(vBox);
    hBox->addLayout(vBox1);

    groupBox->setLayout(hBox);

    updateButton = new QPushButton("Update");
    connect(updateButton, SIGNAL(clicked()), this, SLOT(slotUpdate()));

    QHBoxLayout *hBox1 = new QHBoxLayout;
    hBox1->addWidget(updateButton, 0, Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(groupBox);
    mainLayout->addLayout(hBox1);

    setLayout(mainLayout);
    setWindowTitle("Config SGC");
    setFixedSize(250, 220);
    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    setWindowIcon(QIcon(":/images/logo.ico"));

    actionLicense = new QAction(tr("Estado de Licencia"),this);
    actionLicense->setStatusTip(tr("Estado de Licencia"));
    connect(actionLicense, SIGNAL(triggered()), this, SLOT(showLicense()));

    actionRLicense = new QAction(tr("Registro de Licencia"),this);
    actionRLicense->setStatusTip(tr("Registro de Licencia"));
    connect(actionRLicense, SIGNAL(triggered()), this, SLOT(insertLicense()));

    actionClose = new QAction(tr("Salir"),this);
    actionClose->setStatusTip(tr("Salir"));
    actionClose->setIcon(QIcon(":/images/exit.png"));
    connect(actionClose, SIGNAL(triggered()), this, SLOT(close()));

    /*******SERVICE*************/
    actionStartService = new QAction(tr("Iniciar servicio"),this);
    actionStartService->setStatusTip(tr("Iniciar servicio"));
    actionStartService->setIcon(QIcon(":/images/start.png"));
    connect(actionStartService, SIGNAL(triggered()), this, SLOT(startService()));

    actionStopService = new QAction(tr("Detener servicio"),this);
    actionStopService->setStatusTip(tr("Detener servicio"));
    actionStopService->setIcon(QIcon(":/images/stop.png"));
    connect(actionStopService, SIGNAL(triggered()), this, SLOT(stopService()));

    actionPauseService = new QAction(tr("Pausar servicio"),this);
    actionPauseService->setStatusTip(tr("Pausar servicio"));
    actionPauseService->setIcon(QIcon(":/images/pause.png"));
    connect(actionPauseService, SIGNAL(triggered()), this, SLOT(pauseService()));

    actionResumeService = new QAction(tr("Continuar servicio"),this);
    actionResumeService->setStatusTip(tr("Continuar servicio"));
    actionResumeService->setIcon(QIcon(":/images/start.png"));
    connect(actionResumeService, SIGNAL(triggered()), this, SLOT(resumeService()));

    this->process = new QProcess(this);
    this->process->setProcessChannelMode(QProcess::ForwardedChannels);

    this->controller = new QtServiceController("Setting_App_SC");

    //SERVICE

    editHostSC = new QLineEdit;
    editHostSC->setPlaceholderText("Host");
    editHostSC->setFixedWidth(120);

    editPortSC = new QLineEdit;
    editPortSC->setPlaceholderText("Port");
    editPortSC->setFixedWidth(50);

    buttonSC = new QPushButton("Connect");
    connect(buttonSC, SIGNAL(clicked()), this, SLOT(connectToService()));

    QHBoxLayout *hBox2 = new QHBoxLayout;
    hBox2->addWidget(editHostSC);
    hBox2->addWidget(editPortSC);
    hBox2->addWidget(buttonSC);

    boxService = new QGroupBox("Service Setting_App_SC");
    boxService->setLayout(hBox2);
    boxService->show();
    boxService->setFixedWidth(280);

    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(initDB()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));

    createTryIcon();

    bool flagValidate = validateLicense(currentLicense);
    if(flagValidate) slotUpdate();

    if(QSqlDatabase::isDriverAvailable("QPSQL"))
        db = QSqlDatabase::addDatabase("QPSQL");

    startService();

    loadSettings();
    createLicenses();
    updateButton->setEnabled(flagValidate);
}

Settings::~Settings(){}

void Settings::saveSettings(){
    QSettings settings("SGC", "Setting");

    settings.beginGroup("Database");
    settings.setValue("hostName", editHostname->text());
    settings.setValue("dbName", editDataBaseName->text());
    settings.setValue("port", spinPort->value());
    settings.setValue("userName", editUsername->text());
    settings.setValue("pswdName", editPassword->text());
    settings.endGroup();

    settings.beginGroup("License");
    settings.setValue("code", currentLicense->get_Code());
    settings.setValue("date", currentLicense->get_Date());
    settings.endGroup();

    settings.beginGroup("Setting_App_SC");
    settings.setValue("host", editHostSC->text());
    settings.setValue("port", editPortSC->text());
    settings.endGroup();
}

void Settings::loadSettings(){
    QSettings settings("SGC", "Setting");

    settings.beginGroup("Database");
    QString hostName = settings.value("hostName").toString();
    QString dbName = settings.value("dbName").toString();
    int port = settings.value("port").toInt();
    QString userName = settings.value("userName").toString();
    QString pswdName = settings.value("pswdName").toString();
    settings.endGroup();

    editHostname->setText(hostName);
    editDataBaseName->setText(dbName);
    spinPort->setValue(port);
    editUsername->setText(userName);
    editPassword->setText(pswdName);

    settings.beginGroup("License");
    QString code = settings.value("code").toString();
    QDate date = settings.value("date").toDate();
    settings.endGroup();

    currentLicense = new License(code, date);

    settings.beginGroup("Setting_App_SC");
    QString host = settings.value("host").toString();
    port = settings.value("port").toInt();
    settings.endGroup();

    editHostSC->setText(host);
    editPortSC->setText(QString::number(port));
}

void Settings::closeEvent(QCloseEvent *event){
    saveSettings();
    messages();
    event->accept();
}

void Settings::createTrayIconMenu()
{
    this->trayIconMenu = new QMenu(this);
    this->trayIconMenu->addAction(actionLicense);
    this->trayIconMenu->addAction(actionRLicense);

    this->trayIconMenu->addSeparator();

    this->trayIconMenu->addAction(actionStartService);
    this->trayIconMenu->addAction(actionStopService);
    this->trayIconMenu->addAction(actionPauseService);
    this->trayIconMenu->addAction(actionResumeService);

    this->trayIconMenu->addSeparator();

    this->trayIconMenu->addAction(actionClose);
}

void Settings::createTryIcon(){
    createTrayIconMenu();
    this->trayIcon = new QSystemTrayIcon(QIcon(":/images/logo.png"));
    this->trayIcon->setContextMenu(this->trayIconMenu);
    this->trayIcon->setToolTip("Config SGC");

    this->trayIcon->setVisible(true);
    connect(this->trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(activatedReason(QSystemTrayIcon::ActivationReason)));

    messages();
}

void Settings::activatedReason(QSystemTrayIcon::ActivationReason reason){
    switch (reason){
        case QSystemTrayIcon::DoubleClick :
            this->setVisible(!this->isVisible());
        this->boxService->show();
            break;
        default:
            break;
    }
}

void Settings::displayError(QAbstractSocket::SocketError socketError){
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, tr("SGC"),
                                     tr("No se encontró la Dirección Ip. Por favor, comprueba la "
                                        "configuración de la Dirección Ip y del puerto."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, tr("SGC"),
                                     tr("La conexión fue rechazada. "
                                        "Asegúrese de que el servidor se está ejecutando, "
                                        "y compruebe la configuración de la Dirección Ip y del puerto."));
            break;
        default:
            QMessageBox::information(this, tr("SGC"), "Imposile establecer conexión con el servidor");
        }
}

void Settings::connectToService(){
    QString hostname = editHostSC->text();
    QString port = editPortSC->text();

    blockSize = 0;
    tcpSocket->abort();
    tcpSocket->connectToHost(hostname, port.toInt());

    // we need to wait...
    if(!tcpSocket->waitForConnected(5000))
    {
         QMessageBox::information(this, tr("SGC"), tcpSocket->errorString());
    }else{
        QString str = "Conectandose al servicio";
        this->trayIcon->showMessage("Config SGC", str, QSystemTrayIcon::Information, 10 * 1000);
    }
}

void Settings::slotUpdate(){
    if(tcpSocket->state() == QAbstractSocket::ConnectedState){
        //SEND DATA TO SERVICE

        QString hostname = editHostname->text();
        QString databasename = editDataBaseName->text();
        int port = spinPort->value();
        QString username = editUsername->text();
        QString password = editPassword->text();

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_1);

        out << (quint16)0;

        QString type = "Setting_App";
        out << type;

        out << hostname << port << databasename << username << password;

        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));

        tcpSocket->write(block);
        tcpSocket->flush();

        QString str = "Enviando datos al servicio";
        this->trayIcon->showMessage("Config SGC", str, QSystemTrayIcon::Information, 10 * 1000);
    }else{
        QMessageBox::information(this, "Config SGC", "No hay conexion con el servicio.");
    }
}

void Settings::initDB(){
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_1);

    if(blockSize == 0){
        if(tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> blockSize;
    }
    if(tcpSocket->bytesAvailable() < blockSize)
        return;


    in >> hostname >> port >> database >> username >> password;

    db.setHostName(hostname);
    db.setPort(port);
    db.setDatabaseName(database);
    db.setUserName(username);
    db.setPassword(password);

    messages();
}

void Settings::messages(){
    bool flag = this->controller->isRunning();
    QString str = "";
    if(!flag) str = "El servicio Setting_App_SC no esta iniciado";
    else{
        if(!db.open())
            str = "The server is not listening";
        else
            str = "The server is running on\nIP: " + hostname +
                    "\nport: " + QString::number(port);
    }
    this->trayIcon->showMessage("Config SGC", str, QSystemTrayIcon::Information, 10 * 1000);
}

void Settings::createLicenses(){
    License *one = new License("L21*FI$", QDate(2021, 12, 31));
    License *two = new License("L22*FI$", QDate(2022, 12, 31));
    License *three = new License("L23*FI$", QDate(2023, 12, 31));


    licenses = new QList<License *>();
    licenses->append(one);
    licenses->append(two);
    licenses->append(three);
}

bool Settings::validateLicense(){
    int i = 0;
    while (i < licenses->length()) {
        License *license = licenses->at(i++);
        if(license->equals(currentLicense))
            return true;
    }
    return false;
}

bool Settings::validateLicense(License *license){
    return (QDate::currentDate() <= license->get_Date());
}

int Settings::existCodeLicense(QString code){
    int i = 0;
    while (i < licenses->length()) {
        License *license = licenses->at(i);
        if(license->get_Code().toUpper().trimmed() == code.toUpper().trimmed())
            return i;
        i++;
    }
    return -1;
}

void Settings::showLicense(){
    if(validateLicense()){
        QString code = currentLicense->get_Code();
        QString date = currentLicense->get_Date().toString("d/M/yyyy");

        QMessageBox::information(this, "LICENCIA COMERCIAL", "Licencia:" + code + "\n\n" +
                                        "valida hasta " + date);
    }else QMessageBox::information(this, "LICENCIA COMERCIAL", "Producto no validado.");
}

void Settings::insertLicense(){
    LicenseWizard *licenseWizard = new LicenseWizard();
    if(licenseWizard->exec()){
        QString code = licenseWizard->get_Code();
        License *license = new License();
        int pos = existCodeLicense(code);
        if(pos != -1){
            license->set_Code(licenses->at(pos)->get_Code());
            license->set_Date(licenses->at(pos)->get_Date());

            currentLicense->set_Code(license->get_Code());
            currentLicense->set_Date(license->get_Date());
            bool flag = validateLicense(currentLicense);
            if(flag){
                saveSettings();
                slotUpdate();
            }else{ controller->stop(); }
            updateButton->setEnabled(flag);
        }else { QMessageBox::information(this, "LICENCIA COMERCIAL", "Código Incorrecto."); }
    }
}

/*SERVICE*/
void Settings::startService(){
    process->start("SC start Setting_App_SC", QIODevice::ReadWrite);

    // Wait for it to start
    if(!process->waitForStarted())
        return;

    QByteArray buffer;
    while(process->waitForFinished())
        buffer.append(process->readAll());

    QString str = buffer.data();

    if(str.isEmpty() && this->controller->isRunning()) str = "Servicio iniciado";
    this->trayIcon->showMessage("Config SGC", str, QSystemTrayIcon::Information, 10 * 1000);

    bool flag = this->controller->start();

    actionStartService->setVisible(!flag);
    actionStopService->setVisible(flag);
    actionPauseService->setVisible(flag);
    actionResumeService->setVisible(flag);
}

void Settings::stopService(){
    process->start("SC stop Setting_App_SC", QIODevice::ReadWrite);

    // Wait for it to start
    if(!process->waitForStarted())
        return;

    QByteArray buffer;
    while(process->waitForFinished())
        buffer.append(process->readAll());

    QString str = buffer.data();

    if(str.isEmpty() && this->controller->stop()) str = "Servicio parado";
    this->trayIcon->showMessage("Config SGC", str, QSystemTrayIcon::Information, 10 * 1000);

    bool flag = this->controller->stop();

    actionStartService->setVisible(flag);
    actionStopService->setVisible(!flag);
    actionPauseService->setVisible(!flag);
    actionResumeService->setVisible(!flag);
}

void Settings::pauseService(){
    process->start("SC pause Setting_App_SC", QIODevice::ReadWrite);

    // Wait for it to start
    if(!process->waitForStarted())
        return;

    QByteArray buffer;
    while(process->waitForFinished())
        buffer.append(process->readAll());

    QString str = buffer.data();

    if(str.isEmpty() && this->controller->pause()) str = "Servicio pausado";
    this->trayIcon->showMessage("Config SGC", str, QSystemTrayIcon::Information, 10 * 1000);

    bool flag = this->controller->pause();

    actionStartService->setVisible(!flag);
    actionStopService->setVisible(flag);
    actionPauseService->setVisible(!flag);
    actionResumeService->setVisible(flag);
}

void Settings::resumeService(){
    process->start("SC continue Setting_App_SC", QIODevice::ReadWrite);

    // Wait for it to start
    if(!process->waitForStarted())
        return;

    QByteArray buffer;
    while(process->waitForFinished())
        buffer.append(process->readAll());

    QString str = buffer.data();

    if(str.isEmpty() && this->controller->resume()) str = "Servicio resumido";
    this->trayIcon->showMessage("Config SGC", str, QSystemTrayIcon::Information, 10 * 1000);

    bool flag = this->controller->resume();

    actionStartService->setVisible(!flag);
    actionStopService->setVisible(flag);
    actionPauseService->setVisible(flag);
    actionResumeService->setVisible(!flag);
}
