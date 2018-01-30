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

uint32_t Widget::loadFileToVector(string fileName, std::vector<char> &buffer)
{
    std::ifstream file(fileName, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    ui->console->appendPlainText("  ->File size = " + QString::number(size));
    file.seekg(0, std::ios::beg);

    buffer.resize(size);
    if (file.read(buffer.data(), size))
    {
        /* worked! */
    }
}

void Widget::on_pushButton_convert_clicked()
{
    QString outputFileWithPath = QFileDialog::getSaveFileName(this, tr("Save file"), /*QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" +*/ fileName + ".hpp", "*.h;*.hpp");

    QFileInfo fileInfo(outputFileWithPath);

    QString outputFileName = fileInfo.fileName();
    QString outputFileDir = fileInfo.dir().path() + "/";

    if((!outputFileName.endsWith(".h", Qt::CaseInsensitive)) && (!outputFileName.endsWith(".hpp", Qt::CaseInsensitive)))
    {
        outputFileName += ".h";
    }

    ui->console->appendPlainText("Output file:" + outputFileName);

    ui->console->appendPlainText("START CONVERT:");
    ui->progressBar->setValue(0);


    ui->console->appendPlainText("  ->Start load input file to memory");


    std::vector<char> buffer;
    uint32_t size = loadFileToVector(fileName.toStdString(), buffer);

    generateHeaderFile(outputFileDir.toStdString(), outputFileName.toStdString(), buffer);

    ui->progressBar->setValue(100);
}




void Widget::generateHeaderFile(string headerFileDir, string headerFileName, std::vector<char> &buffer)
{
    string filePath = headerFileDir + headerFileName;
    //OPEN FILE
    ofstream header_file;
    header_file.open (filePath);
    if(header_file.is_open()){
        ui->console->appendPlainText("  Sucesfull created output file");
    }else{
        ui->console->appendPlainText("  Error while creating output file");
        return;
    }

    //GNERATE FILE
    header_file << "#pragma once\n\n";
    header_file << "const static int size = " << buffer.size() << ";\n\n";
    string table_name = remove_extension(headerFileName);
    std::replace( table_name.begin(), table_name.end(), '.', '_');
    header_file << "unsigned char " << table_name << "[size] = {\n\t";

    for(unsigned int i = 0; i < buffer.size(); i++)
    {
        header_file << "0x";

        if((unsigned int)(unsigned char)buffer[i] > 0x0F)
        {
            header_file << hex << (unsigned int)(unsigned char)buffer[i];
        }else{
            header_file << "0" <<hex << (unsigned int)(unsigned char)buffer[i];
        }
        header_file << ", ";
        if((i % 10) == 9)
        {
            header_file << "\n\t";
        }
    }
    header_file << "\n};\n";

    //CLOSE FILE
    header_file.close();
    ui->console->appendPlainText("      Close output file");
}


string Widget::remove_extension(const string& filename) {
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename;
    return filename.substr(0, lastdot);
}
