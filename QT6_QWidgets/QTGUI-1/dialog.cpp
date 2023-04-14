
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
    QString q_s("Select id,service_name, price from services");
    bool ok = executeQuery(q_s,query);
    QString serviceInfo;
    if(ok){
        while(query.next()){
            service_info[query.value(1).toString()] = query.value(0).toString();
            id_service_info[query.value(0).toString()] = query.value(1).toString();
            serviceInfo= query.value(1).toString() + "  ( " + query.value(2).toString()+ "€ )" ;
            services.push_back(serviceInfo);
        }
    }
    else{
        qWarning() << "Service Infomation could not be fetched!";
    }
}


void Dialog::getStylists(QStringList& stylists,QString start_time,QString end_time){
    QSqlQuery query(db);
    QString q_s("Select id, stylist_name from stylists");
    bool ok = executeQuery(q_s,query);
    QString stylistInfo;
    if(ok){
        while(query.next()){
            stylist_info[query.value(1).toString()] = query.value(0).toString();
            id_stylist_info[query.value(0).toString()] = query.value(1).toString() ;
            stylistInfo=query.value(1).toString();
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


bool Dialog::saveAppointmentinDB(QString &name, QString& phone_number,QString& stylist,QDateTime& datetime, QStringList &services,QString& appointmentId)
{
    QSqlQuery query(db);
    QString q_s = "Select id from customers where cust_name =\'" + name + "\' and phone_number = \'"+ phone_number + "\')";
    bool ok = executeQuery(q_s,query);
    if(!ok){
        qWarning()<< "Customer could not be searched in the database";
    }
    QString cust_id;
    if(query.first()){
        cust_id = query.value(0).toString();
    }
    else{
        q_s = "INSERT INTO customers (cust_name,phone_number) VALUES (\'" + name + "\',\'"+ phone_number + "\')";
        ok = executeQuery(q_s,query);
        if(!ok){
            qWarning()<<"Customer could not be saved to the database!";
            return ok;
        }
        cust_id = query.lastInsertId().toString();
    }


    QString stylist_id = stylist_info[stylist];
    QDateTime starttime = datetime;
    foreach(QString service, services){
        QString start_time = starttime.toString(Qt::ISODate);
        QString end_time = starttime.addSecs(5400).toString(Qt::ISODate);
        QString service_id = service_info[service.trimmed()];
        q_s = "INSERT INTO appointments(cust_id,service_id,stylist_id,start_date_time,end_date_time) VALUES (\'"
              + cust_id + "\',\'"+ service_id + "\',\'"+  stylist_id + "\',\'"+start_time + "\',\'"+ end_time  + "\')";

        qInfo() << "insert appointment query" << q_s;
        ok = executeQuery(q_s,query);
        if(!ok){
            qWarning()<<"Appointments could not be saved to the database!";
            return ok;
        }
        starttime = starttime.addSecs(5400);
        if(appointmentId == ""){
            appointmentId = query.lastInsertId().toString();
        }else{
            appointmentId = appointmentId + "," + query.lastInsertId().toString();
        }
    }
    qInfo() << "Appointment saved for each selected service!";
    return true;
}


void Dialog::makeAppointment(){
    QString name = ui->nameEdit->text();
    QString phone_number = ui->mobileNumEdit->text();
    QDateTime datetime = ui->dateTimeEdit->dateTime();
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
    QString service = "",tmp = "" ;
    QStringList services;
    if(selected.size()>0){
        foreach(QListWidgetItem *item,selected){
            tmp = item->text().chopped(8);
            services.push_back(tmp);
            if(service.size() > 0){
                service = service + ", "  + tmp;
            }
            else{
                service = tmp;
            }
        }
        selected.clear();
    }
    else{
        QMessageBox::warning(this,"No service selected","Please select a service!", QMessageBox::Ok);
        return;
    }
    QString appointmentIds = "";
    bool ok= saveAppointmentinDB(name, phone_number,stylist,datetime,services,appointmentIds);

    if(ok){
        QString msg = name + " booked an appointment on " + date + " at " + time + " with stylist " + stylist + " for " + service + " service ";
        qInfo() << msg;
        QMessageBox::information(this,"Appointment Confirmation",msg, QMessageBox::Ok);

        QStringList app_i = appointmentIds.split(",");
        msg = "Please note the booking number for each service: \n";
        int index = 0;
        foreach(QString app,app_i){
            msg = msg + services[index] + " -> " + app + "\n";
            index ++;
        }
        QMessageBox::information(this,"Appointment Details",msg, QMessageBox::Ok);
    }
    else{
        QMessageBox::warning(this,"Connection Issues","Sorry but appointment cannot be made at the moment. \n Please try again later! ", QMessageBox::Ok);
    }
    accept();
}

void Dialog::loadMakeAppointmentTab()
{
    ui->makeAppWidget->setVisible(true);
    QDateTime now= QDateTime::currentDateTime();
    qInfo() << "Now date-time format" <<  now.toString();
    ui->dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm");
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
    connect(ui->cancelBtn,&QPushButton::clicked,this,&Dialog::accept);
}


void Dialog::loadCancelAppointmentTab()
{
    ui->cancelAppWidget->setVisible(true);
    ui->bookingDetailstableWidget->setEnabled(false);
    connect(ui->searchButton,&QPushButton::clicked,this,&Dialog::loadBookingDetails);
    connect(ui->cancelAppButton,&QPushButton::clicked,this,&Dialog::cancelAppointment);
    connect(ui->discardCancelButton,&QPushButton::clicked,this,&Dialog::accept);
}

void Dialog::loadBookingDetails(){
    QString booking_id = ui->bookingNumEdit->text();
    QString customer_name = ui->nameEdit->text();
    QString phone_number = ui->mobileNumEdit->text();
    if(booking_id.size()>0 && customer_name.size()>0 && phone_number.size()>0){
        QSqlQuery query(db);
        QSqlQuery query_cust(db);
        QString q_s = "Select id,service_id,stylist_id,start_date_time,cust_id from appointments where id=\'"+ booking_id + "\'";
        qInfo()<<"Appointment Query:" <<q_s;
        bool ok = executeQuery(q_s,query);
        ui->bookingDetailstableWidget->setEnabled(true);

        ui->bookingDetailstableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->bookingDetailstableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        int row = ui->bookingDetailstableWidget->rowCount();

        if(ok){
            QString cust_id = "";
            while(query.next()){
                cust_id = query.value(4).toString();
                q_s = "Select cust_name,phone_number from customers where id = \'" + cust_id + "\'";
                ok = executeQuery(q_s,query_cust);
                if(ok){
                    while(query_cust.next()){
                        if(query_cust.value(0).toString() ==customer_name && query_cust.value(1).toString() ==phone_number ){
                            ui->bookingDetailstableWidget->insertRow(row);
                            for(int col = 0 ; col < 4;col++){
                                QTableWidgetItem  *tmp ;
                                if(col == 1){
                                    tmp = new QTableWidgetItem(id_service_info[query.value(col).toString()]);
                                }
                                else if(col == 2){
                                    tmp = new QTableWidgetItem(id_stylist_info[query.value(col).toString()]);
                                }
                                else if(col == 3){
                                    tmp = new QTableWidgetItem(QDateTime::fromString(query.value(col).toString(),Qt::ISODate).toString());
                                }
                                else{
                                    tmp = new QTableWidgetItem(query.value(col).toString());
                                }
                                ui->bookingDetailstableWidget->setItem(row,col,tmp);
                            }
                            row++;
                        }
                        else{
                            QMessageBox::warning(this,"Customer Information Mismatch","The given booking ID is not for current customer!!", QMessageBox::Ok);
                            return;
                        }
                    }
                }
                else{
                    qWarning()<< "Customer does not exist!";
                }

            }
        }
    }
    else{
        QMessageBox::warning(this,"Invalid Booking ID","Please enter a valid booking ID!", QMessageBox::Ok);
        return;
    }

}

void Dialog::cancelAppointment(){
    QList<QTableWidgetItem*> items =  ui->bookingDetailstableWidget->selectedItems();
    if(items.size()>0){
        QTableWidgetItem* pitem(items[0]);
        QString appointmentID = pitem->text();

        QSqlQuery query(db);
        QString q_s = "DELETE FROM appointments where id =\'" + appointmentID +"\'";
        bool ok = executeQuery(q_s,query);
        if(!ok){
            QMessageBox::warning(this,"Connection Error"," Sorry, could not delete the selected appointment, please try again later!", QMessageBox::Ok);
            return;
        }
        else{
            QMessageBox::information(this,"Appointment cancellation confirmation","Appointment cancelled!", QMessageBox::Ok);
            accept();
        }
    }
    else{
        QMessageBox::warning(this,"No appointment selected","Please select atleast one appointment!", QMessageBox::Ok);
        return;
    }
}

