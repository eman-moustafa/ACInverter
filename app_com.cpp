#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <windows.h>
#include "app_com.h"

#define BUFFER_SIZE 115
unsigned int data_buffer[BUFFER_SIZE] = {0};
static unsigned int seqNoCounter = 0;
Frame_t frameInfoTx;
Frame_t frameGrx;
Frame_t frameSrx;
Frame_t frameVErx;

pthread_mutex_t mutex;
pthread_mutex_t counter;
void buildFrame(Frame_t *pframe, Msg_Register_t msgId)
{
    int i;
    pframe->Preamble = 0xABCD;
    pframe->Seq_No = seqNoCounter;
    pframe->Msg_Id = (unsigned int)msgId;
    pframe->Data_Length = data_length[msgId];

    if (msgId == Msg_G_Register)
    {

        pframe->msgData[PMDxMDEN] = 0x1AAAAAAA;
        pframe->msgData[PMDxPORTMD] = 0x2AAAAAAA;
        pframe->msgData[PMDxMDCR] = 0x3AAAAAAA;
        pframe->msgData[PMDxCNTSTA] = 0x4AAAAAAA;
        pframe->msgData[PMDxMDCNT] = 0x5AAAAAAA;
        pframe->msgData[PMDxMDPRD] = 0x6AAAAAAA;
        pframe->msgData[PMDxCMPU] = 0x7AAAAAAA;
        pframe->msgData[PMDxCMPV] = 0x8AAAAAAA;
        pframe->msgData[PMDxCMPW] = 0x9AAAAAAA;
        pframe->msgData[PMDxMODESEL] = 0x10AAAAAA;
        pframe->msgData[PMDxMDOUT] = 0x11AAAAAA;
        pframe->msgData[PMDxMDPOT] = 0x12AAAAAA;
        pframe->msgData[PMDxEMGREL] = 0x13AAAAAA;
        pframe->msgData[PMDxEMGCR] = 0x14AAAAAA;
        pframe->msgData[PMDxEMGSTA] = 0x15AAAAAA;
        pframe->msgData[PMDxOVVCR] = 0x16AAAAAA;
        pframe->msgData[PMDxOVVSTA] = 0x17AAAAAA;
        pframe->msgData[PMDxDTR] = 0x18AAAAAA;
        pframe->msgData[PMDxTRGCMP0] = 0x19AAAAAA;
        pframe->msgData[PMDxTRGCMP1] = 0x20AAAAAA;
        pframe->msgData[PMDxTRGCMP2] = 0x21AAAAAA;
        pframe->msgData[PMDxTRGCMP3] = 0x22AAAAAA;
        pframe->msgData[PMDxTRGCR] = 0x23AAAAAA;
        pframe->msgData[PMDxTRGMD] = 0x24AAAAAA;
        pframe->msgData[PMDxTRGSEL] = 0x25AAAAAA;
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

void sendFrame(const Frame_t frame)
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

void receiveFrame(void)
{
    unsigned int *pframeG = (unsigned int *)&frameGrx;
    unsigned int *pframeS = (unsigned int *)&frameSrx;
    unsigned int *pframeVE = (unsigned int *)&frameVErx;
    static char msgCount = 0;

    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        if (data_buffer[i] == 0xABCD)
        {
            if (data_buffer[i + 2] == Msg_G_Register)
            {
                for (int j = 0; j < (FRAME_HEADER_SIZE_WORD + data_length[0]) && (i + j) < BUFFER_SIZE; j++)
                {
                    pframeG[j] = data_buffer[i + j];
                }
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

void *App_Comm_Send(void *arg)
{

    int msgId;
    while (1)
    {

        pthread_mutex_lock(&mutex);

        for (msgId = 0; msgId < (int)max_message_number; msgId++)
        {

            buildFrame(&frameInfoTx, (Msg_Register_t)msgId);
            sendFrame(frameInfoTx);
        }
        seqNoCounter++;
        printf("start Buffer \n");
        for (int i = 0; i < BUFFER_SIZE; i++)
        {
            /* code */
            printf("%X  \n", data_buffer[i]);
        }
        printf("End Buffer \n");

        pthread_mutex_unlock(&mutex);
        Sleep(500);
    }
}

void *App_Comm_Receive(void *arg)
{
    int msgId;
    while (1){
    pthread_mutex_lock(&mutex);
    receiveFrame();
    unsigned int *pframeG = (unsigned int *)&frameGrx;
    unsigned int *pframeS = (unsigned int *)&frameSrx;
    unsigned int *pframeVE = (unsigned int *)&frameVErx;
    printf("\n---------------START PMD FRAME----------------------\n");
    for (int i = 0; i < data_length[0] + FRAME_HEADER_SIZE_WORD; i++)
    {
        printf("%x \t", pframeG[i]);
    }
    printf("\n---------------START SPECIFIC FRAME----------------------\n");

    for (int i = 0; i < data_length[1] + FRAME_HEADER_SIZE_WORD; i++)
    {
        printf("%x \t", pframeS[i]);
    }
    printf("\n------------------ START VECTOR ENGINGE FRAME-------------------\n");
    for (int i = 0; i < data_length[2] + FRAME_HEADER_SIZE_WORD; i++)
    {
        printf("%x \t", pframeVE[i]);
    }
    printf("\n-------------------------------------\n");
    pthread_mutex_unlock(&mutex);
    Sleep(500);
    }
}
/*
int main(void)
{
    pthread_mutex_init(&mutex, NULL);
    pthread_t t1, t2;

    pthread_create(&t1, NULL, &App_Comm_Send, NULL);
    pthread_create(&t2, NULL, &App_Comm_Receive, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
*/
unsigned int frame_Check_Sum(Frame_t *frameInfo)
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
Frame_t getRegisterG()
{
    return frameGrx;
}
Frame_t getRegisterS()
{
    return frameSrx;
}
Frame_t getRegisterVE()
{
    return frameVErx;
}
