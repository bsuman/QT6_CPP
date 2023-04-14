
#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <QList>
#include <QSqlRecord>
#include <QAbstractItemView>

bool Dialog::executeQuery(QString &query_s,QSqlQuery &query){
    bool isQuerySuccessful = query.exec(query_s);
    if(isQuerySuccessful){
        qInfo()<<"Query Successful!!";
    }else{
        QSqlError error = query.lastError();
        qInfo()<< error.text();
    }
    return isQuerySuccessful;
}

bool Dialog::updateStylists(){
    QSqlQuery query(db);
    QString q_s("Select count(*) from stylists");
    executeQuery(q_s,query);
    int num_stylists = 1;
    if(query.first())
        num_stylists = query.value(0).toInt();
    qInfo() << "Number of stylists: " << num_stylists;
    if(num_stylists > 1){
        return true;
    }
    query.clear();
    bool ok(false);
    QStringList stylists = {"Ivo Ernst","Suman Bidarahalli","Suguna Bidarahalli","Srivatsa Rao"};
    foreach(QString stylist, stylists){
        q_s = "INSERT INTO stylists (stylist_name) VALUES (\'"+ stylist + "\')";
        qInfo()<< q_s;
        if(db.tables().contains("stylists")){
            ok= executeQuery(q_s,query);
            if(!ok){
                qWarning()<<"INSERT into table stylists failed !";
                return ok;
            }
        }
    }
    return true;
}

bool Dialog::updateServices()
{
    QSqlQuery query(db);
    QString q_s("Select count(*) from services");
    executeQuery(q_s,query);
    int num_services = 1;
    if(query.first())
        num_services = query.value(0).toInt();
    qInfo() << "Number of services: " << num_services;
    if(num_services > 1){
        return true;
    }
    query.clear();
    bool ok(false);
    QStringList services = {"Cut","Wash","Color","Style"};
    QStringList prices = {"30","50","80","100"};
    int id = 0;
    foreach(QString service, services){
        q_s = "INSERT INTO services (service_name,price) VALUES (\'" + service + "\',\'"+ prices[id] + "\')";
        qInfo()<< q_s;
        if(db.tables().contains("services")){
            ok= executeQuery(q_s,query);
            if(!ok){
                qWarning()<<"INSERT into table services failed !";
                return ok;
            }
        }
        id++;
    }
    qInfo()<<"All services inserted done!!";
    return true;
}

void Dialog::createTables(){

    QSqlQuery query(db);
    QString q_s = "CREATE TABLE if not exists stylists(id int NOT NULL AUTO_INCREMENT,stylist_name varchar(255) NOT NULL,PRIMARY KEY (id))";
    bool ok = executeQuery(q_s,query);
    if(!ok){
        qWarning()<<"Database table stylists could not be created !";
        return;
    }
    q_s ="CREATE TABLE if not exists services(id int NOT NULL AUTO_INCREMENT,service_name varchar(255) NOT NULL, price int NOT NULL,PRIMARY KEY (id))";
    ok = executeQuery(q_s,query);
    if(!ok){
        qWarning()<<"Database table services could not be created !";
        return;
    }
    q_s ="CREATE TABLE if not exists customers(id int NOT NULL AUTO_INCREMENT,cust_name varchar(255) NOT NULL, phone_number varchar(80) NOT NULL,PRIMARY KEY (id))";
    ok = executeQuery(q_s,query);
    if(!ok){
        qWarning()<<"Database table customer could not be created !";
        return;
    }
    q_s ="CREATE TABLE if not exists appointments(id int NOT NULL AUTO_INCREMENT,\
          cust_id int NOT NULL,\
          service_id int NOT NULL,\
          stylist_id int NOT NULL,\
          start_date_time DATETIME NOT NULL ,\
          end_date_time DATETIME NOT NULL ,\
          PRIMARY KEY (id),\
          FOREIGN KEY (cust_id) REFERENCES customers(id),\
          FOREIGN KEY (service_id) REFERENCES services(id),\
          FOREIGN KEY (stylist_id) REFERENCES stylists(id))";
    ok = executeQuery(q_s,query);
    if(!ok){
        qWarning()<<"Database table appointments could not be created !";
        return;
    }
    ok =updateServices();
    if(!ok){
        qWarning()<<" Services Information could not be updated!!";
        return;
    }
    ok =updateStylists();
    if(!ok){
        qWarning()<<" Stylists Information could not be updated!!";
        return;
    }
}

void Dialog::connectToDatabase(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("salon_info");
    db.setUserName("root");
    db.setPassword("Mario123");
    bool ok = db.open();
    if(ok){
        qInfo()<<"Database opened!!";
        createTables();

    }else{
        qWarning()<<"Database cannot be opened!!";
        qWarning()<< db.lastError();
        return;
    }
}


void Dialog::getServices(QStringList& services){
    QSqlQuery query(db);
    QString q_s("Select service_name, price from services");
    bool ok = executeQuery(q_s,query);
    QString serviceInfo;
    if(ok){
        while(query.next()){
            serviceInfo= query.value(0).toString() + "  ( " + query.value(1).toString()+ "€ )" ;
            services.push_back(serviceInfo);
        }
    }
    else{
        qWarning() << "Service Infomation could not be fetched!";
    }
}


void Dialog::getStylists(QStringList& stylists,QString start_time,QString end_time){
    QSqlQuery query(db);
    QString q_s("Select stylist_name from stylists");
    executeQuery(q_s,query);
    bool ok = executeQuery(q_s,query);
    QString stylistInfo;
    if(ok){
        while(query.next()){
            stylistInfo=query.value(0).toString();
            stylists.push_back(stylistInfo);
        }
    }
    else{
        qWarning() << "Service Infomation could not be fetched!";
    }

}


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    connectToDatabase();
    ui->setupUi(this);
    loadMakeAppointmentTab();
    loadCancelAppointmentTab();

}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::makeAppointment(){
    QString name = ui->nameEdit->text();
    QString date = ui->dateTimeEdit->dateTime().date().toString();
    QString time = ui->dateTimeEdit->dateTime().time().toString();
    QString stylist;
    QList<QListWidgetItem *> selected = ui->stylistList->selectedItems();
    if(selected.size()>0){
        QListWidgetItem *item = selected[0];
        stylist = item->text();
        selected.clear();
    }
    else{
        QMessageBox::warning(this,"No stylist selected","Please select a stylist!", QMessageBox::Ok);
        return;
    }
    selected = ui->servicesList->selectedItems();
    QString service = "";
    if(selected.size()>0){
        foreach(QListWidgetItem *item,selected){
            if(service.size() > 0){
                service = service + ", "  + item->text().chopped(8);
            }
            else{
                service = item->text().chopped(8);
            }
        }
        selected.clear();
    }
    else{
        QMessageBox::warning(this,"No service selected","Please select a service!", QMessageBox::Ok);
        return;
    }

    QString msg = name + " booked an appointment on " + date + " at " + time + " with stylist " + stylist + " for " + service + " service ";
    qInfo() << msg;
    QMessageBox::information(this,"Appointment Details",msg, QMessageBox::Ok);
    accept();
}

void Dialog::loadMakeAppointmentTab()
{
    ui->makeAppWidget->setVisible(true);
    QDateTime now= QDateTime::currentDateTime();
    qInfo() << "Now date-time format" <<  now.toString();
    ui->dateTimeEdit->setDateTime(now);
    ui->dateTimeEdit->setDateTimeRange(now,now.addMonths(2));

    QString start_time = now.toString(Qt::ISODate);
    QString end_time = now.addSecs(5400).toString(Qt::ISODate);
    qInfo() << "Start Time: " << start_time;
    qInfo() << "End Time: " << end_time;

    QStringList services ;
    getServices(services);

    QStringList stylists;
    getStylists(stylists,start_time,end_time);
    ui->servicesList->clear();
    ui->servicesList->addItems(services);
    ui->servicesList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->servicesList->setSelectionBehavior(QAbstractItemView::SelectItems);

    ui->stylistList->clear();
    ui->stylistList->addItems(stylists);
    ui->stylistList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->stylistList->setSelectionBehavior(QAbstractItemView::SelectItems);
    connect(ui->makeAppBtn,&QPushButton::clicked,this,&Dialog::makeAppointment);
}


void Dialog::loadCancelAppointmentTab()
{
    ui->cancelAppWidget->setVisible(true);
    connect(ui->bookingNumEdit,&QLineEdit::editingFinished,this,&Dialog::loadBookingDetails);
    connect(ui->cancelAppButton,&QPushButton::clicked,this,&Dialog::cancelAppointment);
}

void Dialog::loadBookingDetails(){
    QString booking_id = ui->bookingNumEdit->text();
    if(booking_id.size()>0){
        QSqlQuery query(db);
        QString q_s("Select id,service_id,stylist_id,start_date_time from appointments where id=\'");
        q_s = q_s + booking_id + "\'";
        qInfo()<<"Appointment Query:" <<q_s;
        executeQuery(q_s,query);
        bool ok = executeQuery(q_s,query);
        if(ok){
            QStringList Appointments;
            QString Details;
            while(query.next()){
                Details=query.value(0).toString();
                Details = Details + ", "+query.value(1).toString();
                Details = Details + ", "+query.value(2).toString();
                Details = Details + ", "+query.value(3).toString();
                Appointments.push_back(Details);
            }
        }
    }
    else{
        QMessageBox::warning(this,"Invalid Booking ID","Please enter a valid booking ID!", QMessageBox::Ok);
        return;
    }

}

void Dialog::cancelAppointment(){

}

