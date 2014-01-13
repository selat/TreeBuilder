#ifndef NEWLEAFTYPEWIDGET_H
#define NEWLEAFTYPEWIDGET_H

#include <QtWidgets>

class NewLeafTypeWidget : public QDialog
{
    Q_OBJECT
public:
    explicit NewLeafTypeWidget(QWidget *parent = 0);

    bool accepted();
    QString getName();
    double getCoefficient();
    QString getTexture();

    void setName(QString &value);
    void setCoefficient(double value);
    void setTexture(QString &value);
    
signals:
    void closed();
    
public slots:
    
protected:
    void showEvent(QShowEvent *event);

private slots:
    void loadLeafTexture();
    void okButtonClicked();
    void cancelButtonClicked();

private:
    void createWidgets();
    void createActions();

    QLineEdit *leafName;
    QDoubleSpinBox *leafCoefficient;
    QLineEdit *leafTexture;
    QPushButton *loadLeafTextureButton;

    QPushButton *okButton;
    QPushButton *cancelButton;

    bool isAccepted;
};

#endif // NEWLEAFTYPEWIDGET_H
