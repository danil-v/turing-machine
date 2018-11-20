#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Машина Тьюринга");

    tape = new QTableWidget (ui->toolBar);
    tape->setMaximumHeight(110);
    ui->toolBar->setIconSize(QSize(40,100));
    ui->toolBar->addAction(ui->actionPrev);
    ui->toolBar->addWidget(tape);
    ui->toolBar->addAction(ui->actionNext);

    tape->setColumnCount(201);
    tape->horizontalHeader()->setVisible(false);
    tape->verticalHeader()->setVisible(false);
    tape->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tape->setRowCount(3);
    tape->setSelectionMode(QAbstractItemView::NoSelection);
    tape->setIconSize(QSize(23,30));

    for (int i=0 ; i<tape->columnCount(); i++)
    {
        tape->setItem(2,i,new QTableWidgetItem(QString::number(i-100)));
        tape->item(2,i)->setTextColor(Qt::darkBlue);

        tape->item(2,i)->setFlags(Qt::NoItemFlags);
        tape->setColumnWidth(i,30);
        tape->setItem(1,i,new QTableWidgetItem(""));
        tape->item(1,i)->setTextColor(Qt::darkGreen);

        tape->item(1,i)->setBackground(QBrush(QPixmap(":/style/frame.png").scaled(38,30)));

        tape->setItem(0,i,new QTableWidgetItem(""));
        tape->item(0,i)->setFlags(Qt::ItemIsEnabled);
        tape->item(0,i)->setTextAlignment(Qt::AlignCenter);
        tape->item(1,i)->setTextAlignment(Qt::AlignCenter);
        tape->item(2,i)->setTextAlignment(Qt::AlignCenter);
        connect(tape,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(TapeChangeCurrent(int,int)));
    }

    tape->setShowGrid(false);


    current=101;
    TapeLeft();

    abc = new QLineEdit(ui->toolBar_2);
    ui->toolBar_2->addWidget(new QLabel (QString("Алфавит\t"),ui->toolBar_2));
    ui->toolBar_2->addWidget(abc);
    ui->toolBar_2->addAction(ui->actionEdit);

    connect(ui->actionPrev,SIGNAL(triggered()),this,SLOT(TapeLeft()));
    connect(ui->actionNext,SIGNAL(triggered()),this,SLOT(TapeRight()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(About()));

    // radio menu
    QActionGroup* group = new QActionGroup( this );

    ui->actionFast->setCheckable(true);
    ui->actionNormal->setCheckable(true);
    ui->actionSlow->setCheckable(true);

    ui->actionFast->setActionGroup(group);
    ui->actionNormal->setActionGroup(group);
    ui->actionSlow->setActionGroup(group);

    ui->actionFast->setChecked(true);

    connect(abc,SIGNAL(textChanged(QString)),this,SLOT(AbcChange(QString)));
    connect(abc,SIGNAL(editingFinished()),this,SLOT(SetTable()));
    connect(tape,SIGNAL(cellChanged(int,int)),this,SLOT(TapeChange(int,int)));

    connect(ui->action_table_add_left,SIGNAL(triggered()),this,SLOT(TableColumnAddLeft()));
    connect(ui->action_table_add_right,SIGNAL(triggered()),this,SLOT(TableColumnAddRight()));
    connect(ui->action_table_column_delete,SIGNAL(triggered()),this,SLOT(TableColumnDelete()));

    this->setWindowIcon(QIcon(":/style/icon.png"));
    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(DoubleClickTable(int,int)));

    connect(ui->actionStep,SIGNAL(triggered()),this,SLOT(ALU()));
    connect(ui->actionRun,SIGNAL(triggered()),this,SLOT(Run()));
    connect(ui->actionPause,SIGNAL(triggered()),this,SLOT(Pause()));
    connect(ui->actionStop,SIGNAL(triggered()),this,SLOT(Stop()));

    connect(ui->actionFast,SIGNAL(triggered()),this,SLOT(SetFast()));
    connect(ui->actionSlow,SIGNAL(triggered()),this,SLOT(SetSlow()));
    connect(ui->actionNormal,SIGNAL(triggered()),this,SLOT(SetNomal()));

    nQ =0;


    SetFast();

    last_cell.first=0;
    last_cell.second=0;


     isEdit = false;
     TapeCursor=100;
     connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(OpenFile()));
     connect(&timer,SIGNAL(timeout()),this,SLOT(ALU()));
     connect(ui->actionEdit,SIGNAL(triggered()),this,SLOT(Edit()));
     connect(ui->actionSaveTape,SIGNAL(triggered()),this,SLOT(TapeSave()));
     connect(ui->actionLoadTape,SIGNAL(triggered()),this,SLOT(TapeLoad()));
     connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(NewFile()));
     connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(SaveFile()));
     connect(ui->actionSave_as,SIGNAL(triggered()),this,SLOT(SaveFileAs()));
     connect(ui->actionRemoveCell,SIGNAL(triggered()),this,SLOT(TableCellDelete()));
     connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));

     inputText = new QLineEdit(ui->toolBar_2);
     inputText->setPlaceholderText("Символы для ввода на ленту...");
     inputText->setMaxLength(200);
     ui->toolBar_2->addSeparator();
     ui->toolBar_2->addWidget(inputText);
     ui->toolBar_2->addAction(ui->actionAddTapeText);
     connect(inputText,SIGNAL(textChanged(QString)),this,SLOT(InputTextChange(QString)));
     connect(ui->actionAddTapeText,SIGNAL(triggered()),this,SLOT(AddTapeText()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TapeLeft()
{
    if(current-8>0)
    {
        tape->setCurrentCell(0,current-8);
        tape->setCurrentCell(0,current+7);
        current--;
        tape->item(0,current+1)->setIcon(QIcon());
        tape->item(0,current)->setIcon(QIcon(":/style/down.png"));
    }
    else if(current-1>-1)
    {
        tape->setCurrentCell(0,current-1);
        current--;
        tape->item(0,current+1)->setIcon(QIcon());
        tape->item(0,current)->setIcon(QIcon(":/style/down.png"));
    }
}

void MainWindow::TapeRight()
{
    if(current+8<201)
    {
        tape->setCurrentCell(0,current+8);
        tape->setCurrentCell(0,current-7);
        current++;
        tape->item(0,current-1)->setIcon(QIcon());
        tape->item(0,current)->setIcon(QIcon(":/style/down.png"));
    }
    else if(current+1<201)
    {
        tape->setCurrentCell(0,current+1);
        current++;
        tape->item(0,current-1)->setIcon(QIcon());
        tape->item(0,current)->setIcon(QIcon(":/style/down.png"));
    }
}

void MainWindow::TapeChangeCurrent(int r, int c)
{
    if(r!=0) return;
    tape->item(0,current)->setIcon(QIcon());
    current=c;
    tape->item(0,current)->setIcon(QIcon(":/style/down.png"));
}



void MainWindow::AbcChange(QString text)
{
    isEdit=true;
    QMap<QChar,int>  mmap;
    QString result;
    text.replace(" ","");
    text.replace("\t","");
    text.replace("_","");

    for (int i=0;i<text.size();i++)
    {
        mmap[text[i]]++;
        if(mmap[text[i]]==1&&text[i]!=',')
        {
           result.push_back(text[i]);
           result.push_back(QChar(','));
        }
    }
    result.chop(1);
    abc->setText(result);


}

void MainWindow::SetTable()
{
    QStringList HoriantalHeader;


    list = abc->text().split(",");

    list.push_back("_");

    if (abc->text().isEmpty())list.pop_front();
    else
    {
        abc->setReadOnly(true);
        abc->setStyleSheet("Background-color:rgba(0,255,0,50);");
    }

    if(!isEdit) return;
    isEdit=false;

    TapeCopy.clear();

    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(list.size());
    ui->tableWidget->setRowCount(list.size());

    ui->tableWidget->setItem(0,0,new QTableWidgetItem);

    for (int i=0 ; i<list.size(); i++)
    {
        ui->tableWidget->setRowHeight(i,20);
        ui->tableWidget->setColumnWidth(i,50);
        HoriantalHeader<<QString("Q")+QString::number(i+1);
    }

    ui->tableWidget->setVerticalHeaderLabels(list);
    ui->tableWidget->setHorizontalHeaderLabels(HoriantalHeader);
    list.pop_back();

    for (int i=0;i<tape->columnCount();i++)
        if(list.indexOf(tape->item(1,i)->text()))
            tape->item(1,i)->setText("");

    UpdateHorisantalHeader();
}

void MainWindow::TapeChange(int r, int c)
{
    if (r!=1) return;

    QString text=tape->item(r,c)->text();

    if(text.length()>1)
    {
        QMessageBox::warning(this,"Ошибка ввода","Вы ввели более одного символа");
        text.chop(text.length()-1);
    }
    if (text.length()>0&&list.indexOf(QString(text[0]))==-1)
    {
        QMessageBox::warning(this,"Ошибка ввода","Вы ввели символ которого нет в алфавите");
        text="";
    }
    tape->item(r,c)->setText(text);
}

void MainWindow::TableColumnAddLeft()
{
    ui->tableWidget->insertColumn(ui->tableWidget->currentColumn());
    UpdateHorisantalHeader();
}

void MainWindow::TableColumnAddRight()
{
    if(abc->text().isEmpty())return;
    ui->tableWidget->insertColumn(ui->tableWidget->currentColumn()+1);
    UpdateHorisantalHeader();
}

void MainWindow::TableColumnDelete()
{
    if (ui->tableWidget->columnCount()<2) return;
    ui->tableWidget->removeColumn(ui->tableWidget->currentColumn());
    UpdateHorisantalHeader();
}

void MainWindow::TableCellDelete()
{
    if(ui->tableWidget->currentItem())
        ui->tableWidget->currentItem()->setText("");
}

void MainWindow::UpdateHorisantalHeader()
{
    QStringList HoriantalHeader;
    for (int i=0 ; i<ui->tableWidget->columnCount(); i++)
    {
        ui->tableWidget->setColumnWidth(i,50);
        HoriantalHeader<<QString("Q")+QString::number(i+1);
    }
    ui->tableWidget->setHorizontalHeaderLabels(HoriantalHeader);
}

void MainWindow::DoubleClickTable(int r, int c)
{
    QStringList Q,ABC=list;
    for (int i=0 ; i<ui->tableWidget->columnCount(); i++)
        Q<<ui->tableWidget->horizontalHeaderItem(i)->text();
    Q<<"Q0";
    ABC.push_back("_");
    window = new Dialog(ABC,Q,this);
    window->exec();
    if(!window->GetText().isEmpty())
    {
        ui->tableWidget->setItem(r,c,new QTableWidgetItem);
        ui->tableWidget->item(r,c)->setText(window->GetText());
    }
    delete window;
}

void MainWindow::About()
{
    QMessageBox::about(this,"О программе","...");
}

void MainWindow::ALU()
{

    if (!ui->tableWidget->columnCount())
    {
       QMessageBox::warning(this,"Ошибка","Заполните алфавит и задайте состояния для управляющего устройства.");
       Stop();
       return;
    }

    int r=0;
    QString ch(tape->item(1,current)->text());
    if (tape->item(1,current)->text().isEmpty())
        ch="_";

    for (int i=0; i<ui->tableWidget->rowCount();i++)
        if(ui->tableWidget->verticalHeaderItem(i)->text()==ch)
            r=i;


    //если пустая ячейка
    if (!ui->tableWidget->item(r,nQ))
        ui->tableWidget->setItem(r,nQ,new QTableWidgetItem);


    ui->tableWidget->item(last_cell.first,last_cell.second)->setBackgroundColor(Qt::white);
    ui->tableWidget->item(r,nQ)->setBackgroundColor(QColor(0,255,0,50));


    last_cell.first=r;
    last_cell.second=nQ;

    if (ui->tableWidget->item(r,nQ)->text().isEmpty())
    {
        ui->tableWidget->item(r,nQ)->setBackgroundColor(QColor(255,0,0,50));
        QMessageBox::warning(this,"Ошибка","Нет команды");
        Stop();
        return;
    }

    //если не пуста, то выполняем инструкцию
    if(ui->tableWidget->item(r,nQ)->text().at(0)!=QChar('_'))
        tape->item(1,current)->setText(ui->tableWidget->item(r,nQ)->text().at(0));
    else tape->item(1,current)->setText("");

    int start = 2;

    if (ui->tableWidget->item(r,nQ)->text().at(1)==QChar('-')&&ui->tableWidget->item(r,nQ)->text().at(2)==QChar('>'))
    {
        tape->setCurrentCell(0,current+1);
        current++;
        tape->item(0,current-1)->setIcon(QIcon());
        tape->item(0,current)->setIcon(QIcon(":/style/down.png"));
        start = 3;
    }

    if (ui->tableWidget->item(r,nQ)->text().at(1)==QChar('<')&&ui->tableWidget->item(r,nQ)->text().at(2)==QChar('-'))
    {
        tape->setCurrentCell(0,current-1);
        current--;
        tape->item(0,current+1)->setIcon(QIcon());
        tape->item(0,current)->setIcon(QIcon(":/style/down.png"));
        start = 3;
    }



    nQ = ui->tableWidget->item(r,nQ)->text().mid(start+1, ui->tableWidget->item(r,nQ)->text().length()-1 - start).toInt();
    nQ--;

    //если останов
    if(nQ<0)
    {
        nQ=0;
        Stop();
        QMessageBox::about(this,"Программа завершена","Управляющее устройство перешло в состояние останова");
        return;
    }

    if (ui->tableWidget->item(r,nQ)==NULL)
        ui->tableWidget->setItem(r,nQ,new QTableWidgetItem);

}

void MainWindow::OpenFile()
{
    path =QFileDialog::getOpenFileName(this,"Выберите файл",QDir::currentPath(),"*.trn");
    if (path.isEmpty())return;

    QFile file(path);
    file.open(QFile::ReadOnly);

    ClearAllForms();

    QStringList ctx = QString(file.readAll()).split("----------\r\n");

    ui->plainTextEdit_task->setPlainText(ctx.at(0));
    ui->plainTextEdit_comments->setPlainText(ctx.at(1));
    QString alphabet=ctx.at(2);
    abc->clearFocus();
    abc->setText(alphabet.replace("\r\n",""));
    emit abc->editingFinished();
    QApplication::processEvents();


    tape->setCurrentCell(0,ctx.at(3).toInt()-2+100);
    tape->item(0,current)->setIcon(QIcon());
    current=ctx.at(3).toInt()+100;
    tape->item(0,current)->setIcon(QIcon(":/style/down.png"));

    for (int i=0;i<tape->columnCount();i++ )
        tape->item(1,i)->setText("");

    int start=100+ctx.at(4).toInt();
    for (int i=start;i<start+ctx.at(5).length()-2;i++ )
        if(ctx.at(5).at(i-start)==' ')
            tape->item(1,i)->setText("");
        else
            tape->item(1,i)->setText(ctx.at(5).at(i-start));



    ui->tableWidget->setColumnCount(ctx.at(6).toInt());
    QStringList text=ctx.at(7).split("\r\n");

    if (!ctx.at(6).toInt())
        goto go;

    for (int i=0;i<text.length();i++)
    {
         QStringList line=text.at(i).split("\t");
         for (int j=0;j<line.length();j++)
         {
             if(ui->tableWidget->item(i,j)==NULL)
                ui->tableWidget->setItem(i,j,new QTableWidgetItem);

             ui->tableWidget->item(i,j)->setText(line.at(j));
         }
    }

    go:

        nQ = 0;

        TapeSave();
        UpdateHorisantalHeader();
}

void MainWindow::NewFile()
{
    path.clear();
    ClearAllForms();
}

void MainWindow::ClearAllForms()
{
    abc->clear();
    inputText->clear();
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->setRowCount(0);

    for (int i=0;i<tape->columnCount();i++ )
        tape->item(1,i)->setText("");

    tape->item(0,current)->setIcon(QIcon());
    current=100;
    tape->item(0,current)->setIcon(QIcon(":/style/down.png"));
    tape->setCurrentCell(0,current);

    emit ui->actionEdit->trigger();

    ui->plainTextEdit_comments->clear();
    ui->plainTextEdit_task->clear();

    QString buff="";
    for (int i=0;i<tape->columnCount();i++ )
        if(tape->item(1,i)->text().isEmpty())buff+=" ";
        else buff+=tape->item(1,i)->text();
    SetTable();
}

void MainWindow::SaveFile()
{
    if (path.isEmpty())
    {
        SaveFileAs();
        return;
    }

    QFile file(path);

    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
        QMessageBox::critical(this,"Ошибка","Не удалось сохранить файл");
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    QString splitter="----------\n";
    out<<ui->plainTextEdit_task->toPlainText()<<splitter;
    out<<ui->plainTextEdit_comments->toPlainText()<<splitter;
    out<<abc->text()<<splitter;
    out<<current-100<<splitter;
    int curr=0;
    for (int i=0;i<tape->columnCount();i++ )
        if(!tape->item(1,i)->text().isEmpty())
        {
            curr=i;
            break;
        }
    out<<curr-100<<splitter;


     QString buff;
     for (int i=curr;i<tape->columnCount();i++ )
         if(tape->item(1,i)->text().isEmpty())
             buff+=" ";
         else buff+=tape->item(1,i)->text();
     out<<buff.trimmed()<<"\n"<<splitter;
     out<<ui->tableWidget->columnCount()<<splitter;

     for (int i=0;i<ui->tableWidget->rowCount();i++ )
     {
         for (int j=0;j<ui->tableWidget->columnCount();j++ )
         {
             if(ui->tableWidget->item(i,j)!=NULL)
                 out<<ui->tableWidget->item(i,j)->text();

             if(j<ui->tableWidget->columnCount()-1)
                 out<<"\t";
         }
         if(i<ui->tableWidget->rowCount()-1)out<<endl;
     }

     file.close();
}

void MainWindow::SaveFileAs()
{
    QString filter="Turing machine file(*.trn)";
    path=QFileDialog::getSaveFileName(this,"Сохранение файла",QDir::currentPath(),filter,&filter);
    if (path.isEmpty())return;
    SaveFile();
}

void MainWindow::SetFast()
{
     timer.setInterval(70);
}

void MainWindow::SetNomal()
{
    timer.setInterval(500);
}

void MainWindow::SetSlow()
{

    timer.setInterval(3000);
}

void MainWindow::Run()
{
    timer.start(timer.interval());
}

void MainWindow::Stop()
{
    timer.stop();
    nQ=0;
}

void MainWindow::Pause()
{
    timer.stop();
}

void MainWindow::Edit()
{
    abc->setReadOnly(false);
    abc->setStyleSheet("Background-color:rgb(255,255,255);");
}

void MainWindow::TapeSave()
{
    TapeCopy.clear();
    for (int i=0;i<tape->columnCount();i++)
        TapeCopy.push_back(tape->item(1,i)->text());
    TapeCursor=current;
}

void MainWindow::TapeLoad()
{
    if(TapeCopy.empty())
    {
        QMessageBox::about(this,"Ошибка","Лента пуста");
        return;
    }
    for (int i=0;i<tape->columnCount();i++)
       tape->item(1,i)->setText(TapeCopy.at(i));

    tape->item(0,current)->setIcon(QIcon());
    current=TapeCursor;
    tape->item(0,current)->setIcon(QIcon(":/style/down.png"));
    tape->setCurrentCell(0,current);
}

void MainWindow::AddTapeText()
{
    if(inputText->text().isEmpty())return;

    for (int i=0;i<tape->columnCount();i++)
        tape->item(1,i)->setText("");

    int d=100-inputText->text().length()/2;
    for (int i=0;i<inputText->text().length();i++)
        tape->item(1,i+d)->setText(QString(inputText->text().at(i)).replace(" ",""));
}

void MainWindow::InputTextChange(QString text)
{
    QStringList chars=abc->text().split(",");

    for (int i=0;i<text.length();i++)
        if(!chars.count(text.at(i)))
            text.replace(text.at(i),"");

    inputText->setText(text);
}




