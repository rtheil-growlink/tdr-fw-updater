#include "main.h"

#define TAG "SDI-12"

void initSdi12()
{
	// port 1
    gpio_set_pull_mode(SDI0, GPIO_FLOATING);
	sdi12_bus_config_t config_port0 = {
		.gpio_num = SDI0,
		.rmt_channel = RMT_CHANNEL_0,
	};
	_sdi12Bus[0] = sdi12_bus_init(&config_port0);

	// port 2
    gpio_set_pull_mode(SDI1, GPIO_FLOATING);
	sdi12_bus_config_t config_port1 = {
		.gpio_num = SDI1,
		.rmt_channel = RMT_CHANNEL_1,
	};
	_sdi12Bus[1] = sdi12_bus_init(&config_port1);

	// port 3
    gpio_set_pull_mode(SDI2, GPIO_FLOATING);
	sdi12_bus_config_t config_port2 = {
		.gpio_num = SDI2,
		.rmt_channel = RMT_CHANNEL_2,
	};
	_sdi12Bus[2] = sdi12_bus_init(&config_port2);

	// port 4
    gpio_set_pull_mode(SDI3, GPIO_FLOATING);
	sdi12_bus_config_t config_port3 = {
		.gpio_num = SDI3,
		.rmt_channel = RMT_CHANNEL_3,
	};
	_sdi12Bus[3] = sdi12_bus_init(&config_port3);
}


void app_main(void)
{
    initSdi12();

    //which bus to use
    int bus = 0;

    //query sensor on bus
    char address;
    esp_err_t err = _sdi12Bus[bus]->address_query(_sdi12Bus[0], &address, 250);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "No sensor found on bus %i", bus);
        return;
    }

    //identify sensor on bus
    char response_out[75];
    err = _sdi12Bus[bus]->read_identification(_sdi12Bus[bus], address, response_out, sizeof(response_out), 400);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error identifying sensor on port 1: %s", esp_err_to_name(err));
        return;
    }

    ESP_LOGI(TAG, "Identify response: %s", response_out);

    //continuously measure sensor on bus
    while (true)
    {
        uint8_t num_values;
        ESP_LOGI(TAG, "Measuring address %c on bus %i", address, bus);
        if (_sdi12Bus[bus]->start_measurement(_sdi12Bus[bus], address, &num_values, 0) == ESP_OK)
        {
            ESP_LOGI(TAG, "Sensor provides %d values", num_values);
            err = _sdi12Bus[0]->read_values(_sdi12Bus[bus], address, 0, response_out, sizeof(response_out), 0);
            if (err == ESP_OK)
            {
                ESP_LOGW(TAG, "reading: %s", response_out);
            }
            else
            {
                ESP_LOGE(TAG, "Error getting readings from sensor on bus %i address %c: %s", bus, address, esp_err_to_name(err));
            }
        }
        else
        {
            ESP_LOGE(TAG, "Error measuring sensor on bus %i address %c: %s", bus, address, esp_err_to_name(err));
            return;
        }

        //delay for 2000ms
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }

    // Example raw command
    //_sdi12Bus[bus]->raw_cmd(_sdi12Bus[bus], address, "I", response_out, sizeof(response_out), 1000);
}