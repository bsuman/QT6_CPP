
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

    void connectToDatabase();
    bool executeQuery(QString &query_s,QSqlQuery &query);
    bool updateServices();
    bool updateStylists();
    void createTables();
    void getServices(QStringList& services);
    void getStylists(QStringList& stylists);

private slots:
    void makeAppointment();
};

#endif // DIALOG_H
