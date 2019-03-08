/*
 * motion_controller.c
 *
 *  Created on: Apr 8, 2017
 *      Author: kbumsik
 */

#include "cmd.h"
#include "pid.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "control_loop_thread.h"

/*******************************************************************************
 * Private variables and functions
 ******************************************************************************/
static SemaphoreHandle_t cmd_semphr = NULL;

/*******************************************************************************
 * Function definition
 ******************************************************************************/
void cmd_init(void)
{
    // /* Get queue from the controll loop */
    cmd_semphr = control_loop_thread_get_cmd_semphr();
    return;
}

void cmd_low_pid_and_go(struct cmd_pid *cmd, struct cmd_events *events)
{
    // process events
    if (events != NULL) {
        if (events->on_start != NULL) {
            events->on_start();
        }
    }
    // int32_t step_left;  //< if it is zero, not target step
    // int32_t step_right; //< if it is zero, not target step
    struct cmd_command commend = {
        .type = CMD_LOW_SET_PID_AND_GO,
        .pid = *cmd,
    };
    control_loop_send_commend(&commend);

    // process events
    if (events != NULL) {
        if (events->on_completed != NULL) {
            events->on_completed();
        }
    }
}

void cmd_low_pid_reset_and_stop(struct cmd_events *events)
{
    // process events
    if (events != NULL) {
        if (events->on_start != NULL) {
            events->on_start();
        }
    }

    struct cmd_command commend = {
        .type = CMD_LOW_RESET_PID_AND_STOP,
    };
    control_loop_send_commend(&commend);

    // process events
    if (events != NULL) {
        if (events->on_completed != NULL) {
            events->on_completed();
        }
    }
}

void cmd_polling(enum cmd_type type)
{
    struct cmd_command commend = {
        .type = type,
    };
    control_loop_send_commend(&commend);

    // Wait for semaphore
    xSemaphoreTake(cmd_semphr, portMAX_DELAY);
}
