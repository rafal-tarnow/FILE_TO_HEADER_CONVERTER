#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <QWidget>

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
    Ui::Widget *ui;
    QString fileName;
};

#endif // WIDGET_HPP
