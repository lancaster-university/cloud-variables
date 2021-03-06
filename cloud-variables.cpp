#include "pxt.h"
#include "CloudVariable.h"

enum class Locations{
    school = 0,
    home = 1,
    classroom = 2,
    //%block="micro:bit foundation"
    microbitfoundation = 3,
    everyone = 4,
};

/**
 * share
 * use
 */

//% color=243 weight=100 icon="\uf0c2" block="Cloud Variables"
namespace cloudvariables {

    int init() {
        return enableRadio();
    }

    /**
      * do some stuff
      */
    //% help=none
    //% weight=96
    //% blockId=share-strict block="share|%variable|with %PERMISSIONS" blockGap=8
    //% PERMISSIONS.fieldEditor="gridpicker" PERMISSIONS.fieldOptions.columns=4
    void shareVariable(StringData* variableName, Locations l)
    {
        init();

        ManagedString nameSpace;

        switch(l)
        {
            case Locations::school:
                nameSpace = "school";
                break;
            case Locations::home:
                nameSpace = "home";
                break;
            case Locations::classroom:
                nameSpace = "classroom";
                break;
            case Locations::microbitfoundation:
                nameSpace = "microbit-foundation";
                break;
            case Locations::everyone:
            default:
                nameSpace = "everyone";
        }

        new CloudVariable(nameSpace, ManagedString(variableName), uBit.radio);
    }

    /**
      * do some stuff
      */
    //% help=none
    //% weight=96
    //% blockId=share-freeform block="share|%variable|with %whom" blockGap=8
    void shareVariableFree(StringData* variableName, StringData* whom)
    {
        init();
        new CloudVariable(ManagedString(whom), ManagedString(variableName), uBit.radio);
    }

    /**
      * do some stuff
      */
    //% help=none
    //% weight=96
    //% blockId=set_shared block="set|%variable|to %value" blockGap=8
    void setSharedVariable(StringData* variableName, StringData* value)
    {
        init();

        uint16_t variableHash = CloudVariable::pearsonHash(ManagedString(variableName));

        for (int i = 0; i < CLOUD_VARIABLE_MAX_VARIABLES; i++)
        {
            if (CloudVariable::variables[i] != NULL)
            {
                if (CloudVariable::variables[i]->variableNameHash == variableHash)
                {
                    *CloudVariable::variables[i] = ManagedString(value);
                    break;
                }
            }
        }
    }

    //%
    StringData* getSharedVariable(StringData* variableName)
    {
        init();

        uint16_t variableHash = CloudVariable::pearsonHash(ManagedString(variableName));

        for (int i = 0; i < CLOUD_VARIABLE_MAX_VARIABLES; i++)
        {
            if (CloudVariable::variables[i] != NULL)
            {
                if (CloudVariable::variables[i]->variableNameHash == variableHash)
                {
                    return CloudVariable::variables[i]->value.leakData();
                }
            }
        }

        return ManagedString("").leakData();
    }

    /**
      * do some stuff
      */
    //% help=none
    //% weight=96
    //% blockId=on_variable_changed block="on|%variable|changed" blockGap=8
    void onVariableChanged(StringData* variableName, Action body)
    {
        uint16_t variableHash = CloudVariable::pearsonHash(ManagedString(variableName));
        registerWithDal(RADIO_CLOUD_VARIABLE_ID, variableHash, body);
    }
}