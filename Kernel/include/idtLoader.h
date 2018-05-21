#ifndef IDT_LOADER
#define IDT_LOADER

#pragma pack(push)		/* Push de la alineación actual */
#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Descriptor de interrupcion */
typedef struct {
  uint16_t offset_l, selector;
  uint8_t cero, access;
  uint16_t offset_m;
  uint32_t offset_h, other_cero;
} INT_DESCR;

#pragma pack(pop)		/* Reestablece la alinceación actual */

void loadIDT(void);

#endif