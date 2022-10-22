#ifndef LICENSE_H
#define LICENSE_H

#include <QString>
#include <QDate>

class License
{
public:
    License(QString code, QDate date);
    License();

    QString get_Code() { return this->code; }
    void set_Code(QString code) { this->code = code; }

    QDate get_Date() { return this->date; }
    void set_Date(QDate date) { this->date = date; }

    bool equals(License *);

private:
    QString code;
    QDate date;
};

#endif // LICENSE_H
