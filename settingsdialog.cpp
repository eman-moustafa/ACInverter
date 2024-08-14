#include "settingsdialog.h"
#include "ui_settingsdialog.h"


#include <QIntValidator>
#include <QLineEdit>
#include <QSerialPortInfo>

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");


SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SettingsDialog),
    m_intValidator(new QIntValidator(0, 4000000, this))
{
    m_ui->setupUi(this);

    m_ui->baudRateBox_2_2->setInsertPolicy(QComboBox::NoInsert);

    connect(m_ui->applyButton_2, &QPushButton::clicked,
            this, &SettingsDialog::apply);
    connect(m_ui->serialPortInfoListBox_2, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsDialog::showPortInfo);
    connect(m_ui->baudRateBox_2_2,  QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsDialog::checkCustomBaudRatePolicy);
    connect(m_ui->serialPortInfoListBox_2, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsDialog::checkCustomDevicePathPolicy);

    fillPortsParameters();
    fillPortsInfo();
    updateSettings();
}

SettingsDialog::~SettingsDialog()
{
    delete m_ui;
}

SettingsDialog::Settings SettingsDialog::settings() const
{
    return m_currentSettings;
}

void SettingsDialog::showPortInfo(int idx)
{
    if (idx == -1)
        return;

    const QStringList list = m_ui->serialPortInfoListBox_2->itemData(idx).toStringList();
    m_ui->descriptionLabel_2->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
    m_ui->manufacturerLabel_2->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
    m_ui->serialNumberLabel_2->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
    m_ui->locationLabel_2->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : tr(blankString)));
    m_ui->vidLabel_2->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
    m_ui->pidLabel_2->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : tr(blankString)));
}

void SettingsDialog::apply()
{
    updateSettings();
    hide();
}

void SettingsDialog::checkCustomBaudRatePolicy(int idx)
{
    const bool isCustomBaudRate = !m_ui->baudRateBox_2_2->itemData(idx).isValid();
    m_ui->baudRateBox_2_2->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        m_ui->baudRateBox_2_2->clearEditText();
        QLineEdit *edit = m_ui->baudRateBox_2_2->lineEdit();
        edit->setValidator(m_intValidator);
    }
}

void SettingsDialog::checkCustomDevicePathPolicy(int idx)
{
    const bool isCustomPath = !m_ui->serialPortInfoListBox_2->itemData(idx).isValid();
    m_ui->serialPortInfoListBox_2->setEditable(isCustomPath);
    if (isCustomPath)
        m_ui->serialPortInfoListBox_2->clearEditText();
}

void SettingsDialog::fillPortsParameters()
{
    m_ui->baudRateBox_2_2->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    m_ui->baudRateBox_2_2->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    m_ui->baudRateBox_2_2->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    m_ui->baudRateBox_2_2->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    m_ui->baudRateBox_2_2->addItem(tr("Custom"));

    m_ui->dataBitsBox_2->addItem(QStringLiteral("5"), QSerialPort::Data5);
    m_ui->dataBitsBox_2->addItem(QStringLiteral("6"), QSerialPort::Data6);
    m_ui->dataBitsBox_2->addItem(QStringLiteral("7"), QSerialPort::Data7);
    m_ui->dataBitsBox_2->addItem(QStringLiteral("8"), QSerialPort::Data8);
    m_ui->dataBitsBox_2->setCurrentIndex(3);

    m_ui->parityBox_2->addItem(tr("None"), QSerialPort::NoParity);
    m_ui->parityBox_2->addItem(tr("Even"), QSerialPort::EvenParity);
    m_ui->parityBox_2->addItem(tr("Odd"), QSerialPort::OddParity);
    m_ui->parityBox_2->addItem(tr("Mark"), QSerialPort::MarkParity);
    m_ui->parityBox_2->addItem(tr("Space"), QSerialPort::SpaceParity);

    m_ui->stopBitsBox_2->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    m_ui->stopBitsBox_2->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    m_ui->stopBitsBox_2->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    m_ui->flowControlBox_2->addItem(tr("None"), QSerialPort::NoFlowControl);
    m_ui->flowControlBox_2->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    m_ui->flowControlBox_2->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void SettingsDialog::showSettings(){
    fillPortsInfo();
    this->show();
}
void SettingsDialog::fillPortsInfo()
{
    m_ui->serialPortInfoListBox_2->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        m_ui->serialPortInfoListBox_2->addItem(list.first(), list);
    }

    m_ui->serialPortInfoListBox_2->addItem(tr("Custom"));
}

void SettingsDialog::updateSettings()
{
    m_currentSettings.name = m_ui->serialPortInfoListBox_2->currentText();

    if (m_ui->baudRateBox_2_2->currentIndex() == 4) {
        m_currentSettings.baudRate = m_ui->baudRateBox_2_2->currentText().toInt();
    } else {
        m_currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    m_ui->baudRateBox_2_2->itemData(m_ui->baudRateBox_2_2->currentIndex()).toInt());
    }
    m_currentSettings.stringBaudRate = QString::number(m_currentSettings.baudRate);

    m_currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                m_ui->dataBitsBox_2->itemData(m_ui->dataBitsBox_2->currentIndex()).toInt());
    m_currentSettings.stringDataBits = m_ui->dataBitsBox_2->currentText();

    m_currentSettings.parity = static_cast<QSerialPort::Parity>(
                m_ui->parityBox_2->itemData(m_ui->parityBox_2->currentIndex()).toInt());
    m_currentSettings.stringParity = m_ui->parityBox_2->currentText();

    m_currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                m_ui->stopBitsBox_2->itemData(m_ui->stopBitsBox_2->currentIndex()).toInt());
    m_currentSettings.stringStopBits = m_ui->stopBitsBox_2->currentText();

    m_currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                m_ui->flowControlBox_2->itemData(m_ui->flowControlBox_2->currentIndex()).toInt());
    m_currentSettings.stringFlowControl = m_ui->flowControlBox_2->currentText();

    m_currentSettings.localEchoEnabled = m_ui->localEchoCheckBox_2->isChecked();
}

