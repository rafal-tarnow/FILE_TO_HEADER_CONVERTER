#include "widget.hpp"
#include "ui_widget.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>

#include <iostream>
#include <fstream>
using namespace std;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_openFile_clicked()
{
    ui->progressBar->setValue(0);
    fileName = QFileDialog::getOpenFileName(this, tr("Select file to convert"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));

    ui->label_file_to_convert->setText(fileName);

    ui->console->appendPlainText("Selected File to convert: " + fileName);

}

void Widget::on_pushButton_convert_clicked()
{
    QString headerFile = QFileDialog::getSaveFileName(this, tr("Save file"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), ".h");

    if((!headerFile.endsWith(".h", Qt::CaseInsensitive)) && (!headerFile.endsWith(".hpp", Qt::CaseInsensitive)))
    {
        headerFile += ".h";
    }

    ui->console->appendPlainText("Output file:" + headerFile);

    ui->console->appendPlainText("START CONVERT:");
    ui->progressBar->setValue(0);


    ui->console->appendPlainText("  ->Start load input file to memory");
    std::ifstream file(fileName.toStdString(), std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    ui->console->appendPlainText("  ->File size = " + QString::number(size));
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size))
    {
        /* worked! */
    }


    ofstream myfile;
    myfile.open (headerFile.toStdString());

    if(myfile.is_open())
    {
        ui->console->appendPlainText("  Sucesfull created output file");
    }else
    {
        ui->console->appendPlainText("  Error while creating output file");
        return;
    }

    myfile << "#pragma once\n\n";
    myfile << "const static int size = "<<size << ";\n\n\n\n\n\n\n";
    myfile << "const char tablica[size] = {\n\t";

    for(int i = 0; i < buffer.size(); i++)
    {
        myfile << "0x";

        if((unsigned int)(unsigned char)buffer[i] > 0x0F)
        {
            myfile << hex << (unsigned int)(unsigned char)buffer[i];
        }else{
            myfile << "0" <<hex << (unsigned int)(unsigned char)buffer[i];
        }
        myfile << ", ";
        if((i % 10) == 9)
        {
            myfile << "\n\t";
        }
    }
    myfile << "\n}\n";

    myfile.close();
    ui->console->appendPlainText("      Close output file");
    ui->progressBar->setValue(100);

}
