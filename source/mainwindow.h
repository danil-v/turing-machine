#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QSplitter>
#include <QScrollBar>
#include <QTableWidgetItem>
#include <QPainter>
#include <QMap>
#include <QChar>
#include <QMessageBox>
#include <QStringList>
#include <QFileDialog>
#include <QFile>
#include <QTimer>
#include <QPair>
#include <QTextStream>
#include <QDebug>


#include "dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QTableWidget *tape;//лента
    QPlainTextEdit *task;//задание
    QLineEdit *abc;// алфавит
    QTableWidgetItem *Item;
    int current;// текущая позиция каретки
    Dialog *window;// виджет ввода состояния
    int nQ;// текущее состояние
    int wait_ms;// задержка ms
    QTimer timer;// таймер
    QStringList list;
    QPair <int,int> last_cell;
    bool isEdit;
    QStringList TapeCopy;
    int TapeCursor;
    QString path;

    QLineEdit *inputText;
    QPushButton *addInputText;


private slots:
    void TapeLeft();//передаинуть каретку влево
    void TapeRight();//передвинуть каретку вправо
    void TapeChangeCurrent(int r, int c);//переместить каретку по клику мыши

    void AbcChange( QString text);// именение алфавита
    void SetTable();//задать таблицу состояний исходя из алфавита
    void TapeChange(int r, int c);//изменение символа на ленте

    void TableColumnAddLeft();//добавить колонку слева
    void TableColumnAddRight();//добавить колонку справа
    void TableColumnDelete();//удалить колонку
    void TableCellDelete();// очистить ячейку
    void UpdateHorisantalHeader();//обновить горизонтальный хеадер

    void DoubleClickTable(int r,int c);// изменение ячейки таблицы состояний по двойному клику

    void About();// справка

    void ALU();// управляющее устройство ( сделать шаг)

    void OpenFile();// открыть файл
    void NewFile();// новый файл
    void ClearAllForms();//очистить все формы на виджете
    void SaveFile();// сохранить файл
    void SaveFileAs();// сохранить файл как

    void SetFast();//установить быструю скорость
    void SetNomal();//установить нормальную скорость
    void SetSlow();//установить медленную скорость
    void Run();// запуск
    void Stop();// стоп
    void Pause();// пауза

    void Edit();// разрешить редактировать алфавит

    void TapeSave();//сохранить ленту
    void TapeLoad();//выгрузить ленту

    //добавить символы на ленту
    void AddTapeText();
    void InputTextChange( QString text);// именение алфавита
};

#endif // MAINWINDOW_H
