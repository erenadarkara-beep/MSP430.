#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* --------- Hardware-abstraktion: UDFYLD til din platform --------- */
bool     btn_read_pressed(void);      // P2.1: læse-knap (samler v1/v2)
bool     btn_mode_pressed(void);      // P1.1: vælg operation
uint8_t  dipswitch_read(void);        // læs tal fra dipswitch (0..255)
void     oled_init(void);
void     oled_clear(void);
void     oled_print(const char *s);
void     delay_ms(unsigned ms);
/* ----------------------------------------------------------------- */

static void wait_click(bool (*btn)(void)) {
    while (!btn()) { /* idle */ }
    delay_ms(20);
    while (btn()) { /* hold */ }
    delay_ms(20);
}

static const char *op_to_str(uint8_t op){
    switch (op & 3){
        case 0: return "+";
        case 1: return "-";
        case 2: return "*";
        default:return "/";
    }
}

int main(void) {
    oled_init();
    oled_clear();
    oled_print("Kalkulator m. P1.1");

    uint8_t  v1 = 0, v2 = 0;     // operand-typer (se intervalsnak nedenfor)
    uint8_t  op = 0;             // 0:+ 1:- 2:* 3:/
    char     buf[24];

    while (1) {
        /* Skift operation ved klik på P1.1 */
        if (btn_mode_pressed()) {
            wait_click(btn_mode_pressed);
            op = (op + 1) & 3;   // modulo 4
            oled_clear();
            snprintf(buf, sizeof(buf), "Op: %s", op_to_str(op));
            oled_print(buf);
        }

        /* Læs to værdier når P2.1 klikkes to gange (som i del a) */
        wait_click(btn_read_pressed);
        v1 = dipswitch_read();

        wait_click(btn_read_pressed);
        v2 = dipswitch_read();

        /* Udfør valgt operation på v1 og v2 */
        // Brug bredere resultat-type så int ikke overflow’er
        int32_t res = 0;
        switch (op) {
            case 0:  res = (int32_t)v1 + (int32_t)v2; break;        // +
            case 1:  res = (int32_t)(int16_t)v1 - (int32_t)(int16_t)v2; break; // -
            case 2:  res = (int32_t)v1 * (int32_t)v2; break;        // *
            default: res = (v2 == 0) ? 0 : (int32_t)v1 / (int32_t)v2; break;    // /
        }

        /* Vis resultat */
        oled_clear();
        snprintf(buf, sizeof(buf), "%ld", (long)res);
        oled_print(buf);
    }
    return 0;
}
