#include "Sdmmc.h"
#include <sys/stat.h>
#include "driver/sdmmc_host.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"
#include "esp_log.h"

static const char* TAG = "LOG:";

void CSdmmc::Init()
{
    ESP_LOGI(TAG, "Initializing SD card using SDMMC peripheral");
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();

    host.flags = SDMMC_HOST_FLAG_1BIT;
    host.max_freq_khz = 10000;

    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.width = 1;
    gpio_set_pull_mode(GPIO_NUM_15, GPIO_PULLUP_ONLY);   // CMD, needed in 4- and 1- line modes
    gpio_set_pull_mode(GPIO_NUM_2, GPIO_PULLUP_ONLY);    // D0, needed in 4- and 1-line modes
    gpio_set_pull_mode(GPIO_NUM_4, GPIO_PULLUP_ONLY);    // D1, needed in 4-line mode only
    gpio_set_pull_mode(GPIO_NUM_12, GPIO_PULLUP_ONLY);   // D2, needed in 4-line mode only
    gpio_set_pull_mode(GPIO_NUM_13, GPIO_PULLUP_ONLY);   // D3, needed in 4- and 1-line modes

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 4 * 1024
    };

    sdmmc_card_t* card;
    esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount filesystem! If you want the card to be formatted, set format_if_mount_failed = true.");
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize the card (%s)! Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return;
    }
    sdmmc_card_print_info(stdout, card);

    struct stat st;
    if(stat("/sdcard/LogFiles", &st) == -1)
    {
        mkdir("/sdcard/LogFiles", 0777);
    }
}

#define NAME_DATE 0
bool CSdmmc::CreateFile()
{
    bool rc = false;
    char filename[64];

    if (NAME_DATE)
    {
        // date time ..2020_12_14.txt
        timeval timeval;
        gettimeofday(&timeval, NULL);
        time_t t = timeval.tv_sec;
        tm* now = localtime( &t );
        char buffer[32];
        strftime (buffer,32,"%Y_%m_%d_%H_%M_%S.txt",now);
        snprintf(filename, sizeof(filename), "/sdcard/LogFiles/%s", buffer);
    }
    else
    {
        struct stat buf;
        int i = 1;
        // incrementing number ..1.txt
        do
        {
            snprintf(filename, sizeof(filename), "/sdcard/LogFiles/Data%d.txt", i++);
        } while (stat(filename, &buf) == 0) ;
    }

    mOfstream.open(filename, std::ofstream::out | std::ofstream::app);
    if(!mOfstream)
    {
        ESP_LOGE(TAG,"Failed to open file for writing!");
    }
    else
    {
        rc = true;
    }

    return rc;
}

void CSdmmc::PutString(const std::string& in_dataString)
{
    mOfstream << in_dataString;
}

void CSdmmc::Finish()
{
    mOfstream.close();
}