#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <QWidget>
#include <string>
#include <QSettings>
#include <QFileDialog>
#include <QDebug>
#include <QDirIterator>

using namespace std;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_openFile_clicked();

    void on_pushButton_convert_clicked();

    void on_pushButton_inputDir_clicked();

    void on_pushButton_outputDir_clicked();

    void on_pushButton_convertDir_clicked();

private:

    uint32_t loadFileToVector(string fileName, std::vector<char> &buffer);
    void generateHeaderFile(QString inputFileFullPath, QString outputFileFullPath);
    string remove_extension(const string& filename);
    Ui::Widget *ui;



    class FileToFile
    {
    public:
        FileToFile()
        {
            inputFileDir = settings.value("fileDir").toString();
        }

        void setInputFileFullPath(QString filePath)
        {
            inputFileFullPath = filePath;
            QFileInfo fileInfo(inputFileFullPath);
            inputFileDir = fileInfo.dir().absolutePath();
            inputFileName = fileInfo.fileName();
            settings.setValue("fileDir",inputFileDir);
        }

        QString getInputFileName()
        {
            return inputFileName;
        }

        QString getInputFileFullPath()
        {
            return inputFileFullPath;
        }

        void setOutputFileFullPath(QString filePath)
        {
            outputFileFullPath = filePath;
        }

        QString getOutputFileFullPath()
        {
            return outputFileFullPath;
        }

        void print()
        {
            qDebug() << "----FileToFile-----";
            qDebug() << "   inputFileName = " << inputFileName;
            qDebug() << "   inputFileFullPath = " << inputFileFullPath;
            qDebug() << "   inputFileDir = " << inputFileDir;
            qDebug() << " ";
            qDebug() << "   outputFileFullPath = " << outputFileFullPath;
            qDebug() << "-------------------";
        }

        QString inputFileName;
        QString inputFileFullPath;
        QString inputFileDir;

        QString outputFileFullPath;

        QSettings settings;
    };

    class DirToDir{
    public:
        DirToDir()
        {
            inputDirPath = settings.value("DirToDir::inputDirPath").toString();
            setInputDirectory(inputDirPath);

            outputDirPath = settings.value("DirToDir::outputDirPath").toString();
            setOutputDirectory(outputDirPath);
        }
        ~DirToDir()
        {
            settings.setValue("DirToDir::inputDirPath",inputDirPath);
            settings.setValue("DirToDir::outputDirPath",outputDirPath);
        }


        void setInputDirectory(QString inputDirectory)
        {
            inputDirPath = inputDirectory;

            QDirIterator it(inputDirPath, QDir::Files ,QDirIterator::Subdirectories);
            while (it.hasNext()) {
                inputFilesFullPathList.push_back(it.next());
            }
        }

        QString getInputDirectory()
        {
            return inputDirPath;
        }

        void setOutputDirectory(QString outputDirectory)
        {
            outputDirPath = outputDirectory;
        }

        QString getOutputDirectory()
        {
            return outputDirPath;
        }

        QMap<QString,QString> getOutputFiles()
        {
            outputFiles.clear();
            for(int i = 0; i < inputFilesFullPathList.size(); i++)
            {
                QString inFileFullPath =  inputFilesFullPathList.at(i);
                QString tmp = inFileFullPath;
                QString outFileFullPath = tmp.replace(0,inputDirPath.size(),outputDirPath) + ".hpp";
                outputFiles[inFileFullPath] = outFileFullPath;
            }
            return outputFiles;
        }



        void print()
        {
            qDebug() << "--------DirToDir--------";
            qDebug() << "   inputDirPath = " << inputDirPath;
            qDebug() << " ";
            for(int i = 0; i < inputFilesFullPathList.size(); i++)
            {
                qDebug() << "   " << inputFilesFullPathList.at(i);
            }
            qDebug() << " ";
            qDebug() << "   outputDirPath = " << outputDirPath;
            qDebug() << " ";
            QMapIterator<QString, QString> i(outputFiles);
            while (i.hasNext()) {
                i.next();
                qDebug() << "   " << i.key() << " --->> " << i.value();
            }
            qDebug() << "------------------------";
        }
    private:
        QString inputDirPath;
        QStringList inputFilesFullPathList;

        QMap<QString, QString> outputFiles;
        QString outputDirPath;
        QSettings settings;
    };


    FileToFile fileToFile;
    DirToDir dirToDir;
};

#endif // WIDGET_HPP
