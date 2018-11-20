#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QStringList abc, QStringList Q, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Добавление состояния");
    ui->comboBox->addItems(abc);
    ui->comboBox_2->addItems(QStringList()<<"->"<<"<-"<<"|");
    ui->comboBox_3->addItems(Q);

    connect(ui->pushButton_add,SIGNAL(clicked()),this,SLOT(SetText()));
    connect(ui->pushButton_cancel,SIGNAL(clicked()),this,SLOT(Close()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::SetText()
{
    text = ui->comboBox->currentText()+ui->comboBox_2->currentText()+ui->comboBox_3->currentText();
    this->close();
}

void Dialog::Close()
{
    this->close();
}

QString Dialog::GetText()
{
    return text;
}
