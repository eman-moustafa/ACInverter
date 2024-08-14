#ifndef DATA_H
#define DATA_H
#include <QString>



class Data
{
public:
    Data();

    QString PMDEnableRegister = NULL;

    QString PortOutputModeRegister = NULL;

    QString PMDControlRegister = NULL;

    QString PWMCounterStatusRegister = NULL;

    QString PWMCounterRegister = NULL;

    QString PWMCycleRegister = NULL;

    QString PWMCompareURegister = NULL;

    QString PWMCompareVRegister = NULL;

    QString PWMCompareWRegister = NULL;

    QString ModeSelectRegister = NULL;

    QString PMDOutputControlRegister = NULL;

    QString PMDOutputSettingRegister = NULL;

    QString EMGReleaseRegister = NULL;

    QString EMGControlRegister = NULL;

    QString EMGStatusRegister = NULL;

    QString OVVControlRegister = NULL;

    QString OVVStatusRegister = NULL;

    QString DeadTimeRegister = NULL;

    QString TriggerCompare0Register = NULL;

    QString TriggerCompare1Register = NULL;

    QString TriggerCompare2Register = NULL;

    QString TriggerCompare3Register = NULL;

    QString TriggerControlRegister = NULL;

    QString TriggerOutputModeSettingRegister = NULL;

    QString TriggerOutputSelectRegister = NULL;

    void setPMDEnableRegister(const QString &newPMDEnableRegister);

    void setPortOutputModeRegister (const QString &newPortOutputModeRegister );

    void setPMDControlRegister (const QString &newPMDControlRegister );

    void setPWMCounterStatusRegister (const QString &newPWMCounterStatusRegister );

    void setPWMCounterRegister (const QString &newPWMCounterRegister);

    void setPWMCycleRegister (const QString &newPWMCycleRegister );


    void setPWMCompareURegister (const QString &newPWMCompareURegister );

    void setPWMCompareVRegister (const QString &newPWMCompareVRegister );

    void setPWMCompareWRegister (const QString &newPWMCompareWRegister );

    void setModeSelectRegister (const QString &newModeSelectRegister );

    void setPMDOutputControlRegister (const QString &newPMDOutputControlRegister );

    void setEMGReleaseRegister (const QString &newEMGReleaseRegister  );

    void setEMGControlRegister (const QString &newEMGControlRegister );

    void setEMGStatusRegister (const QString &newEMGStatusRegister );

    void setOVVControlRegister (const QString &newOVVControlRegister );

    void setOVVStatusRegister (const QString &newOVVStatusRegister );

    void setDeadTimeRegister (const QString &newDeadTimeRegister );

    void setTriggerCompare0Register (const QString &newTriggerCompare0Register );

    void setTriggerCompare1Register (const QString &newTriggerCompare1Register );

    void setTriggerCompare2Register (const QString &newTriggerCompare2Register );

    void setTriggerCompare3Register (const QString &newTriggerCompare3Register );

    void setTriggerControlRegister (const QString &newTriggerControlRegister );

    void setTriggerOutputModeSettingRegister (const QString &newTriggerOutputModeSettingRegister );

    void setTriggerOutputSelectRegister (const QString &newTriggerOutputSelectRegister );




};

#endif // DATA_H
