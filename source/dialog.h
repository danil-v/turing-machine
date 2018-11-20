#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QStringList abc, QStringList Q, QWidget *parent = 0);
    ~Dialog();

    QString GetText();

private:
    Ui::Dialog *ui;
    QString text;

private slots:
    void SetText();
    void Close();

};

#endif // DIALOG_H
