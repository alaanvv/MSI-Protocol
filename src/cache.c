#include <string.h>
#include "cache.h"

static u8 tag_lookup(Cache* cache, u16 addr, u16* tag_out, u16* index_out) {
  u16    tag = addr / LINE_SIZE / LINE_AMOUNT;
  u16  index = addr / LINE_SIZE % LINE_AMOUNT;
  Line* line = &cache->lines[index];

  if (tag_out)   *tag_out   = tag;
  if (index_out) *index_out = index;

  u8 hit = line->state != 'I' && line->tag == tag;

  return hit;
}

static u8 cache_snoop(Cache* cache, u8 core_id, BusReq req, u16 addr, i8* read_from) {
  if (req == BUS_RD) {
    u16 index;
    u8 hit = tag_lookup(cache, addr, NULL, &index);
    if (!hit) return 0;
    char state = cache->lines[index].state;

    if (state != 'M') {
      return 1;
    }

    cache->write_back_count++;
    DEBUG("MEMÓRIA RAM ATUALIZADA (WRITE-BACK)\n");
    *read_from = core_id;
    cache->lines[index].state = 'S';
    DEBUG("ESTADO DA LINHA %d DA CACHE DO CORE %d ATUALIZADO PARA S\n", index, core_id);
    return 1;
  }
  if (req == BUS_WR) {
    u16 index;
    u8 hit = tag_lookup(cache, addr, NULL, &index);
    if (!hit) return 0;

    if (cache->lines[index].state == 'M') {
      DEBUG("MEMÓRIA RAM ATUALIZADA (WRITE-BACK)\n");
      cache->write_back_count++;
    }

    cache->force_invalidation_count++;
    cache->lines[index].state = 'I';
    DEBUG("ESTADO DA LINHA %d DA CACHE DO CORE %d ATUALIZADO PARA I\n", index, core_id);
    return 1;
  }
  if (req == BUS_RD_WR) {
    u16 index;
    u8 hit = tag_lookup(cache, addr, NULL, &index);
    if (!hit) return 0;
    char state = cache->lines[index].state;

    if (state == 'M') {
      DEBUG("MEMÓRIA RAM ATUALIZADA (WRITE-BACK)\n");
      cache->write_back_count++;
      *read_from = core_id;
    }

    cache->force_invalidation_count++;
    cache->lines[index].state = 'I';
    DEBUG("ESTADO DA LINHA %d DA CACHE DO CORE %d ATUALIZADO PARA I\n", index, core_id);
    return 1;
  }
  return 0;
}

static void bus_sig(BusReq req, u8 from_id, u16 addr, i8* read_from) {
  DEBUG("REQUISIÇÃO DE BARRAMENTO: %s\n", BusReqView[req]);

  u8 found = 0;
  u8 found_multiple = 0;
  char found_str[16] = {0};
  for (u8 c = 0; c < CORE_AMOUNT; c++) {
    if (c == from_id) continue;
    u8 _found = cache_snoop(&cores[c].cache, c, req, addr, read_from);
    found_multiple |= found && _found;
    found = found || _found;
    char buffer[32];
    sprintf(buffer, "%d ", c);
    if (_found) strcat(found_str, buffer);
  }
  if (!found) DEBUG("NENHUMA CÓPIA VÁLIDA PRESENTE\n");
  else {
    if (found_multiple) DEBUG("CÓPIA PRESENTE NAS CACHES DOS CORES %sNO ESTADO S\n", found_str);
    else DEBUG("CÓPIA PRESENTE NA CACHE DO CORE %sNO ESTADO %c\n", found_str, *read_from == -1 ? 'S' : 'M');
  }
}

void cache_rd(Cache* cache, u8 core_id, u16 addr) {
  cache->rd_count++;

  u16 tag, index;
  u8 hit = tag_lookup(cache, addr, &tag, &index);

  DEBUG("LINHA MAPEADA: 0x%02x\n", index);
  DEBUG("TAG:  0x%01x\n", tag);

  Line* line = &cache->lines[index];

  if (!hit) {
    cache->rd_err_count++;
    DEBUG("EVENTO NA CACHE: MISS DE LEITURA\n");
    i8 read_from = -1;
    bus_sig(BUS_RD, core_id, addr, &read_from);
    if (read_from == -1) DEBUG("BLOCO LIDO DA MEMÓRIA PARA A LINHA %d DA CACHE DO CORE %d\n", index, core_id);
    else                 DEBUG("BLOCO LIDO DA LINHA %d DA CACHE DO CORE %d PARA A LINHA %d DA CACHE DO CORE %d\n", index, read_from, index, core_id);
    if (line->state == 'M') {
      cache->write_back_count++;
      DEBUG("MEMÓRIA RAM ATUALIZADA (WRITE-BACK)\n");
    }
    line->tag   = tag;
    line->state = 'S';
    DEBUG("ESTADO DA LINHA %d DA CACHE DO CORE %d ATUALIZADO PARA S\n", index, core_id);
  }
  else DEBUG("EVENTO NA CACHE: HIT DE LEITURA\n");
}

void cache_wr(Cache* cache, u8 core_id, u16 addr) {
  cache->wr_count++;

  u16 tag, index;
  u8 hit = tag_lookup(cache, addr, &tag, &index);

  DEBUG("LINHA MAPEADA: 0x%02x\n", index);
  DEBUG("TAG:  0x%01x\n", tag);

  Line* line = &cache->lines[index];

  if (hit) {
    DEBUG("EVENTO NA CACHE: HIT DE ESCRITA\n");
    i8 read_from = -1;
    if (line->state == 'S')
      bus_sig(BUS_WR, core_id, addr, &read_from);
    line->tag   = tag;
    line->state = 'M';
    DEBUG("ESTADO DA LINHA %d DA CACHE DO CORE %d ATUALIZADO PARA M\n", index, core_id);
  }
  else {
    cache->wr_err_count++;
    DEBUG("EVENTO NA CACHE: MISS DE ESCRITA\n");
    i8 read_from = -1;
    bus_sig(BUS_RD_WR, core_id, addr, &read_from);
    if (read_from == -1) DEBUG("BLOCO LIDO DA MEMÓRIA PARA A LINHA %d DA CACHE DO CORE %d\n", index, core_id);
    else                 DEBUG("BLOCO LIDO DA LINHA %d DA CACHE DO CORE %d PARA A LINHA %d DA CACHE DO CORE %d\n", index, read_from, index, core_id);
    if (line->state == 'M') {
      cache->write_back_count++;
      DEBUG("MEMÓRIA RAM ATUALIZADA (WRITE-BACK)\n");
    }
    line->tag   = tag;
    line->state = 'M';
    DEBUG("ESTADO DA LINHA %d DA CACHE DO CORE %d ATUALIZADO PARA M\n", index, core_id);
  }
}
