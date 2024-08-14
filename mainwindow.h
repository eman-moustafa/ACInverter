#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QMovie>
#include <QInputDialog>
#include <QTimer>
#include <QToolTip>
#include "data.h"
#include"settingsdialog.h"
#include <QDateTime>

#include <QScreen>
#include <QPixmap>
#include <QFileDialog>




QT_BEGIN_NAMESPACE

class QLabel;

namespace Ui{
class MainWindow;
}

QT_END_NAMESPACE

class Console;
class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



    QList<QByteArray> splitData;

    typedef struct
    {
        unsigned int Preamble;
        unsigned int Seq_No;
        unsigned int Msg_Id;
        unsigned int Data_Length;
        unsigned int Check_Sum;
        unsigned int msgData[45];

    } Frame_t;


    unsigned int *pframeG = (unsigned int *)&frameGrx;
    unsigned int *pframeS = (unsigned int *)&frameSrx;
    unsigned int *pframeVE = (unsigned int *)&frameVErx;

#define FRAME_HEADER_SIZE_WORD 5
unsigned char data_length[3] = {
    25U, // General Register
    45U, // Specific Register
    10U  // Vector Engine Control Register
};

    typedef enum
    {
        Msg_G_Register = 0, // General Register
        Msg_S_Register,     // Specific Register
        Msg_VE_Register,    // Vector Engine Control Register
        max_message_number
    } Msg_Register_t;


typedef enum
{
    PMDxMDEN = 0,
    PMDxPORTMD,
    PMDxMDCR,
    PMDxCNTSTA,
    PMDxMDCNT,
    PMDxMDPRD,
    PMDxCMPU,
    PMDxCMPV,
    PMDxCMPW,
    PMDxMODESEL,
    PMDxMDOUT,
    PMDxMDPOT,
    PMDxEMGREL,
    PMDxEMGCR,
    PMDxEMGSTA,
    PMDxOVVCR,
    PMDxOVVSTA,
    PMDxDTR,
    PMDxTRGCMP0,
    PMDxTRGCMP1,
    PMDxTRGCMP2,
    PMDxTRGCMP3,
    PMDxTRGCR,
    PMDxTRGMD,
    PMDxTRGSEL
} e_msg_g;

typedef enum
{
    VEMCTLFx = 0,
    VEMODEx,
    VEFMODEx,
    VETPWMx,
    VEOMEGAx,
    VETHETAx,
    VEIDREFx,
    VEIQREFx,
    VEVDx,
    VEVQx,
    VECIDKIx,
    VECIDKPx,
    VECIQKIx,
    VECIQKPx,
    VEVDIHx,
    VEVDILHx,
    VEVQIHx,
    VEVQILHx,
    VEFPWMCHGx,
    VEMDPRDx,
    VEMINPLSx,
    VETRGCRCx,
    VECOSx,
    VESINx,
    VECOSMx,
    VESINMx,
    VESECTORx,
    VESECTORMx,
    VEIAOx,
    VEIBOx,
    VEICOx,
    VEIAADCx,
    VEIBADCx,
    VEICADCx,
    VEVDCx,
    VEIDx,
    VEIQx,
    VECMPUx,
    VECMPVx,
    VECMPWx,
    VEOUTCRx,
    VETRGCMP0x,
    VETRGCMP1x,
    VETRGSELx,
    VEEMGRSx,
} e_msg_s;

typedef enum
{
    VEEN = 0,
    VECPURUNTRG,
    VETASKAPP,
    VEACTSCH,
    VEREPTIME,
    VETRGMODE,
    VEERRINTEN,
    VECOMPEND,
    VEERRDET,
    VESCHTASKRUN
} e_msg_ve;
private slots:
    void openSerialPort();
    void closeSerialPort();
    void about();
    void ClearData();
    void writeData(const QByteArray &data);
    void readData();
    void updateData();
    void initActionsConnections();

    void showStatusMessage(const QString &message);

    void ShowResponse(QString info);

    void on_pushButton_clicked();


    void receiveFrame();

    void  sendFrame(const Frame_t frame);

    void buildFrame(Frame_t *pframe, Msg_Register_t msgId);

    unsigned int  frame_Check_Sum(Frame_t *frameInfo);

    QString numberToString(int number) ;



private:
    Ui::MainWindow *ui;
    QLabel *m_status = nullptr;
    Console *m_console = nullptr;
    SettingsDialog *m_settings = nullptr;
    Data *m_Data=nullptr;
    QSerialPort *m_serial = nullptr;

    #define BUFFER_SIZE 115
    unsigned int data_buffer[BUFFER_SIZE] = {0};
    unsigned int seqNoCounter = 0;
    Frame_t frameInfoTx;
    Frame_t frameGrx;
    Frame_t frameSrx;
    Frame_t frameVErx;



};
#endif // MAINWINDOW_H
