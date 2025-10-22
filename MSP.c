#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* ---- Hardware-abstraktion: UDFYLD disse til din platform ---- */
bool     button_is_pressed(void);           // returnér true når P2.1 er trykket
uint8_t  dipswitch_read(void);              // læs heltal fra dipswitch (fx 0..255)
void     oled_init(void);                   // initier OLED
void     oled_clear(void);                  // ryd skærm
void     oled_print(const char *s);         // skriv tekst på OLED
void     delay_ms(unsigned ms);             // millisekund-forsinkelse
/* -------------------------------------------------------------- */

/* Vent på ét ‘klik’ (tryk+slip) med simpel debounce */
static void wait_for_click(void) {
    // vent til trykket påbegyndes
    while (!button_is_pressed()) { /* idle */ }
    delay_ms(20); // debounce
    // vent til knappen slippes igen
    while (button_is_pressed()) { /* hold */ }
    delay_ms(20); // debounce
}

int main(void) {
    oled_init();
    oled_clear();
    oled_print("Kalkulator klar");

    char buf[16];
    while (1) {
        // 1) Første tal
        wait_for_click();                 // "Er P2.1 trykket?" -> Ja
        uint16_t v1 = dipswitch_read();   // læs til variabel 1

        // 2) Andet tal
        wait_for_click();                 // "Er P2.1 trykket?" -> Ja
        uint16_t v2 = dipswitch_read();   // læs til variabel 2

        // 3) Summér
        uint16_t sum = v1 + v2;

        // 4) Formatér til ASCII
        snprintf(buf, sizeof(buf), "%u", (unsigned)sum);

        // 5) Skriv resultat på OLED
        oled_clear();
        oled_print(buf);

        // (flowchart: tilbage til start og vent på næste knaptryk)
    }
    return 0;
}
