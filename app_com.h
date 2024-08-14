#ifndef APP_COM_H
#define APP_COM_H

#define FRAME_HEADER_SIZE_WORD 5
unsigned char data_length[] = {
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


typedef struct
{
    unsigned int Preamble;
    unsigned int Seq_No;
    unsigned int Msg_Id;
    unsigned int Data_Length;
    unsigned int Check_Sum;
    unsigned int msgData[45];

} Frame_t;

void App_Comm_Init();
void buildFrame(Frame_t *pFrame, Msg_Register_t msgId);
void sendFrame(Frame_t frame);
void receiveFrame(void);
void *App_Comm_Send(void *arg);
void *App_Comm_Receive(void *arg);
unsigned int frame_Check_Sum(Frame_t *frameInfo);
Frame_t getRegisterG(void);
Frame_t getRegisterS(void);
Frame_t getRegisterVE(void);

#endif // APP_COM_INVERTER_H
