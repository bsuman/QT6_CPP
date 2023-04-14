
#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>



QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog

{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;
    QSqlDatabase db;
    QMap<QString,QString> stylist_info;
    QMap<QString,QString> service_info;
    QMap<QString,QString> id_stylist_info;
    QMap<QString,QString> id_service_info;

    void connectToDatabase();
    bool executeQuery(QString &query_s,QSqlQuery &query);
    bool updateServices();
    bool updateStylists();
    void createTables();
    void getServices(QStringList& services);
    void getStylists(QStringList& stylists,QString start_time,QString end_time);
    void loadMakeAppointmentTab();
    void loadCancelAppointmentTab();
    bool saveAppointmentinDB(QString &name, QString& phone_number,QString& stylist,QDateTime& datetime, QStringList &services,QString& appointmentId);

private slots:
    void makeAppointment();
    void cancelAppointment();
    void loadBookingDetails();
    void loadStylists();
};

#endif // DIALOG_H
