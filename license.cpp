#include "license.h"

License::License(QString code, QDate date)
{
    this->code = code;
    this->date = date;
}

License::License()
{
    this->code = "";
    this->date = QDate::currentDate();
}

bool License::equals(License *tmp){
    return (this->code == tmp->get_Code() && this->date == tmp->get_Date());
}
