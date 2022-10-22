#ifndef LICENSEWIZARD_H
#define LICENSEWIZARD_H

#include <QWizard>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QLabel;
class QLineEdit;
class QRadioButton;
QT_END_NAMESPACE

#include <QtWidgets>

class IntroPage;
class RegisterPage;
class ConclusionPage;

class LicenseWizard : public QWizard
{
    Q_OBJECT
public:
    enum { Page_Intro, Page_Register, Page_Conclusion };
    LicenseWizard(QWidget *parent = 0);

    QString get_Code() { return this->code; }

public slots:
    void set_Code(QString code) { this->code = code; }

private:
    IntroPage *item1;
    RegisterPage *item2;
    ConclusionPage *item3;

    QString code;
};

class IntroPage : public QWizardPage
{
    Q_OBJECT
public:
    IntroPage(QWidget *parent = 0);

    int nextId() const;

private:
    QLabel *topLabel;
    QRadioButton *registerRadioButton;
};

class RegisterPage : public QWizardPage
{
    Q_OBJECT
public:
    RegisterPage(QWidget *parent = 0);

    int nextId() const;

    QLineEdit *get_Key() { return this->upgradeKeyLineEdit; }

private:
    QLabel *nameLabel;
    QLabel *upgradeKeyLabel;
    QLineEdit *nameLineEdit;
    QLineEdit *upgradeKeyLineEdit;
};

#include <QPrinter>
#include <QPrintDialog>

class ConclusionPage : public QWizardPage
{
    Q_OBJECT
public:
    ConclusionPage(QWidget *parent = 0);

    void initializePage();
    int nextId() const;
    void setVisible(bool visible);

private slots:
    void printButtonClicked(); 

private:
    QLabel *bottomLabel;
    QCheckBox *agreeCheckBox;
};
//! [6]

#endif
