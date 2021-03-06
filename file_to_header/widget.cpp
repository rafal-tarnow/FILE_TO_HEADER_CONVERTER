#include "widget.hpp"
#include "ui_widget.h"
#include <QStandardPaths>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <boost/filesystem/path.hpp>

using namespace std;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->label_inputDir->setText(dirToDir.getInputDirectory());
    ui->label_outputDir->setText(dirToDir.getOutputDirectory());
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_openFile_clicked()
{
    ui->progressBar->setValue(0);
    fileToFile.setInputFileFullPath(QFileDialog::getOpenFileName(this, tr("Select file to convert"), fileToFile.inputFileDir));

    ui->label_file_to_convert->setText(fileToFile.getInputFileName());
    ui->console->appendPlainText("Selected File to convert: " + fileToFile.getInputFileName());
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
    QString outputFileWithPath = QFileDialog::getSaveFileName(this, tr("Save file"), /*QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" +*/ fileToFile.getInputFileName() + ".hpp", "*.h;*.hpp");

    fileToFile.setOutputFileFullPath(outputFileWithPath);

    //    if((!outputFileName.endsWith(".h", Qt::CaseInsensitive)) && (!outputFileName.endsWith(".hpp", Qt::CaseInsensitive)))
    //    {
    //        outputFileName += ".h";
    //    }

    ui->console->appendPlainText("START CONVERT:");
    ui->progressBar->setValue(0);


    fileToFile.print();

    generateHeaderFile(fileToFile.getInputFileFullPath(), fileToFile.getOutputFileFullPath());

    ui->progressBar->setValue(100);
}


void checkFileDirectoryExistAndCreate(QString fileFullPath)
{
    QFileInfo info(fileFullPath);
    QString fileDir = info.absoluteDir().absolutePath();
    qDebug() << " Dir for file " << fileFullPath << " = " << fileDir;

    QDir dir(fileDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

void Widget::generateHeaderFile(QString inputFileFullPath, QString outputFileFullPath)
{
    //CHECK FILE DIRECTORY EXIST

    checkFileDirectoryExistAndCreate(outputFileFullPath);


    //LOAD FILE TO BUFFER
    std::vector<char> buffer;
    uint32_t size = loadFileToVector(inputFileFullPath.toStdString(), buffer);


    int bytesInRow = ui->spinBox_bytesInRow->value();
    //OPEN FILE
    ofstream header_file;
    header_file.open (outputFileFullPath.toStdString());
    if(header_file.is_open()){
        //ui->console->appendPlainText("  Sucesfull created output file" + outputFileFullPath);
    }else{
        ui->console->appendPlainText("  Error while creating output file" + outputFileFullPath);
        return;
    }


    QFileInfo fileInfo(inputFileFullPath);
    string fileName = fileInfo.fileName().toStdString();
    //GNERATE FILE
    string table_name = remove_extension(fileName);
    std::replace( table_name.begin(), table_name.end(), '.', '_');

    header_file << "#pragma once\n\n";
    header_file << "const static int size_of_" << table_name<< " = " << buffer.size() << ";\n\n";
    header_file << "unsigned char " << table_name << "[size_of_" << table_name << "] = {\n\t";

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
        if((i % bytesInRow) == bytesInRow-1)
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

void Widget::on_pushButton_inputDir_clicked()
{
    QString inputDirPath = QFileDialog::getExistingDirectory();
    ui->label_inputDir->setText(inputDirPath);


    dirToDir.setInputDirectory(inputDirPath);
    dirToDir.print();
}

void Widget::on_pushButton_outputDir_clicked()
{
    QString outputDirPath = QFileDialog::getExistingDirectory();
    dirToDir.setOutputDirectory(outputDirPath);
    dirToDir.getOutputFiles();
    dirToDir.print();

    ui->label_outputDir->setText(outputDirPath);
}

void Widget::on_pushButton_convertDir_clicked()
{
    QMap<QString,QString> mapaPlikow = dirToDir.getOutputFiles();
    QMapIterator<QString, QString> i(mapaPlikow);
    while (i.hasNext())
    {
        i.next();
        qDebug() << "   " << i.key() << " ---convert_to-->> " << i.value();
        generateHeaderFile(i.key(), i.value());
    }



}
