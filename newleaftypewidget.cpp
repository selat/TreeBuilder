#include "newleaftypewidget.h"

NewLeafTypeWidget::NewLeafTypeWidget(QWidget *parent) :
    QDialog(parent)
{
    setWindowModality(Qt::ApplicationModal);
    createWidgets();
    createActions();
    setFixedSize(minimumSize());
    isAccepted = false;
}

bool NewLeafTypeWidget::accepted()
{
    return isAccepted;
}

QString NewLeafTypeWidget::getName()
{
    return leafName -> text();
}

double NewLeafTypeWidget::getCoefficient()
{
    return leafCoefficient -> value();
}

QString NewLeafTypeWidget::getTexture()
{
    return leafTexture -> text();
}

void NewLeafTypeWidget::setName(QString &value)
{
    leafName -> setText(value);
}

void NewLeafTypeWidget::setCoefficient(double value)
{
    leafCoefficient -> setValue(value);
}

void NewLeafTypeWidget::setTexture(QString &value)
{
    leafTexture -> setText(value);
}

void NewLeafTypeWidget::showEvent(QShowEvent *event)
{
    isAccepted = false;
}

void NewLeafTypeWidget::loadLeafTexture()
{
    leafTexture -> setText(QFileDialog::getOpenFileName(this, tr("Texture load"),
                            QCoreApplication::applicationDirPath(), tr("Images (*.png *.jpg *.bmp)")));
}

void NewLeafTypeWidget::okButtonClicked()
{
    isAccepted = true;
    hide();
    emit closed();
}

void NewLeafTypeWidget::cancelButtonClicked()
{
    isAccepted = false;
    hide();
    emit closed();
}

void NewLeafTypeWidget::createWidgets()
{
    QFormLayout *layout = new QFormLayout(this);

    leafName = new QLineEdit(this);
    leafName -> setText("Leaf");
    layout -> addRow(tr("Name: "), leafName);

    leafCoefficient = new QDoubleSpinBox(this);
    leafCoefficient -> setSingleStep(0.1);
    leafCoefficient -> setMaximum(1000.0);
    leafCoefficient -> setValue(1.0);
    layout -> addRow(tr("Coefficient: "), leafCoefficient);

    leafTexture = new QLineEdit(this);
    leafTexture -> setText(QApplication::applicationDirPath() + "/leaf.png");
    layout -> addRow(tr("Texture: "), leafTexture);

    loadLeafTextureButton = new QPushButton(tr("Load texture"), this);
    layout -> addRow(loadLeafTextureButton);

    QHBoxLayout *okCancelLayout = new QHBoxLayout();
    okButton = new QPushButton(tr("OK"), this);
    okButton -> setDefault(true);
    okCancelLayout -> addWidget(okButton);
    cancelButton = new QPushButton(tr("Cancel"), this);
    okCancelLayout -> addWidget(cancelButton);

    layout -> addRow(okCancelLayout);

    setLayout(layout);
}

void NewLeafTypeWidget::createActions()
{
    connect(loadLeafTextureButton, &QPushButton::pressed, this, &NewLeafTypeWidget::loadLeafTexture);
    connect(okButton, &QPushButton::pressed, this, &NewLeafTypeWidget::okButtonClicked);
    connect(cancelButton, &QPushButton::pressed, this, &NewLeafTypeWidget::cancelButtonClicked);
}
