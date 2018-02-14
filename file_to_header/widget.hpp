#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <QWidget>
#include <string>
#include <QSettings>

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

private:
    QSettings settings;
    uint32_t loadFileToVector(string fileName, std::vector<char> &buffer);
    void generateHeaderFile(string headerFileDir, string headerFileName, std::vector<char> &buffer);
    string remove_extension(const string& filename);
    Ui::Widget *ui;
    QString fileName;
    QString openFileDir;
};

#endif // WIDGET_HPP
