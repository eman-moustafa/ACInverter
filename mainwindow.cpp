#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "settingsdialog.h"
#include "QDebug"
#include <QMessageBox>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    m_status(new QLabel),
    m_settings(new SettingsDialog),
    m_Data(new Data),
    m_serial(new QSerialPort(this))
{
    ui->setupUi(this);

    ui->actionConnect->setEnabled(true);
    ui->actionDisConnect->setEnabled(false);
    ui->actionConfig->setEnabled(true);
    ui->actionClear->setEnabled(false);

    static unsigned int seqNoCounter = 0;


    initActionsConnections();

    connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readData);


}

void MainWindow::initActionsConnections()
{

    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionDisConnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(ui->actionConfig, &QAction::triggered, m_settings, &SettingsDialog::showSettings);
    connect(ui->actionClear, &QAction::triggered, this, &MainWindow::ClearData);



}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openSerialPort()
{

    const SettingsDialog::Settings p = m_settings->settings();
    m_serial->setPortName(p.name);
    m_serial->setBaudRate(p.baudRate);
    m_serial->setDataBits(p.dataBits);
    m_serial->setParity(p.parity);
    m_serial->setStopBits(p.stopBits);
    m_serial->setFlowControl(p.flowControl);
    if (m_serial->open(QIODevice::ReadWrite)) {

        qDebug()<<"Open serialport";
        ui->actionConnect->setEnabled(false);
        ui->actionDisConnect->setEnabled(true);
        ui->actionClear->setEnabled(true);
        ui->actionConfig->setEnabled(false);
       showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));

} else {
        QMessageBox::critical(this, tr("Error"), m_serial->errorString());

        showStatusMessage(tr("Open error"));
        qDebug()<<"eror";
    }
}

//! [4]

//! [5]
void MainWindow::closeSerialPort()
{
    if (m_serial->isOpen())
        m_serial->close();
    ui->actionConnect->setEnabled(true);
    ui->actionDisConnect->setEnabled(false);
    ui->actionConfig->setEnabled(true);
    ui->actionClear->setEnabled(true);

    showStatusMessage(tr("Disconnected"));
}
//! [5]

QString MainWindow:: numberToString(int number) {
    return QString::number(number);
}
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Simple Coffee Maker Monitor"),
                       tr("The <b>Simple Coffee Maker Monitor</b> example demonstrates how to "
                          "use the Qt Serial Port module in modern GUI applications "
                          "using Qt, with a menu bar, toolbars, and a status bar."));
}

//! [6]
void MainWindow::writeData(const QByteArray &data)
{
    m_serial->write(data);
    qDebug()<<"Write Function";
}


void MainWindow::readData()
{
    qDebug()<<"read data";


    if (m_serial->canReadLine()) {
        const QByteArray data = m_serial->readLine();
        qDebug() << data << endl;

        QList<QByteArray> splitData = data.split(',');
        for (int i = 0; i < splitData.length(); i++) {
            bool ok;
            data_buffer[i] = splitData[i].toInt(&ok);
            if (ok) {
                qDebug() << "data buffer" << data_buffer[i];
            } else {
                qDebug() << "Failed to convert" << splitData[i] << "to integer";
            }
        }

        receiveFrame();
        updateData();
    }

}
void MainWindow::receiveFrame()
{
    qDebug()<<"receiveFrame";

    for (int i = 1; i < BUFFER_SIZE; i++) {
        data_buffer[i] = static_cast<int>(data_buffer[i]);
        //qDebug() << "DATA" << data_buffer[i];
    }

    static char msgCount = 0;

    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        if (data_buffer[i] == 0xABCD)
        {
            if (data_buffer[i + 2] == Msg_G_Register)
            {
                for (int j = 0; j < (FRAME_HEADER_SIZE_WORD + data_length[0]) && (i + j) < BUFFER_SIZE; j++)
                {
                    qDebug()<<"Data "<<data_buffer[j];

                    pframeG[j] = data_buffer[i + j];
                }


                m_Data->PMDEnableRegister=numberToString(data_buffer[4]);

                m_Data->PortOutputModeRegister= numberToString(data_buffer[5]);

                m_Data->ModeSelectRegister=numberToString( data_buffer[13]);

                m_Data->PMDControlRegister=numberToString(data_buffer[6]);

                m_Data->PWMCounterStatusRegister=numberToString(data_buffer[7]);

                m_Data->PWMCounterRegister=numberToString(data_buffer[8]);

                m_Data->PWMCycleRegister=numberToString(data_buffer[9]);

                m_Data->PWMCompareURegister=numberToString(data_buffer[10]);

                m_Data->PWMCompareVRegister=numberToString(data_buffer[11]);

                m_Data->PWMCompareWRegister=numberToString(data_buffer[12]);

                m_Data->PMDOutputControlRegister=numberToString(data_buffer[14]);

                m_Data->PMDOutputSettingRegister=numberToString(data_buffer[15]);

                m_Data->EMGControlRegister=numberToString(data_buffer[17]);

                m_Data->EMGStatusRegister=numberToString(data_buffer[18]);

                m_Data->OVVControlRegister=numberToString(data_buffer[19]);

                m_Data->OVVStatusRegister=numberToString(data_buffer[20]);

                m_Data->DeadTimeRegister=numberToString(data_buffer[21]);

                m_Data->TriggerCompare0Register=numberToString(data_buffer[20]);

                m_Data->TriggerCompare1Register=numberToString(data_buffer[23]);

                m_Data->TriggerCompare2Register=numberToString(data_buffer[24]);

                m_Data->TriggerCompare3Register=numberToString(data_buffer[25]);

                m_Data->TriggerControlRegister=numberToString(data_buffer[26]);

                m_Data->TriggerOutputModeSettingRegister=numberToString(data_buffer[27]);

                m_Data->TriggerOutputSelectRegister=numberToString(data_buffer[28]);

                i += FRAME_HEADER_SIZE_WORD + data_length[0] - 1;
            }
            else if (data_buffer[i + 2] == Msg_S_Register)
            {
                for (int j = 0; j < (FRAME_HEADER_SIZE_WORD + data_length[1]) && (i + j) < BUFFER_SIZE; j++)
                {
                    pframeS[j] = data_buffer[i + j];
                }
                i += FRAME_HEADER_SIZE_WORD + data_length[1] - 1;
            }
            else if ( data_buffer[i + 2] == Msg_VE_Register)
            {
                for (int j = 0; j < (FRAME_HEADER_SIZE_WORD + data_length[2]) && (i + j) < BUFFER_SIZE; j++)
                {
                    pframeVE[j] = data_buffer[i + j];
                }
                i += FRAME_HEADER_SIZE_WORD + data_length[2] - 1;
            }

        }
    }
}

void MainWindow ::updateData()
{


    qDebug()<<"Enter update";
    ui->r_PMDEnableRegister->setText(m_Data->PMDEnableRegister );

    ui->r_PortOutputModeRegister->setText( m_Data->PortOutputModeRegister);

    ui->r_ModeSelectRegister->setText( m_Data->ModeSelectRegister);

    ui->rPMDControlRegister->setText( m_Data->PMDControlRegister);

    ui->rPWMCounterStatusRegister->setText( m_Data->PWMCounterStatusRegister);

    ui->rPWMCounterRegister->setText(m_Data->PWMCounterRegister);

    ui->rPWMCycleRegister->setText( m_Data->PWMCycleRegister);

    ui->rPWMCompareURegister->setText( m_Data->PWMCompareURegister);

    ui->rPWMCompareVRegister->setText( m_Data->PWMCompareVRegister);

    ui->rPWMCompareWRegister->setText( m_Data->PWMCompareWRegister);

    ui->rPMDOutputControlRegister->setText(m_Data->PMDOutputControlRegister);

    ui->rPMDOutputSettingRegister->setText(m_Data->PMDOutputSettingRegister);

    ui->rEMGControlRegister->setText(m_Data->EMGControlRegister);

    ui->rEMGStatusRegister->setText(m_Data->EMGStatusRegister );

    ui->rOVVControlRegister->setText(m_Data-> OVVControlRegister);

    ui->rOVVStatusRegister->setText( m_Data->OVVStatusRegister);

    ui->rDeadTimeRegister->setText(m_Data->DeadTimeRegister);

    ui->rTriggerCompare0Register->setText(m_Data->TriggerCompare0Register );

    ui->rTriggerCompare1Register->setText(m_Data->TriggerCompare1Register );

    ui->rTriggerCompare2Register->setText( m_Data->TriggerCompare2Register);

    ui->rTriggerCompare3Register->setText(m_Data->TriggerCompare3Register);

    ui->rTriggerControlRegister->setText( m_Data->TriggerControlRegister);

    ui->rTriggerOutputModeSettingRegister->setText(m_Data->TriggerOutputModeSettingRegister );

    ui->rTriggerOutputSelectRegister->setText(m_Data->TriggerOutputSelectRegister);



}


void MainWindow:: buildFrame(Frame_t *pframe, Msg_Register_t msgId)
{
    qDebug()<<"Enter build frame";
    int i;
    pframe->Preamble = 0xABCD;
    pframe->Seq_No = seqNoCounter;
    pframe->Msg_Id = (unsigned int)msgId;
    pframe->Data_Length = data_length[msgId];

    if (msgId == Msg_G_Register)
    {

        pframe->msgData[PMDxMDEN] = ui->w_PMDEnableRegister->toPlainText().toInt();
        pframe->msgData[PMDxPORTMD] =ui->w_PortOutputModeRegister->toPlainText().toInt();
        pframe->msgData[PMDxMDCR]=ui->wPMDControlRegister->toPlainText().toInt();
      //  pframe->msgData[PMDxCNTSTA]=; ///pwm counter state register "Read "
      //  pframe->msgData[PMDxMDCNT]=; ///pwm counter register "read"
        pframe->msgData[PMDxMDPRD]=ui->wPWMCycleRegister->toPlainText().toInt();//
        pframe->msgData[PMDxCMPU]=ui->wPWMCompareURegister->toPlainText().toInt();
        pframe->msgData[PMDxCMPV]=ui->wPWMCompareVRegister->toPlainText().toInt();
        pframe->msgData[PMDxCMPW]=ui->wPWMCompareWRegister->toPlainText().toInt();
        pframe->msgData[PMDxMODESEL]=ui->w_ModeSelectRegister->toPlainText().toInt();
        pframe->msgData[PMDxMDOUT]=ui->wPMDOutputControlRegister->toPlainText().toInt();
        pframe->msgData[PMDxMDPOT]=ui->wPMDOutputSettingRegister->toPlainText().toInt();
        pframe->msgData[PMDxEMGREL]=ui->wEMGReleaseRegister->toPlainText().toInt();
        pframe->msgData[PMDxEMGCR]=ui->wEMGControlRegister->toPlainText().toInt();
      //pframe->msgData[PMDxEMGSTA]=; ///EMG status register "read "
        pframe->msgData[PMDxOVVCR]=ui->wOVVControlRegister->toPlainText().toInt();
        //pframe->msgData[PMDxOVVSTA]=;  ///ovv status register "read"
        pframe->msgData[PMDxDTR]=ui->wDeadTimeRegister->toPlainText().toInt();
        pframe->msgData[PMDxTRGCMP0]=ui->wTriggerCompare0Register->toPlainText().toInt();
        pframe->msgData[PMDxTRGCMP1]=ui->wTriggerCompare1Register->toPlainText().toInt();
        pframe->msgData[PMDxTRGCMP2]=ui->wTriggerCompare2Register->toPlainText().toInt();
        pframe->msgData[PMDxTRGCMP3]=ui->wTriggerCompare3Register->toPlainText().toInt();
        pframe->msgData[PMDxTRGCR]=ui->wTriggerControlRegister->toPlainText().toInt();
        pframe->msgData[PMDxTRGMD]=ui->wTriggerOutputModeSettingRegister->toPlainText().toInt();
        pframe->msgData[PMDxTRGSEL]=ui->wTriggerOutputSelectRegister->toPlainText().toInt();
        QString combinedData;

        combinedData += QString::number( pframe->Preamble)+ ",";
        combinedData += QString::number(seqNoCounter)+ ",";
        combinedData += QString::number( msgId)+ ",";
        combinedData += QString::number(data_length[msgId])+ ",";
        combinedData += QString::number(pframe->msgData[PMDxMDEN]) + ",";
        combinedData += QString::number(pframe->msgData[PMDxPORTMD]) + ",";
        combinedData += QString::number(pframe->msgData[PMDxMDCR]) + ",";
        combinedData += QString::number(pframe->msgData[PMDxMDPRD]) + ",";
        combinedData += QString::number(pframe->msgData[PMDxCMPU]) + ",";
        combinedData += QString::number(pframe->msgData[PMDxCMPV]) + ",";
        combinedData += QString::number(pframe->msgData[PMDxCMPW]) + ",";
        combinedData += QString::number(pframe->msgData[PMDxMODESEL]) + ",";
        combinedData += QString::number(pframe->msgData[PMDxMDOUT]) + ",";
        combinedData += QString::number(pframe->msgData[PMDxMDPOT]) + ",";
        combinedData += QString::number( pframe->msgData[PMDxEMGREL]) + ",";
        combinedData += QString::number( pframe->msgData[PMDxEMGCR]) + ",";
        combinedData += QString::number( pframe->msgData[PMDxOVVCR]) + ",";
        combinedData += QString::number( pframe->msgData[PMDxDTR]) + ",";
        combinedData += QString::number( pframe->msgData[PMDxTRGCMP0]) + ",";
        combinedData += QString::number( pframe->msgData[PMDxTRGCMP1]) + ",";
        combinedData += QString::number( pframe->msgData[PMDxTRGCMP2]) + ",";
        combinedData += QString::number( pframe->msgData[PMDxTRGCMP3]) + ",";
        combinedData += QString::number( pframe->msgData[PMDxTRGCR]) + ",";
        combinedData += QString::number( pframe->msgData[PMDxTRGMD]) + ",";
        combinedData += QString::number( pframe->msgData[PMDxTRGSEL]);
        combinedData += "\n";

        QByteArray dataToWrite = combinedData.toUtf8(); // Use toUtf8() for encoding
       // qDebug()<<"dataToWrite"<<dataToWrite;
        qint64 bytesWritten = m_serial->write(dataToWrite);
          if (bytesWritten != dataToWrite.size()) {
              qDebug() << "Error writing data:" << m_serial->errorString();
          }



          QList<QByteArray> splitData = dataToWrite.split(',');
          for (int i = 0; i < splitData.length(); i++) {
              bool ok;
              data_buffer[i] = splitData[i].toInt(&ok);
              if (ok) {
                  qDebug() << "data buffer" << data_buffer[i];
              } else {
                  qDebug() << "Failed to convert" << splitData[i] << "to integer";
              }
          }




    }
    else if (msgId == Msg_S_Register)
    {
        pframe->msgData[VEMCTLFx] = 0x1BBBBBAA;
        pframe->msgData[VEMODEx] = 0x2BBBBBAA;
        pframe->msgData[VEFMODEx] = 0x3BBBBBAA;
        pframe->msgData[VETPWMx] = 0x4BBBBBAA;
        pframe->msgData[VEOMEGAx] = 0x5BBBBBAA;
        pframe->msgData[VETHETAx] = 0x6BBBBBAA;
        pframe->msgData[VEIDREFx] = 0x7BBBBBAA;
        pframe->msgData[VEIQREFx] = 0x8BBBBBAA;
        pframe->msgData[VEVDx] = 0x9BBBBBAA;
        pframe->msgData[VEVQx] = 0x10BBBBAA;
        pframe->msgData[VECIDKIx] = 0x11BBBBAA;
        pframe->msgData[VECIDKPx] = 0x12BBBBAA;
        pframe->msgData[VECIQKIx] = 0x13BBBBAA;
        pframe->msgData[VECIQKPx] = 0x14BBBBAA;
        pframe->msgData[VEVDIHx] = 0x15BBBBAA;
        pframe->msgData[VEVDILHx] = 0x16BBBBAA;
        pframe->msgData[VEVQIHx] = 0x17BBBBAA;
        pframe->msgData[VEVQILHx] = 0x18BBBBAA;
        pframe->msgData[VEFPWMCHGx] = 0x19BBBBAA;
        pframe->msgData[VEMDPRDx] = 0x20BBBBAA;
        pframe->msgData[VEMINPLSx] = 0x21BBBBAA;
        pframe->msgData[VETRGCRCx] = 0x22BBBBAA;
        pframe->msgData[VECOSx] = 0x23BBBBAA;
        pframe->msgData[VESINx] = 0x24BBBBAA;
        pframe->msgData[VECOSMx] = 0x25BBBBAA;
        pframe->msgData[VESINMx] = 0x26BBBBAA;
        pframe->msgData[VESECTORx] = 0x27BBBBAA;
        pframe->msgData[VESECTORMx] = 0x28BBBBAA;
        pframe->msgData[VEIAOx] = 0x29BBBBAA;
        pframe->msgData[VEIBOx] = 0x30BBBBAA;
        pframe->msgData[VEICOx] = 0x31BBBBAA;
        pframe->msgData[VEIAADCx] = 0x32BBBBAA;
        pframe->msgData[VEIBADCx] = 0x33BBBBAA;
        pframe->msgData[VEICADCx] = 0x34BBBBAA;
        pframe->msgData[VEVDCx] = 0x35BBBBAA;
        pframe->msgData[VEIDx] = 0x36BBBBAA;
        pframe->msgData[VEIQx] = 0x37BBBBAA;
        pframe->msgData[VECMPUx] = 0x38BBBBAA;
        pframe->msgData[VECMPVx] = 0x39BBBBAA;
        pframe->msgData[VECMPWx] = 0x40BBBBAA;
        pframe->msgData[VEOUTCRx] = 0x41BBBBAA;
        pframe->msgData[VETRGCMP0x] = 0x42BBBBAA;
        pframe->msgData[VETRGCMP1x] = 0x43BBBBAA;
        pframe->msgData[VETRGSELx] = 0x44BBBBAA;
        pframe->msgData[VEEMGRSx] = 0x45BBBBAA;
    }
    else if (msgId == Msg_VE_Register)
    {
        pframe->msgData[VEEN] = 0x1CCCCCEE;
        pframe->msgData[VECPURUNTRG] = 0x2CCCCCEE;
        pframe->msgData[VETASKAPP] = 0x3CCCCCEE;
        pframe->msgData[VEACTSCH] = 0x4CCCCCEE;
        pframe->msgData[VEREPTIME] = 0x5CCCCCEE;
        pframe->msgData[VETRGMODE] = 0x6CCCCCEE;
        pframe->msgData[VEERRINTEN] = 0x7CCCCCEE;
        pframe->msgData[VECOMPEND] = 0x8CCCCCEE;
        pframe->msgData[VEERRDET] = 0x9CCCCCEE;
        pframe->msgData[VESCHTASKRUN] = 0x10CCCCEE;
    }

    pframe->Check_Sum = frame_Check_Sum(pframe);
}

void MainWindow:: sendFrame(const Frame_t frame)
{
    unsigned short transmitFrameIndex = 0;
    unsigned int *pframe = (unsigned int *)&frame;
    static unsigned int startFrame = 0;
     unsigned char i;
    // copy frame into BUFFER
    for (transmitFrameIndex = 0; transmitFrameIndex < (FRAME_HEADER_SIZE_WORD + frame.Data_Length); transmitFrameIndex++, startFrame++)
    {
        data_buffer[startFrame] = pframe[transmitFrameIndex];
    }
        startFrame += ((sizeof(Frame_t)/sizeof(int))-(FRAME_HEADER_SIZE_WORD + frame.Data_Length));
        if(startFrame>=BUFFER_SIZE){
            startFrame=0;
        }

}


unsigned int MainWindow:: frame_Check_Sum(Frame_t *frameInfo)
{
    int sum =
        frameInfo->Seq_No +
        frameInfo->Msg_Id +
        frameInfo->Data_Length;
    for (unsigned char i = 0; i < frameInfoTx.Data_Length; i++)
    {
        sum += frameInfoTx.msgData[i];
    }
    sum = ((0xff) & sum);
    sum = 0x100 - sum;
    return sum;
}


void MainWindow::showStatusMessage(const QString &message)
{
    m_status->setText(message);
}

void MainWindow::ShowResponse(QString info){
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(info);
    msgBox.exec();
}


void MainWindow::ClearData()
{
    ui->r_PMDEnableRegister->setText(NULL);
    ui->r_PortOutputModeRegister->setText(NULL);
    ui->r_ModeSelectRegister->setText(NULL);
    ui->rPMDControlRegister->setText(NULL);
    ui->rPWMCounterStatusRegister->setText(NULL);
    ui->rPWMCounterRegister->setText(NULL);
    ui->rPWMCycleRegister->setText(NULL);
    ui->rPWMCompareURegister->setText(NULL);
    ui->rPWMCompareVRegister->setText(NULL);
    ui->rPWMCompareWRegister->setText(NULL);
    ui->rPMDOutputControlRegister->setText(NULL);
    ui->rPMDOutputSettingRegister->setText(NULL);
    ui->rEMGControlRegister->setText(NULL);
    ui->rEMGStatusRegister->setText(NULL);
    ui->rOVVControlRegister->setText(NULL);
    ui->rOVVStatusRegister->setText(NULL);
    ui->rDeadTimeRegister->setText(NULL);
    ui->rTriggerCompare0Register->setText(NULL);
    ui->rTriggerCompare1Register->setText(NULL);
    ui->rTriggerCompare2Register->setText(NULL);
    ui->rTriggerCompare3Register->setText(NULL);
    ui->rTriggerControlRegister->setText(NULL);
    ui->rTriggerOutputModeSettingRegister->setText(NULL);
    ui->rTriggerOutputSelectRegister->setText(NULL);


    ui->w_PMDEnableRegister->setText(NULL);
    ui->w_PortOutputModeRegister->setText(NULL);
    ui->w_ModeSelectRegister->setText(NULL);
    ui->wPMDControlRegister->setText(NULL);
    ui->wPWMCycleRegister->setText(NULL);
    ui->wPWMCompareURegister->setText(NULL);
    ui->wPWMCompareVRegister->setText(NULL);
    ui->wPWMCompareWRegister->setText(NULL);
    ui->wPMDOutputControlRegister->setText(NULL);
    ui->wPMDOutputSettingRegister->setText(NULL);
    ui->wEMGControlRegister->setText(NULL);
    ui->wEMGReleaseRegister->setText(NULL);
    ui->wOVVControlRegister->setText(NULL);
    ui->wDeadTimeRegister->setText(NULL);
    ui->wTriggerCompare0Register->setText(NULL);
    ui->wTriggerCompare1Register->setText(NULL);
    ui->wTriggerCompare2Register->setText(NULL);
    ui->wTriggerCompare3Register->setText(NULL);
    ui->wTriggerControlRegister->setText(NULL);
    ui->wTriggerOutputModeSettingRegister->setText(NULL);
    ui->wTriggerOutputSelectRegister->setText(NULL);


}

void MainWindow::on_pushButton_clicked()
{
/*

            m_Data->PMDEnableRegister = ui->w_PMDEnableRegister->toPlainText();

            m_Data->PortOutputModeRegister = ui->w_PortOutputModeRegister->toPlainText();

            m_Data->ModeSelectRegister = ui->w_ModeSelectRegister->toPlainText();

            m_Data->PMDControlRegister = ui->wPMDControlRegister->toPlainText();

            m_Data->PWMCycleRegister = ui->wPWMCycleRegister->toPlainText();

            m_Data->PWMCompareURegister = ui->wPWMCompareURegister->toPlainText();

            m_Data->PWMCompareVRegister = ui->wPWMCompareVRegister->toPlainText();

            m_Data->PWMCompareWRegister = ui->wPWMCompareWRegister->toPlainText();

            m_Data->PMDOutputControlRegister = ui->wPMDOutputControlRegister->toPlainText();



    QString combinedData = m_Data->PMDEnableRegister + "," + m_Data->PortOutputModeRegister
            + "," + m_Data->ModeSelectRegister + "," + m_Data->PMDControlRegister
            + "," + m_Data->PWMCycleRegister   + "," + m_Data->PWMCompareURegister
            + "," + m_Data->PWMCompareVRegister + "," + m_Data->PWMCompareWRegister
            + "," + m_Data->PMDOutputControlRegister+ "\n"; // Add newline for framing
    QByteArray dataToWrite = combinedData.toUtf8(); // Use toUtf8() for encoding
    qint64 bytesWritten = m_serial->write(dataToWrite);
      if (bytesWritten != dataToWrite.size()) {
          qDebug() << "Error writing data:" << m_serial->errorString();
      }
*/

    buildFrame(&frameInfoTx,(Msg_Register_t)0);
    //sendFrame(frameInfoTx);
    qDebug()<<frameGrx.Preamble;
    for (int i=0;i<BUFFER_SIZE ;i++ ) {
       // qDebug()<<"DataBUFFER"<<data_buffer[i];

    }



}

