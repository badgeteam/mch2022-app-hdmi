#include <string.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
#include "main.h"
#include "fpga_util.h"
#include "ws2812.h"

extern const uint8_t bitstream_bin_start[] asm("_binary_bitstream_bin_start");
extern const uint8_t bitstream_bin_end[] asm("_binary_bitstream_bin_end");

extern const uint8_t animation_frame_1_png_start[]  asm("_binary_animation_frame_1_png_start");
extern const uint8_t animation_frame_2_png_start[]  asm("_binary_animation_frame_2_png_start");
extern const uint8_t animation_frame_3_png_start[]  asm("_binary_animation_frame_3_png_start");
extern const uint8_t animation_frame_4_png_start[]  asm("_binary_animation_frame_4_png_start");
extern const uint8_t animation_frame_5_png_start[]  asm("_binary_animation_frame_5_png_start");
extern const uint8_t animation_frame_6_png_start[]  asm("_binary_animation_frame_6_png_start");
extern const uint8_t animation_frame_7_png_start[]  asm("_binary_animation_frame_7_png_start");
extern const uint8_t animation_frame_8_png_start[]  asm("_binary_animation_frame_8_png_start");
extern const uint8_t animation_frame_9_png_start[]  asm("_binary_animation_frame_9_png_start");
extern const uint8_t animation_frame_10_png_start[] asm("_binary_animation_frame_10_png_start");
extern const uint8_t animation_frame_11_png_start[] asm("_binary_animation_frame_11_png_start");
extern const uint8_t animation_frame_12_png_start[] asm("_binary_animation_frame_12_png_start");
extern const uint8_t animation_frame_13_png_start[] asm("_binary_animation_frame_13_png_start");
extern const uint8_t animation_frame_14_png_start[] asm("_binary_animation_frame_14_png_start");
extern const uint8_t animation_frame_15_png_start[] asm("_binary_animation_frame_15_png_start");
extern const uint8_t animation_frame_16_png_start[] asm("_binary_animation_frame_16_png_start");
extern const uint8_t animation_frame_17_png_start[] asm("_binary_animation_frame_17_png_start");
extern const uint8_t animation_frame_18_png_start[] asm("_binary_animation_frame_18_png_start");
extern const uint8_t animation_frame_19_png_start[] asm("_binary_animation_frame_19_png_start");
extern const uint8_t animation_frame_20_png_start[] asm("_binary_animation_frame_20_png_start");
extern const uint8_t animation_frame_21_png_start[] asm("_binary_animation_frame_21_png_start");
extern const uint8_t animation_frame_22_png_start[] asm("_binary_animation_frame_22_png_start");
extern const uint8_t animation_frame_23_png_start[] asm("_binary_animation_frame_23_png_start");
extern const uint8_t animation_frame_24_png_start[] asm("_binary_animation_frame_24_png_start");
extern const uint8_t animation_frame_25_png_start[] asm("_binary_animation_frame_25_png_start");
extern const uint8_t animation_frame_26_png_start[] asm("_binary_animation_frame_26_png_start");
extern const uint8_t animation_frame_27_png_start[] asm("_binary_animation_frame_27_png_start");
extern const uint8_t animation_frame_28_png_start[] asm("_binary_animation_frame_28_png_start");

extern const uint8_t animation_frame_1_png_end[]  asm("_binary_animation_frame_1_png_end");
extern const uint8_t animation_frame_2_png_end[]  asm("_binary_animation_frame_2_png_end");
extern const uint8_t animation_frame_3_png_end[]  asm("_binary_animation_frame_3_png_end");
extern const uint8_t animation_frame_4_png_end[]  asm("_binary_animation_frame_4_png_end");
extern const uint8_t animation_frame_5_png_end[]  asm("_binary_animation_frame_5_png_end");
extern const uint8_t animation_frame_6_png_end[]  asm("_binary_animation_frame_6_png_end");
extern const uint8_t animation_frame_7_png_end[]  asm("_binary_animation_frame_7_png_end");
extern const uint8_t animation_frame_8_png_end[]  asm("_binary_animation_frame_8_png_end");
extern const uint8_t animation_frame_9_png_end[]  asm("_binary_animation_frame_9_png_end");
extern const uint8_t animation_frame_10_png_end[] asm("_binary_animation_frame_10_png_end");
extern const uint8_t animation_frame_11_png_end[] asm("_binary_animation_frame_11_png_end");
extern const uint8_t animation_frame_12_png_end[] asm("_binary_animation_frame_12_png_end");
extern const uint8_t animation_frame_13_png_end[] asm("_binary_animation_frame_13_png_end");
extern const uint8_t animation_frame_14_png_end[] asm("_binary_animation_frame_14_png_end");
extern const uint8_t animation_frame_15_png_end[] asm("_binary_animation_frame_15_png_end");
extern const uint8_t animation_frame_16_png_end[] asm("_binary_animation_frame_16_png_end");
extern const uint8_t animation_frame_17_png_end[] asm("_binary_animation_frame_17_png_end");
extern const uint8_t animation_frame_18_png_end[] asm("_binary_animation_frame_18_png_end");
extern const uint8_t animation_frame_19_png_end[] asm("_binary_animation_frame_19_png_end");
extern const uint8_t animation_frame_20_png_end[] asm("_binary_animation_frame_20_png_end");
extern const uint8_t animation_frame_21_png_end[] asm("_binary_animation_frame_21_png_end");
extern const uint8_t animation_frame_22_png_end[] asm("_binary_animation_frame_22_png_end");
extern const uint8_t animation_frame_23_png_end[] asm("_binary_animation_frame_23_png_end");
extern const uint8_t animation_frame_24_png_end[] asm("_binary_animation_frame_24_png_end");
extern const uint8_t animation_frame_25_png_end[] asm("_binary_animation_frame_25_png_end");
extern const uint8_t animation_frame_26_png_end[] asm("_binary_animation_frame_26_png_end");
extern const uint8_t animation_frame_27_png_end[] asm("_binary_animation_frame_27_png_end");
extern const uint8_t animation_frame_28_png_end[] asm("_binary_animation_frame_28_png_end");

const void *anim_start_regions[] = {
    animation_frame_1_png_start,
    animation_frame_2_png_start,
    animation_frame_3_png_start,
    animation_frame_4_png_start,
    animation_frame_5_png_start,
    animation_frame_6_png_start,
    animation_frame_7_png_start,
    animation_frame_8_png_start,
    animation_frame_9_png_start,
    animation_frame_10_png_start,
    animation_frame_11_png_start,
    animation_frame_12_png_start,
    animation_frame_13_png_start,
    animation_frame_14_png_start,
    animation_frame_15_png_start,
    animation_frame_16_png_start,
    animation_frame_17_png_start,
    animation_frame_18_png_start,
    animation_frame_19_png_start,
    animation_frame_20_png_start,
    animation_frame_21_png_start,
    animation_frame_22_png_start,
    animation_frame_23_png_start,
    animation_frame_24_png_start,
    animation_frame_25_png_start,
    animation_frame_26_png_start,
    animation_frame_27_png_start,
    animation_frame_28_png_start,
};

const void *anim_end_regions[] = {
    animation_frame_1_png_end,
    animation_frame_2_png_end,
    animation_frame_3_png_end,
    animation_frame_4_png_end,
    animation_frame_5_png_end,
    animation_frame_6_png_end,
    animation_frame_7_png_end,
    animation_frame_8_png_end,
    animation_frame_9_png_end,
    animation_frame_10_png_end,
    animation_frame_11_png_end,
    animation_frame_12_png_end,
    animation_frame_13_png_end,
    animation_frame_14_png_end,
    animation_frame_15_png_end,
    animation_frame_16_png_end,
    animation_frame_17_png_end,
    animation_frame_18_png_end,
    animation_frame_19_png_end,
    animation_frame_20_png_end,
    animation_frame_21_png_end,
    animation_frame_22_png_end,
    animation_frame_23_png_end,
    animation_frame_24_png_end,
    animation_frame_25_png_end,
    animation_frame_26_png_end,
    animation_frame_27_png_end,
    animation_frame_28_png_end,
};

const size_t num_anim  = sizeof(anim_start_regions)  / sizeof(const void *);

static pax_buf_t pax_buffer;
xQueueHandle button_queue;

static uint8_t *hdmi_spi_buf = NULL;
static uint32_t fpga_base = 0;

void lcd_flush() {
    ili9341_write(get_ili9341(), pax_buffer.buf);
}

void hdmi_flush() {
    ICE40* ice40 = get_ice40();
    hdmi_spi_buf[0] = 0xe0; // Write to memory command
    for (uint8_t index = 0; index < 40; index++) {
        memcpy(&hdmi_spi_buf[1], &(((uint8_t*) pax_buffer.buf)[3840 * index]), 3840);
        ice40_send_turbo(ice40, hdmi_spi_buf, 3841);
    }
    struct fpga_wb_cmdbuf *cq = fpga_wb_alloc(1);
    fpga_wb_queue_write(cq, 1, 4, fpga_base);
    fpga_wb_exec(cq, get_ice40());
    fpga_base += 320 * 240;
}


void exit_to_launcher() {
    REG_WRITE(RTC_CNTL_STORE0_REG, 0);
    esp_restart();
}

void display_logo(const void *start, const void *end) {
    size_t len = (size_t) end - (size_t) start;
    pax_decode_png_buf(&pax_buffer, (void *) start, len, pax_buffer.type, CODEC_FLAG_EXISTING);
}

void app_main() {
    bsp_init();
    bsp_rp2040_init();
    bsp_ice40_init();
    button_queue = get_rp2040()->queue;
    pax_buf_init(&pax_buffer, NULL, 320, 240, PAX_BUF_16_565RGB);
    nvs_flash_init();
    wifi_init();
    
    hdmi_spi_buf = malloc(3841);

    esp_err_t res = ice40_load_bitstream(get_ice40(), bitstream_bin_start, bitstream_bin_end - bitstream_bin_start);
    if (res != ESP_OK) {
        pax_background(&pax_buffer, 0xFF0000);
        pax_draw_text(&pax_buffer, 0xFFFFFFFF, pax_font_saira_condensed, 18, 10, 10, "Failed to load bitstream");
        ili9341_write(get_ili9341(), pax_buffer.buf);
        rp2040_input_message_t message;
        xQueueReceive(button_queue, &message, portMAX_DELAY);
        exit_to_launcher();
    }

    uint32_t dummy;
    struct fpga_wb_cmdbuf *cq = fpga_wb_alloc(16);

    fpga_wb_queue_write(cq, 1, 0x00, 0);	        // Disable HDMI DMA
    fpga_wb_queue_write(cq, 2, 0x00, 0);	        // Disable SPI DMA

    fpga_wb_queue_write(cq, 0, 0x00, 0x00000004);   // Reset   manual control
    fpga_wb_queue_write(cq, 0, 0x00, 0x00000002);   // Request manual control
    fpga_wb_queue_write(cq, 0, 0x40, 0x35000000);   // Send SPI command 0x35 (enable QPI)
    fpga_wb_queue_read (cq, 0, 0x0c, &dummy);       // Discard RF
    fpga_wb_queue_write(cq, 0, 0x00, 0x00000004);   // Release manual control

    fpga_wb_queue_write(cq, 1, 0x04, fpga_base);    // Set HDMI base address
    fpga_wb_queue_write(cq, 1, 0x00, 1);	        // Enable HDMI DMA

    fpga_wb_queue_write(cq, 2, 0x04, fpga_base);    // Set SPI base address
    fpga_wb_queue_write(cq, 2, 0x00, 1);	        // Enable SPI DMA

    fpga_wb_exec(cq, get_ice40());
    
    printf("Ready.\r\n");
    
    // Power on LED region.
    uint8_t leds[15];
    memset(&leds, 0, sizeof(leds));
    gpio_set_direction(GPIO_SD_PWR, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_SD_PWR, true);
    // Init LEDs to off.
    ws2812_init(GPIO_LED_DATA);
    ws2812_send_data(leds, sizeof(leds));
    
    
    const pax_font_t *font = pax_font_saira_condensed;
    
    while (1) {
        for (int i = 0; i < num_anim; i++) {
            display_logo(anim_start_regions[i], anim_end_regions[i]);
            lcd_flush();
            hdmi_flush();
        }
        for (int i = 0; i < num_anim; i++) {
            display_logo(anim_start_regions[i], anim_end_regions[i]);
            pax_draw_text(&pax_buffer, 0xFFFFFFFF, font, font->default_size, 10, 10, "LCD OUTPUT");
            lcd_flush();
            display_logo(anim_start_regions[i], anim_end_regions[i]);
            pax_draw_text(&pax_buffer, 0xFFFFFFFF, font, font->default_size, 10, 10, "HDMI OUTPUT");
            hdmi_flush();
        }
    }
}
