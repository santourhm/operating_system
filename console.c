#include "console.h"
#include <string.h>

// Constants for better readability and maintenance
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8
#define TIMER_RESERVED_HEIGHT 8
#define CURSOR_THICKNESS 1

// Cursor state
static struct {
    uint32_t x;
    uint32_t y;
} cursor = {0, TIMER_RESERVED_HEIGHT};

// Direct memory access optimization
void pixel(int x , int y , uint32_t color){
    if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT) {
        ((uint32_t*)VIDEO_MEMORY)[x + y * SCREEN_WIDTH] = color;
    }
}

// Optimized character writing using bit operations
void ecrit_car(uint32_t lig, uint32_t col, char c, uint32_t color) {
    const uint8_t *char_pattern = font8x8_basic[(unsigned char)c];
    uint32_t *video_ptr = (uint32_t*)VIDEO_MEMORY + lig + col * SCREEN_WIDTH;
    
    for (int i = 0; i < CHAR_HEIGHT; i++) {
        uint8_t row = char_pattern[i];
        uint32_t *line_ptr = video_ptr + i * SCREEN_WIDTH;
        
        // Unrolled loop for better performance
        if (row & 0x01) line_ptr[0] = color; else line_ptr[0] = BACKGROUND_COLOR;
        if (row & 0x02) line_ptr[1] = color; else line_ptr[1] = BACKGROUND_COLOR;
        if (row & 0x04) line_ptr[2] = color; else line_ptr[2] = BACKGROUND_COLOR;
        if (row & 0x08) line_ptr[3] = color; else line_ptr[3] = BACKGROUND_COLOR;
        if (row & 0x10) line_ptr[4] = color; else line_ptr[4] = BACKGROUND_COLOR;
        if (row & 0x20) line_ptr[5] = color; else line_ptr[5] = BACKGROUND_COLOR;
        if (row & 0x40) line_ptr[6] = color; else line_ptr[6] = BACKGROUND_COLOR;
        if (row & 0x80) line_ptr[7] = color; else line_ptr[7] = BACKGROUND_COLOR;
    }
}

// Optimized cursor handling
static inline void update_cursor(uint32_t x, uint32_t y, uint32_t color) {
    uint32_t *line_ptr = (uint32_t*)VIDEO_MEMORY + x + (y + CHAR_HEIGHT - 1) * SCREEN_WIDTH;
    for (uint32_t i = 0; i < CHAR_WIDTH; i++) {
        line_ptr[i] = color;
    }
}

void place_curseur(uint32_t lig, uint32_t col) {
    if (lig >= SCREEN_WIDTH || col >= SCREEN_HEIGHT) {
        return;
    }
    cursor.x = lig;
    cursor.y = col;
    update_cursor(cursor.x, cursor.y, WRITING_COLOR);
}

void efface_curseur(void) {
    update_cursor(cursor.x, cursor.y, BACKGROUND_COLOR);
}

// Optimized screen clearing
void efface_ecran(void) {
    uint32_t *video_memory = (uint32_t*)VIDEO_MEMORY;
    uint32_t size = SCREEN_WIDTH * SCREEN_HEIGHT;
    
    // Preserve timer area
    memset(video_memory, BACKGROUND_COLOR, TIMER_RESERVED_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
    
    // Clear main display area
    memset(video_memory + TIMER_RESERVED_HEIGHT * SCREEN_WIDTH, 
           BACKGROUND_COLOR, 
           (SCREEN_HEIGHT - TIMER_RESERVED_HEIGHT) * SCREEN_WIDTH * sizeof(uint32_t));
    
    cursor.x = 0;
    cursor.y = TIMER_RESERVED_HEIGHT;
    place_curseur(cursor.x, cursor.y);
}

// Optimized scrolling
void defilement(void) {
    uint32_t *video_memory = (uint32_t*)VIDEO_MEMORY;
    const size_t line_size = SCREEN_WIDTH * sizeof(uint32_t);
    const size_t scroll_start = TIMER_RESERVED_HEIGHT * SCREEN_WIDTH;
    
    // Move content up
    memmove(video_memory + scroll_start,
            video_memory + scroll_start + CHAR_HEIGHT * SCREEN_WIDTH,
            (SCREEN_HEIGHT - TIMER_RESERVED_HEIGHT - CHAR_HEIGHT) * line_size);
    
    // Clear bottom line
    memset(video_memory + (SCREEN_HEIGHT - CHAR_HEIGHT) * SCREEN_WIDTH,
           BACKGROUND_COLOR,
           CHAR_HEIGHT * line_size);
}

// Optimized character handling
void traite_car(const char c) {
    efface_curseur();
    
    switch (c) {
        case '\b':
            if (cursor.x >= CHAR_WIDTH) {
                cursor.x -= CHAR_WIDTH;
            }
            break;
            
        case '\t':
            cursor.x = (cursor.x + 63) & ~63; // Align to 64-pixel boundary
            if (cursor.x >= SCREEN_WIDTH - CHAR_WIDTH) {
                cursor.x = 0;
                cursor.y += CHAR_HEIGHT;
            }
            break;
            
        case '\n':
            cursor.y += CHAR_HEIGHT;
            cursor.x = 0;
            break;
            
        case '\f':
            efface_ecran();
            return;
            
        case '\r':
            cursor.x = 0;
            break;
            
        default:
            if (c >= 32 && c <= 126) {
                ecrit_car(cursor.x, cursor.y, c, WRITING_COLOR);
                cursor.x += CHAR_WIDTH;
                if (cursor.x >= SCREEN_WIDTH - CHAR_WIDTH) {
                    cursor.x = 0;
                    cursor.y += CHAR_HEIGHT;
                }
            }
    }
    
    if (cursor.y >= SCREEN_HEIGHT - 1) {
        defilement();
        cursor.y -= CHAR_HEIGHT;
    }
    
    place_curseur(cursor.x, cursor.y);
}

// Optimized string output
void console_putbytes(const char *s, int len) {
    for (int i = 0; i < len; i++) {
        traite_car(s[i]);
    }
}