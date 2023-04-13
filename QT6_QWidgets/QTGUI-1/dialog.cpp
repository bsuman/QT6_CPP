
#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <QList>
#include <QSqlRecord>

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
    qInfo() << "Number of services: " << num_stylists;
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
          date_time DATETIME NOT NULL ,\
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


void Dialog::getStylists(QStringList& stylists){
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
    QDateTime now= QDateTime::currentDateTime();
    ui->dateTimeEdit->setDateTime(now);
    ui->dateTimeEdit->setDateTimeRange(now,now.addMonths(2));
    QStringList services ;
    getServices(services);
    QStringList stylists;
    getStylists(stylists);
    ui->servicesList->addItems(services);
    ui->stylistList->addItems(stylists);
    connect(ui->makeAppBtn,&QPushButton::clicked,this,&Dialog::makeAppointment);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::makeAppointment(){
    qInfo() <<"makeAppointment called!";

    QString name = ui->nameEdit->text();
    QString date = ui->dateTimeEdit->dateTime().date().toString();
    QString time = ui->dateTimeEdit->dateTime().time().toString();
    //QString stylist = ui->listWidget->currentItem()->text();
    QString msg = name + " booked an appointment on " + date + " at " + time;
    qInfo() << msg;
    QMessageBox::information(this,"Appointment Details",msg, QMessageBox::Ok);
    accept();
}
