#include "licensewizard.h"

//! [0] //! [1] //! [2]
LicenseWizard::LicenseWizard(QWidget *parent)
    : QWizard(parent)
{
    item1 = new IntroPage;
    item2 = new RegisterPage;
    item3 = new ConclusionPage;

    setPage(Page_Intro, item1);
    setPage(Page_Register, item2);
    setPage(Page_Conclusion, item3);

    setStartId(Page_Intro);

#ifndef Q_OS_MAC
    setWizardStyle(ModernStyle);
#endif

    setOption(HaveHelpButton, false);

    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logoClient.png"));

    setWindowTitle(tr("Licencia"));
    setFixedSize(500, 400);
    setWindowFlags(Qt::Window);

    connect(item2->get_Key(), SIGNAL(textChanged(QString)), this, SLOT(set_Code(QString)));
}

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introducción"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/6.png"));

    topLabel = new QLabel(tr("Esta ventana te ayudará a registrar tu copia del "
                             "<i>Sistema de Gestión Comercial (SGC)</i>&trade; o iniciar "
                             "un período de evaluación del producto."));
    topLabel->setWordWrap(true);

    registerRadioButton = new QRadioButton(tr("&Registrar tu copia"));
    registerRadioButton->setChecked(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(registerRadioButton);
    setLayout(layout);
}
//! [16] //! [17]

//! [18]
int IntroPage::nextId() const {
    return LicenseWizard::Page_Register;
}

RegisterPage::RegisterPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Registrar tu copia de <i>Sistema de Gestión Comercial (SGC)</i>&trade;"));
    setSubTitle(tr("Si tienes la licencia, por favor complete en el campo apropiado."));

    nameLabel = new QLabel(tr("Nombre:"));
    nameLineEdit = new QLineEdit;
    nameLabel->setBuddy(nameLineEdit);

    upgradeKeyLabel = new QLabel(tr("Licencia:"));
    upgradeKeyLineEdit = new QLineEdit;
    upgradeKeyLabel->setBuddy(upgradeKeyLineEdit);

    registerField("register.name*", nameLineEdit);
    registerField("register.upgradeKey", upgradeKeyLineEdit);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(nameLineEdit, 0, 1);
    layout->addWidget(upgradeKeyLabel, 1, 0);
    layout->addWidget(upgradeKeyLineEdit, 1, 1);
    setLayout(layout);
}

//! [24]
int RegisterPage::nextId() const
{
    return LicenseWizard::Page_Conclusion;
}

ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Complete Su Registro"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/6.png"));

    bottomLabel = new QLabel;
    bottomLabel->setWordWrap(true);

    agreeCheckBox = new QCheckBox(tr("Estoy de acuerdo con los términos de licencia"));

    registerField("conclusion.agree*", agreeCheckBox);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(bottomLabel);
    layout->addWidget(agreeCheckBox);
    setLayout(layout);
}

//! [26]
int ConclusionPage::nextId() const
{
    return -1;
}
//! [26]

//! [27]
void ConclusionPage::initializePage()
{
    QString licenseText;

    licenseText = tr("<u>Acuerdo de Licencia de Programa:</u> "
                         "Este software se encuentra bajo los términos de la actual licencia.");
    bottomLabel->setText(licenseText);
}
//! [27]

//! [28]
void ConclusionPage::setVisible(bool visible)
{
    QWizardPage::setVisible(visible);

    if (visible) {
        wizard()->setButtonText(QWizard::CustomButton1, tr("&Print"));
        wizard()->setOption(QWizard::HaveCustomButton1, true);
        connect(wizard(), SIGNAL(customButtonClicked(int)),
                this, SLOT(printButtonClicked()));
    } else {
        wizard()->setOption(QWizard::HaveCustomButton1, false);
        disconnect(wizard(), SIGNAL(customButtonClicked(int)),
                   this, SLOT(printButtonClicked()));
    }
}
//! [28]

void ConclusionPage::printButtonClicked()
{
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec()){
        QPainter painter;
       if (painter.begin(&printer)) {
           painter.drawText(100, 100, bottomLabel->text());
           painter.end();
       }
    }
#endif
}
