/*
  Copyright (c) 2006-2008 Gordon Gremme <gremme@zbh.uni-hamburg.de>
  Copyright (c) 2006-2008 Center for Bioinformatics, University of Hamburg

  Permission to use, copy, modify, and distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <assert.h>
#include <limits.h>
#include <string.h>
#include "core/alpha.h"
#include "core/ma.h"
#include "core/undef.h"
#include "core/xansi.h"

#define DNA_WILDCARDS                "nsywrkvbdhmNSYWRKVBDHM"
#define PROTEIN_CHARACTERS_UPPERCASE "LVIFKREDAGSTNQYWPHMC"
#define PROTEIN_CHARACTERS_LOWERCASE "lvifkredagstnqywphmc"
#define PROTEIN_WILDCARDS            "XUBZ*-"

#define ALPHA_GUESS_MAX_LENGTH       5000
#define ALPHA_GUESS_PROTEIN_CHARS    "LIFEQPlifeqpXZ*-"

struct GtAlpha {
  unsigned char code_to_character_map[UCHAR_MAX];
  unsigned int character_to_code_map[UCHAR_MAX],
               map_size,
               reference_count;
};

GtAlpha* gt_alpha_new(void)
{
  GtAlpha *a = gt_calloc(1, sizeof (GtAlpha));
  memset(a->code_to_character_map, UNDEF_UCHAR, UCHAR_MAX);
  memset(a->character_to_code_map, UNDEF_UCHAR, UCHAR_MAX);
  return a;
}

GtAlpha* gt_alpha_new_dna(void)
{
  unsigned int i;
  GtAlpha *a = gt_alpha_new();

  /* fill the code to character map */
  a->code_to_character_map[0] = 'a';
  a->code_to_character_map[1] = 'c';
  a->code_to_character_map[2] = 'g';
  a->code_to_character_map[3] = 't';
  a->code_to_character_map[4] = DNA_WILDCARDS[0];

  /* fill the character to code map */
  a->character_to_code_map['a'] = a->character_to_code_map['A'] = 0;
  a->character_to_code_map['c'] = a->character_to_code_map['C'] = 1;
  a->character_to_code_map['g'] = a->character_to_code_map['G'] = 2;
  a->character_to_code_map['t'] = a->character_to_code_map['T'] =
  a->character_to_code_map['u'] = a->character_to_code_map['U'] = 3;
  for (i = 0; DNA_WILDCARDS[i] != '\0'; i++)
    a->character_to_code_map[(int) DNA_WILDCARDS[i]] = 4;

  /* set the size */
  a->map_size = 5;

  return a;
}

GtAlpha* gt_alpha_new_protein(void)
{
  unsigned int i;
  GtAlpha *a = gt_alpha_new();

  /* fill the code to character map */
  for (i = 0; PROTEIN_CHARACTERS_UPPERCASE[i] != '\0'; i++)
    a->code_to_character_map[i] = PROTEIN_CHARACTERS_UPPERCASE[i];
  a->code_to_character_map[i] = PROTEIN_WILDCARDS[0];

  /* fill the character to code map */
  for (i = 0; PROTEIN_CHARACTERS_UPPERCASE[i] != '\0'; i++)
    a->character_to_code_map[(int) PROTEIN_CHARACTERS_UPPERCASE[i]] = i;
  for (i = 0; PROTEIN_CHARACTERS_LOWERCASE[i] != '\0'; i++)
    a->character_to_code_map[(int) PROTEIN_CHARACTERS_LOWERCASE[i]] = i;
  for (i = 0; PROTEIN_WILDCARDS[i] != '\0'; i++)
    a->character_to_code_map[(int) PROTEIN_WILDCARDS[i]] = 20;

  /* set the size */
  a->map_size = 21;

  return a;
}

GtAlpha* gt_alpha_guess(const char *seq, unsigned long seqlen)
{
  unsigned long i;
  assert(seq && seqlen);
  for (i = 0; i < seqlen && i < ALPHA_GUESS_MAX_LENGTH; i++) {
    if (strchr(ALPHA_GUESS_PROTEIN_CHARS, seq[i]))
      return gt_alpha_new_protein();
  }
  return gt_alpha_new_dna();
}

GtAlpha* gt_alpha_ref(GtAlpha *a)
{
  assert(a);
  a->reference_count++;
  return a;
}

void gt_alpha_add_mapping(GtAlpha *a, const char *characters)
{
  size_t i, num_of_characters;
  assert(a && characters && a->map_size < UCHAR_MAX-1);
  num_of_characters = strlen(characters);
  assert(num_of_characters);
  a->character_to_code_map[(int) characters[0]] = a->map_size;
  a->code_to_character_map[a->map_size] = characters[0];
  for (i = 1; i < num_of_characters; i++)
    a->character_to_code_map[(int) characters[i]] = a->map_size;
  a->map_size++;
}

char gt_alpha_decode(const GtAlpha *a, unsigned int c)
{
  assert(a);
  assert(a->code_to_character_map[c] != UNDEF_UCHAR);
  return a->code_to_character_map[c];
}

unsigned int gt_alpha_encode(const GtAlpha *a, char c)
{
  assert(a);
  assert(a->character_to_code_map[(int) c] != UNDEF_UCHAR);
  return a->character_to_code_map[(int) c];
}

void gt_alpha_decode_seq(const GtAlpha *a, char *out, char *in,
                         unsigned long length)
{
  unsigned long i;
  assert(a && out && in);
  for (i = 0; i < length; i++) {
    assert(a->code_to_character_map[(int) in[i]] != UNDEF_UCHAR);
    out[i] = a->code_to_character_map[(int) in[i]];
  }
}

void gt_alpha_encode_seq(const GtAlpha *a, char *out, char *in,
                         unsigned long length)
{
  unsigned long i;
  assert(a && out && in);
  for (i = 0; i < length; i++) {
    assert(a->character_to_code_map[(int) in[i]] != UNDEF_UCHAR);
    out[i] = (char) a->character_to_code_map[(int) in[i]];
  }
}

bool gt_alpha_char_is_valid(const GtAlpha *a, char c)
{
  if (a->character_to_code_map[(int) c] == UNDEF_CHAR)
    return false;
  return true;
}

bool gt_alpha_is_compatible_with_alpha(const GtAlpha *gt_alpha_a,
                                            const GtAlpha *gt_alpha_b)
{
  assert(gt_alpha_a && gt_alpha_b);
  if (gt_alpha_a->map_size == gt_alpha_b->map_size)
    return true;
  return false;
}

unsigned int gt_alpha_size(const GtAlpha *a)
{
  assert(a);
  return a->map_size;
}

void gt_alpha_delete(GtAlpha *a)
{
  if (!a) return;
  if (a->reference_count) { a->reference_count--; return; }
  gt_free(a);
}
