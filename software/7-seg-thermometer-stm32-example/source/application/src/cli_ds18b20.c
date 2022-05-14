/**
 ** Name
 **   cli_ds18b20.c
 **
 ** Purpose
 **   DS18B20 temperature sensor commands
 **
 ** Revision
 **   28-Aug-2020 (SSB) [] Initial
 **   18-Dec-2021 (SSB) [] Add support for updated 1-Wire library
 **/

#include "cli.h"

#include "ds18b20.h"

#include <stdio.h>

static Cli_Ret cli_ds_list( Cli_Cmd_Args* args )
{
    Cli_Ret  ret = CLI_RET_OK;
    Ds_Hdl_t ds;

    (void) args;

    ds = ds18b20_get_hdl();
    ow_reset( &ds->ow_hdl );
    ow_list_all_devices( &ds->ow_hdl );

    return ret;
}

static Cli_Ret cli_ds_read( Cli_Cmd_Args* args )
{
    Cli_Ret  ret = CLI_RET_OK;
    Ds_Hdl_t ds;
    uint8_t  out[8] = {0};

    ds = ds18b20_get_hdl();

    if (( args->num[2] < 1 ) || ( args->num[2] > OW_MAX_DEVICES ))
    {
        ret = CLI_RET_INV_CMD;
    }
    else
    {
        ds18b20_update();
        ds18b20_temp_to_string( out, ds->last_temp_raw[args->num[2]] );
        printf( "%s\r\n", out );
    }

    return ret;
}

static const Cli_Cmd ds_cmds[] =
{
    { "list"
    , cli_ds_list
    , "List devices attached to 1-wire bus"
    }
    ,
    { "read"
    , cli_ds_read
    , "Read temperature from sensor - Usage: ds read <dev_idx>"
    }
};

const Cli_Cmd_List cmd_ds_list =
{
    "ds"
    , ds_cmds
    , sizeof ( ds_cmds ) / sizeof ( ds_cmds[0] )
    , "DS18B20 commands"
};
