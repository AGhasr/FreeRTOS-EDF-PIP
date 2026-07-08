#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "esp_attr.h"
#include "esp_cpu.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"


// Task L (low priority)
void doTaskL(void *parameters) {
    TickType_t startingTime, endTime, executionTime;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1) {
        startingTime = xTaskGetTickCount();
        printf("Task L starting time %lu.\n", (unsigned long)xLastWakeTime);

        // vLogReadyListOrder();
        // Simulate work
        
        while ((xTaskGetTickCount() - xLastWakeTime) < pdMS_TO_TICKS(500));

        printf("Task L Done!\n");
                // Record the end time
        endTime = xTaskGetTickCount();

        // Calculate the execution time in milliseconds
        executionTime = (endTime - xLastWakeTime) * portTICK_PERIOD_MS;
        
        printf("Task L took %lu ms to complete its work.\n", (unsigned long)executionTime);
        printf("Task L end time %lu.\n", (unsigned long)endTime);
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
    }
}

// Task M (medium priority)
void doTaskM(void *parameters) {
    TickType_t startingTime, endTime, executionTime;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    // vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
    while (1) {
        
        // printf("Task M starting...\n");

        // vLogReadyListOrder();
        // Simulate work
        startingTime = xTaskGetTickCount();
        printf("Task M starting time %lu.\n", (unsigned long)xLastWakeTime);

            while ((xTaskGetTickCount() - xLastWakeTime) < pdMS_TO_TICKS(300));
        
        

        printf("Task M Done!\n");
                        // Record the end time
        endTime = xTaskGetTickCount();

        // Calculate the execution time in milliseconds
        executionTime = (endTime - xLastWakeTime) * portTICK_PERIOD_MS;
        
        printf("Task M took %lu ms to complete its work.\n", (unsigned long)executionTime);
        printf("Task M end time %lu.\n", (unsigned long)endTime);
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(800));    
    }
}

// Task H (high priority)
void doTaskH(void *parameters) {
    TickType_t startingTime, endTime, executionTime;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
    
    while (1) {
        startingTime = xTaskGetTickCount();
        // printf("Task H starting...\n");
        printf("Task H starting time %lu.\n", (unsigned long)xLastWakeTime);

        // while ((xTaskGetTickCount() - startingTime) < pdMS_TO_TICKS(50));
        // vLogReadyListOrder();
        // Simulate work
        // printf("Task H will try acquiring lock1 at %lu.\n",(unsigned long)xTaskGetTickCount());
            while ((xTaskGetTickCount() - xLastWakeTime) < pdMS_TO_TICKS(100));
        
        
        
        

                        // Record the end time
        endTime = xTaskGetTickCount();

        // Calculate the execution time in milliseconds
        executionTime = (endTime - xLastWakeTime) * portTICK_PERIOD_MS;

        
        
        printf("Task H took %lu ms to complete its work.\n", (unsigned long)executionTime);
        printf("Task H end time %lu.\n", (unsigned long)endTime);
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(600));
    }
}

void busyWork(uint32_t ms) {
    TickType_t start = xTaskGetTickCount();
    TickType_t duration = pdMS_TO_TICKS(ms);
    while ((xTaskGetTickCount() - start) < duration) {
        // Simulate CPU-bound work by running an empty loop
    }
}


void app_main() {
    printf("---FreeRTOS Deadline Inheritance Demo---\n");


    xTaskCreate(doTaskL, "Task L", 2048, NULL, 0, NULL, pdMS_TO_TICKS(500));
    xTaskCreate(doTaskM, "Task M", 2048, NULL, 0, NULL, pdMS_TO_TICKS(300));
    xTaskCreate(doTaskH, "Task H", 2048, NULL, 0, NULL, pdMS_TO_TICKS(100));


    ESP_LOGI("app_main", "Starting scheduler from app_main()");
    vTaskStartScheduler();

    ESP_LOGE("app_main", "insufficient RAM! aborting");
    abort();
}
